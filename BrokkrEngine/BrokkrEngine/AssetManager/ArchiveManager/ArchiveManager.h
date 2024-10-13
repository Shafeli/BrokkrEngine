#pragma once
#include <memory>
#include <unordered_map>
#include "../../AssetManager/AssetManager.h"
#include "../../Logging/Logger.h"
#include "../../Utilities/FolderAssistant.h"

namespace Brokkr
{
    class ArchiveManager final : public AssetSubsystem
    {
        Logger m_archiveManagerLog;

        std::unordered_map<const char*, std::unique_ptr<FolderAssistant>> m_assetArchives;
    public:
        ArchiveManager(AssetManager* assetManager)
        :AssetSubsystem(assetManager)
        , m_archiveManagerLog("ArchiveManagerLog")
        {
            
        }

        void LoadFolder(const char* filePath); // add a folder to the Archive Manager

        char* GetAssetData(const char* assetName); // Searches for an asset with the given name

        // Searches for an asset with the given name & file name for speed
        char* GetAssetData(const char* fileName, const char* assetName);
        virtual void Destroy() override;
    };
}


