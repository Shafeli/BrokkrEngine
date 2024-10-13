#pragma once
#include "../../GameEntity.h"
#include "../../../EventManager/EventManager.h"
#include "../../../GameEntity/Component/Component.h"
#include "../../../Primitives/Rect.h"

namespace Brokkr
{
    class PhysicsManager;
    class Collider;
    class TransformComponent;

    class ColliderComponent final : public Brokkr::Component
    {
        Collider* m_transform;

        GameEntity* m_pOwner = nullptr;
        PhysicsManager* m_pPhysicsManager = nullptr;
        EventManager* m_pEventManager = nullptr;

        EventManager::EventHandler m_blockedHandler;
        EventManager::EventHandler m_onEnterHandler;

        std::string m_eventStr;
        Event m_blockEvent;

        bool m_isPassable = true;

    public:

        ColliderComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rect<float> transform, int overlap, bool passable);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;

        [[nodiscard]] Rect<float> GetTransform() const;

        void BlockMove([[maybe_unused]] const Event& event);

        void MoveTo(float x, float y);
    };
}

