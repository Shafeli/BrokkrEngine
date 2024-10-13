#pragma once
#include <string>
#include "../Component.h"
#include "../../BrokkrEngine/Application/Core.h"


namespace Brokkr
{
    class RenderPipeline;
    class GameEntity;

    class SpriteComponent final : public Component
    {
        GameEntity* m_pOwner = nullptr;
        EngineSystemsRef m_systemRef;

        std::string m_textureName;

        RenderPipeline* m_pRenderPipeline = nullptr;

    public:
        SpriteComponent(GameEntity* pOwner, std::string textureName, EngineSystemsRef systemRef);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Render() override;

        virtual void Attach() override;
        virtual void Detach() override;
        virtual void Enable() override;
        virtual void Disable() override;
        /*virtual void Serialize(const std::string& filePath) override;
        virtual void Deserialize(const std::string& filePath) override;*/
    };
}
