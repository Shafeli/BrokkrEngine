
#include "TriggerComponent.h"
#include "../../BrokkrEngine/EventManager/Event/PayloadComponent/CollisionPayload/CollisionPayload.h"
#include "../../BrokkrEngine/PhysicsManager/PhysicsManager.h"
#include "../../BrokkrEngine/GameEntity/GameEntity.h"
Brokkr::TriggerComponent::TriggerComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rect<float> transform,
    const char* eventString, int Target)
    : m_pOwner(pOwner)
    , m_pCoreSystems(pCoreSystems)
    , m_eventStr(eventString)
    , m_triggerEvent(Event::EventType("TriggerError", Event::kPriorityNormal))
    , m_triggerEventTarget(Target)
{
    m_pPhysicsManager = pCoreSystems->GetCoreSystem<PhysicsManager>();
    m_pEventManager = pCoreSystems->GetCoreSystem<EventManager>();

    m_triggerZone = m_pPhysicsManager->CreateCollider(transform, m_pOwner->GetId(), true, BROKKR_OVERLAP_DYNAMIC);

}

void Brokkr::TriggerComponent::OnEnter(const Event& event)
{
    if (m_triggerEventTarget == BROKKR_TRIGGER_SELF)
    {
        m_pEventManager->PushEvent(m_triggerEvent);
        return;
    }
    if (m_triggerEventTarget == BROKKR_TRIGGER_ALL)
    {
        m_pEventManager->PushEvent(m_triggerEvent);
        return;
    }
    if (m_triggerEventTarget == BROKKR_TRIGGER_OVERLAP)
    {
        std::string tempStr = m_eventStr;
        tempStr += std::to_string(event.GetComponent<CollisionPayload>()->GetObjectMoving()->m_ownerID);
        m_triggerEvent = Event::EventType(tempStr.c_str(), Event::kPriorityNormal);

        m_pEventManager->PushEvent(m_triggerEvent);
        return;
    }

    m_fileLog.Log(Logger::LogLevel::kError, "Something went wrong in Trigger Event response.....");

}

bool Brokkr::TriggerComponent::Init()
{
    const auto eventStr = "OnEnter" + std::to_string(m_pOwner->GetId());
    m_overlapHandler.first = Event::kPriorityNormal;
    m_overlapHandler.second = [this](auto&& event) { OnEnter(std::forward<decltype(event)>(event)); };
    m_pEventManager->AddHandler(eventStr.c_str(), m_overlapHandler);

    if (m_triggerEventTarget == BROKKR_TRIGGER_SELF)
    {
        m_eventStr += std::to_string(m_pOwner->GetId());
        m_triggerEvent = Event::EventType(eventStr.c_str(), Event::kPriorityNormal);
    }

    return  true;
}

void Brokkr::TriggerComponent::Update()
{
    //
}

void Brokkr::TriggerComponent::Destroy()
{
    //
}
