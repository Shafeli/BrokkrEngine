#include "InputComponent.h"

bool Brokkr::InputComponent::Init()
{
    m_pInputManager = m_systemRef->GetCoreSystem<InputManager>();
    m_pEventManager = m_systemRef->GetCoreSystem<EventManager>();

    return true; // default to true
}

void Brokkr::InputComponent::Update()
{
    if (m_eventTimer.GetElapsedTimeInSeconds() > m_rechargeTimeAmount) // if event timer is greater then rechargeTimeAmount
    {
        if (m_pInputManager->IsKeyPressed(m_inputKey) > m_holdTimeAmount) // input key is held for the holdTimeAmount
        {
            m_pEventManager->PushEvent(m_inputEvent);   // Push the event 
            m_eventTimer.Restart();     // Restart the timer for cool down time
        }
    }
}

void Brokkr::InputComponent::Destroy()
{
    //
}