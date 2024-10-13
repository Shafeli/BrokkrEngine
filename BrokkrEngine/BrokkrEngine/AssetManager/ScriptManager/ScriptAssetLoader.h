#pragma once
#include <unordered_map>

#include "../AssetManager.h"

namespace Brokkr
{
    class ScriptAssetLoader;

    class Script
    {
        int m_refCount;
    public:
        explicit Script(ScriptAssetLoader* pCoreManager)
            : m_refCount(0)
            , m_pOwner(pCoreManager)
        {
            //
        }

        virtual ~Script() = default;

        virtual void Destroy() = 0;
        virtual const char* TypeName() = 0;

        static std::vector<std::string> GetAbsolutePathsFromFolder(const std::string& folderPath, const std::string& extension);
        static std::vector<std::string> GetRelativePathsFromFolder(const char* folderPath, const char* extension);

        void AddRef() { m_refCount++; }
        int RemoveRef()
        {
            return --m_refCount;
        }

    protected:
        ScriptAssetLoader* m_pOwner;
    };

    class ScriptAssetLoader final : public AssetSubsystem
    {
        inline static Logger m_fileLog{ "ScriptAssetManagerLog" };
        std::vector<const char*> m_scriptFolders; // folders that contain Scripts

        std::unordered_map<std::string ,std::unique_ptr<Script>> m_pScriptAssets;

    public:
        explicit ScriptAssetLoader(AssetManager* assetManager)
            : AssetSubsystem(assetManager)
        {
            //
        }

        [[nodiscard]] int GetNumLoadedScripts() const;
        [[nodiscard]] std::vector<std::string> GetScriptNames() const;

        [[nodiscard]] static std::string GetScriptName(const std::string& scriptPath);

        Script* GetScript(const char* scriptName);

        // TODO: not that this is a solved problem propagate to over systems either by moving the base functions on this to
        // TODO: the base class or some other way to be 
        template <typename ScriptType, typename ... Args>
        void AddAssetsFromFolder(const char* folderPath, const char* extension, Args&&... args)
        {
            const std::vector<std::string> assetPaths = Script::GetRelativePathsFromFolder(folderPath, extension);
            for (const auto& assetPath : assetPaths)
            {
                std::string assetName = GetScriptName(assetPath);
                if (!assetName.empty())
                {

                    // TODO: Document that All scripts need a name and path in their constructor ... or this brakes
                    std::string workablePath = folderPath;
                    workablePath += "/";
                    workablePath += assetPath.c_str();

                    m_scriptFolders.emplace_back(workablePath.c_str());
                    AddScriptAsset<ScriptType>(assetName.c_str(), workablePath.c_str(), std::forward<Args>(args)...);
                }
            }
        }

        // TODO: Note might be able to make a base AddAsset as well. maybe even later abstract the container functionality of
        // TODO: these loaders / component container classes so later removing std::vectors and maps will be easier
        template <typename ScriptType, typename ... Args>
        ScriptType* AddScriptAsset(const char* assetName, Args&&... args)
        {
            // Create an instance of the asset
            std::unique_ptr<ScriptType> newAsset = std::make_unique<ScriptType>(this, std::forward<Args>(args)...);

            ScriptType* result = newAsset.get(); // Get a raw pointer

            m_pScriptAssets.emplace(assetName, std::move(newAsset)); // Add the asset to the map 
            return result; // Return a pointer
        }

        void FreeScriptAsset(const char* scriptName);
        virtual void Destroy() override;
    };
}
