#pragma once

#include "../Component.h"
#include "../../BrokkrEngine/EventManager/EventManager.h"


namespace Brokkr
{
    class ColliderComponent;
    class DeltaTime;
    class CoreSystems;
    class GameEntity;

    class BrokkrVelocity final : public Component
    {
        inline static Logger m_fileLog{ "DefaultVelocityLog" };

        float m_speed = 50.0f; // base /shrug not sure what to default to 

        GameEntity* m_pOwner = nullptr;
        CoreSystems* m_pCoreSystems = nullptr;
        ColliderComponent* m_pTransform = nullptr;
        DeltaTime* m_pDeltaTime = nullptr;

        EventManager::EventHandler m_moveUpHandler;
        EventManager::EventHandler m_moveDownHandler;
        EventManager::EventHandler m_moveLeftHandler;
        EventManager::EventHandler m_moveRightHandler;

    public:

        BrokkrVelocity
        (
            GameEntity* pOwner
            , CoreSystems* pCoreSystems

        );

        BrokkrVelocity
        (
            GameEntity* pOwner
            , CoreSystems* pCoreSystems
            , float speed

        );

        void MoveUp([[maybe_unused]] const Event& event) const;
        void MoveDown([[maybe_unused]] const Event& event) const;
        void MoveLeft([[maybe_unused]] const Event& event) const;
        void MoveRight([[maybe_unused]] const Event& event) const;

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;

        // TODO: would be cool to enable and disable movement
        // then on touch disable one objects controls and activate
        // the others controls effectively switching who you are controlling at run time
        virtual void Enable() override;
        virtual void Disable() override;

    };
}
