#include "ColliderComponent.h"
#include <string>
#include "../../GameEntity.h"
#include "../TransformComponent/TransformComponent.h"
#include "../../BrokkrEngine/EventManager/Event/PayloadComponent/CollisionPayload/CollisionPayload.h"

/*
#define BROKKR_OVERLAP_STATIC 0
#define BROKKR_OVERLAP_DYNAMIC 1
#define BROKKR_OVERLAP_ALL 2
*/

Brokkr::ColliderComponent::ColliderComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rect<float> transform, int overlap, bool passable)
    : m_pOwner(pOwner)
    , m_eventStr("Blocked")
    , m_blockEvent(Event::EventType("Block", Event::kPriorityNormal))
    , m_isPassable(passable)
{
    m_pPhysicsManager = pCoreSystems->GetCoreSystem<PhysicsManager>();

    if (overlap == 1)
    {
        m_transform = m_pPhysicsManager->CreateCollider(transform, m_pOwner->GetId(), true, overlap);
    }
    else if (overlap == 0)
    {
        m_transform = m_pPhysicsManager->CreateCollider(transform, m_pOwner->GetId(), false, overlap);
    }
    else if (overlap == 3)
    {
        m_transform = m_pPhysicsManager->CreateCollider(transform, m_pOwner->GetId(), true, overlap);
    }

    m_pEventManager = pCoreSystems->GetCoreSystem<EventManager>();
}

bool Brokkr::ColliderComponent::Init()
{

    if (!m_isPassable)
    {
        const auto eventStr = "OnEnter" + std::to_string(m_pOwner->GetId());
        m_onEnterHandler.first = Event::kPriorityNormal;
        m_onEnterHandler.second = [this](auto&& event) { BlockMove(std::forward<decltype(event)>(event)); };
        m_pEventManager->AddHandler(eventStr.c_str(), m_onEnterHandler);
    }

    return true; //default
}

Brokkr::Rect<float> Brokkr::ColliderComponent::GetTransform() const
{
    if (m_transform)
        return m_transform->m_collider;
    return {};
}

void Brokkr::ColliderComponent::Update()
{
    // not needed for this component for now
}

void Brokkr::ColliderComponent::Destroy()
{
    m_pPhysicsManager->Remove(m_transform);
}

void Brokkr::ColliderComponent::BlockMove(const Event& event)
{
    CollisionPayload* data = event.GetComponent<CollisionPayload>();

    // Check if the object being collided with is the blocking collider
    if (data->GetObjectMoving()->m_ownerID == m_pOwner->GetId())
    {
        return;
    }

    Brokkr::PhysicsManager::RequestMoveCorrection(data->GetObjectMoving(), -data->GetMovingObjectsDisplacement());
}

void Brokkr::ColliderComponent::MoveTo(float x, float y)
{
    m_pPhysicsManager->RequestMove(m_transform, { x, y });
}


