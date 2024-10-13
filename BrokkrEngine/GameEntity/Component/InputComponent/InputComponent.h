#pragma once

#include "../../GameEntity.h"
#include "../../../Application/Core.h"
#include "../../../EventManager/EventManager.h"
#include "../../../GameEntity/Component/Component.h"
#include "../../../InputManager/InputManager.h"
#include "../../../Utilities/Clock.h"


///////////////////////////////////////////////////////////////////////////////////////
//      InputComponent:
// Input Component really only cares about what event you want it to send
// and will implicitly call events targeted on its Owner container id unless told otherwise.
//
// Functionality:
// can set a hold time amount that is used for length of time required to hold the key down before
// the event is triggered
//
// can set a cooldown time similar to the hold time ; but the timer tracks time sense last used
// and when that is greater then the set time amount then the input can be counted again
//
///////////////////////////////////////////////////////////////////////////////////////
namespace Brokkr
{
    class GameEntity;

    class InputComponent final : public Component
    {
        GameEntity* m_pOwner = nullptr;

        EngineSystemsRef m_systemRef = nullptr;

        InputManager::KeyCode m_inputKey;
        InputManager* m_pInputManager = nullptr;

        EventManager* m_pEventManager = nullptr;
        Event m_inputEvent;

        Clock m_eventTimer;

        float m_holdTimeAmount = 0.0f;
        float m_rechargeTimeAmount = 0.0f;

    public:

        // Sent event to Self with Hold time recharge time and event priority 
        InputComponent
        (
            GameEntity* pOwner
            , EngineSystemsRef systemRef
            , InputManager::KeyCode inputKey
            , const float holdTime
            , const float rechargeTime
            , const char* eventString
            , const unsigned int eventPrio
        )
            : m_pOwner(pOwner)
            , m_systemRef(systemRef)
            , m_inputKey(inputKey)
            , m_inputEvent(Event::EventType("InputComponentEventError", Event::kPriorityNormal))
            , m_holdTimeAmount(holdTime)
            , m_rechargeTimeAmount(rechargeTime)
        {
            const std::string eventStr = eventString + std::to_string(pOwner->GetId());
            m_inputEvent = Event::EventType(eventStr.c_str(), eventPrio);
            m_eventTimer.Restart();
        }

        // Basic input targeted at Container
        InputComponent
        (
            GameEntity* pOwner
            , EngineSystemsRef systemRef
            , InputManager::KeyCode inputKey
            , const char* eventString
        )
            : m_pOwner(pOwner)
            , m_systemRef(systemRef)
            , m_inputKey(inputKey)
            , m_inputEvent(Event::EventType("InputComponentEventError", Event::kPriorityNormal))
        {
            const std::string eventStr = eventString + std::to_string(pOwner->GetId());
            m_inputEvent = Event::EventType(eventStr.c_str(), Event::kPriorityNormal);
            m_eventTimer.Restart();
        }

        // Send event to a Target ID
        InputComponent
        (
            GameEntity* pOwner
            , EngineSystemsRef systemRef
            , InputManager::KeyCode inputKey
            , const float holdTime
            , const float rechargeTime
            , const char* eventString
            , const unsigned int eventPrio
            , int targetID
        )
            : m_pOwner(pOwner)
            , m_systemRef(systemRef)
            , m_inputKey(inputKey)
            , m_inputEvent(Event::EventType("InputComponentEventError", Event::kPriorityNormal))
            , m_holdTimeAmount(holdTime)
            , m_rechargeTimeAmount(rechargeTime)
        {
            const std::string eventStr = eventString + std::to_string(targetID);
            m_inputEvent = Event::EventType(eventStr.c_str(), eventPrio);
            m_eventTimer.Restart();
        }

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
    };
}
