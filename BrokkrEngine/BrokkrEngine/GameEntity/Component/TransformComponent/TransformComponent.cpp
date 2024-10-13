#include "TransformComponent.h"
#include "../../GameEntity.h"
#include "../../../EventManager/EventManager.h"
#include "../ColliderComponent/ColliderComponent.h"

Brokkr::TransformComponent::TransformComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rect<float> transform)
    : m_transformLog("TransformLog")
    , m_pOwner(pOwner)
    
{
    m_transform = transform;
    m_pEventManager = pCoreSystems->GetCoreSystem<EventManager>();
}

bool Brokkr::TransformComponent::Init()
{
    return true; //default
}

void Brokkr::TransformComponent::Update()
{
    // not needed for this component for now
}

void Brokkr::TransformComponent::Destroy()
{

}

void Brokkr::TransformComponent::AddCollider(ColliderComponent* pColliderComponent)
{
    m_collider = pColliderComponent;

    const auto eventStr = "UpdatePosition" + std::to_string(m_pOwner->GetId());

    m_updateHandler.first = Event::kPriorityNormal; // update before render data is pushed
    m_updateHandler.second = [this](auto&& event) { UpdatePosition(std::forward<decltype(event)>(event)); };

    m_pEventManager->AddHandler(eventStr.c_str(), m_updateHandler);
}

void Brokkr::TransformComponent::UpdatePosition(const Event& event)
{
    if (m_collider)
    {
        m_transform.MoveTo(m_collider->GetTransform().GetPosition());
    }
}

void Brokkr::TransformComponent::Resize(float width, float height)
{
    /*m_transform->m_collider.Resize(width, height);*/
}


