#pragma once
#include "../TileComponent.h"

namespace Brokkr
{
    class TileTransformComponent;
    class RenderPipeline;

    class TileRenderComponent final : public TileComponent
    {
        std::string m_textureName;
        RenderPipeline* m_pRenderPipeline = nullptr;
        WorldTile* pOwner_DEBUG = nullptr;
        TileTransformComponent* m_pTransform = nullptr;

    public:
        TileRenderComponent(WorldTile* pOwner, CoreSystems* pCoreSystems)
            : TileComponent(pOwner, pCoreSystems)
            , m_textureName("Square")
            , pOwner_DEBUG(pOwner) // TODO: THIS IS NOT FINISHED ITS FOR DEBUGGING FIX BEFORE SUBMIT
        {
            //
        }

        void SetTexture(const std::string& textureName);

        virtual void RenderProcess() override;

        template <typename Type>
        static std::string ToString()
        {
            return "TileRenderComponent";
        }

        template <typename Type, typename ... Args>
        static std::unique_ptr<TileRenderComponent> FromString(const std::string& str, Args&&... args)
        {
            // Convert the string to a TileRenderComponent object
            if (str == ToString<TileRenderComponent>())
            {
                return std::make_unique<TileRenderComponent>(args);
            }
            return nullptr; // failed to make 
        }

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
    };
}
