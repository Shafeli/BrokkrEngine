#pragma once

#include <string>

#include "../Component.h"
#include "../../../EventManager/EventManager.h"
#include "../../../Primitives/Rect.h"

#define BROKKR_TRIGGER_ALL 0       // If you want to broadcast to all Entity Containers
#define BROKKR_TRIGGER_OVERLAP 1   // If you want to trigger on any overlapping IDs
#define BROKKR_TRIGGER_SELF 2      // If you want to trigger on Component container ID

namespace Brokkr
{
    class CoreSystems;
    class Collider;
    class GameEntity;
    class PhysicsManager;

    class TriggerComponent final : public Component
    {
        inline static Logger m_fileLog{ "TriggerComponentLog" };

        Collider* m_triggerZone = nullptr;
        GameEntity* m_pOwner = nullptr;
        PhysicsManager* m_pPhysicsManager = nullptr;
        CoreSystems* m_pCoreSystems = nullptr;
        EventManager* m_pEventManager = nullptr;

        std::string m_eventStr;
        Event m_triggerEvent;
        EventManager::EventHandler m_overlapHandler;

        int m_triggerEventTarget; 
    public:
        TriggerComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rect<float> transform, const char* eventString, int Target);

        void OnEnter([[maybe_unused]] const Event& event);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;

    };
}
