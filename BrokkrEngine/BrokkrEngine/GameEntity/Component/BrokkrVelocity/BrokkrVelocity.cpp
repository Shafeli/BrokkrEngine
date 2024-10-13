#include "BrokkrVelocity.h"

#include "../../GameEntity.h"
#include "../../BrokkrEngine/Application/DeltaTime.h"
#include "../../BrokkrEngine/Application/CoreSystems/CoreSystems.h"
#include "../ColliderComponent/ColliderComponent.h"
#include "../TransformComponent/TransformComponent.h"

Brokkr::BrokkrVelocity::BrokkrVelocity(GameEntity* pOwner, CoreSystems* pCoreSystems)
    : m_pOwner(pOwner)
    , m_pCoreSystems(pCoreSystems)
{
    //
}

Brokkr::BrokkrVelocity::BrokkrVelocity(GameEntity* pOwner, CoreSystems* pCoreSystems, float speed)
    : m_speed(speed)
    , m_pOwner(pOwner)
    , m_pCoreSystems(pCoreSystems)
{
    //
}

void Brokkr::BrokkrVelocity::MoveUp(const Event& event) const
{
    const float displacement = (-m_speed * m_pDeltaTime->GetSeconds());
    m_pTransform->MoveTo(0.0f, displacement);
}

void Brokkr::BrokkrVelocity::MoveDown(const Event& event) const
{
    const float displacement = (m_speed * m_pDeltaTime->GetSeconds());
    m_pTransform->MoveTo(0.0f, displacement);
}

void Brokkr::BrokkrVelocity::MoveLeft(const Event& event) const
{
    const float displacement = (-m_speed * m_pDeltaTime->GetSeconds());
    m_pTransform->MoveTo(displacement, 0.0f);
}

void Brokkr::BrokkrVelocity::MoveRight(const Event& event) const
{
    const float displacement = (m_speed * m_pDeltaTime->GetSeconds());
    m_pTransform->MoveTo(displacement, 0.0f);
}

bool Brokkr::BrokkrVelocity::Init()
{
    std::string temp = "MoveUp" + std::to_string(m_pOwner->GetId());
    m_pTransform = m_pOwner->GetComponent<ColliderComponent>();
    m_pDeltaTime = m_pCoreSystems->GetCoreSystem<DeltaTime>();

    //up
    m_moveUpHandler.first = Event::kPriorityLow; // Process low allow for thing to happen before position change request
    m_moveUpHandler.second = [this](auto&& event) { MoveUp(std::forward<decltype(event)>(event)); };
    m_pCoreSystems->GetCoreSystem<EventManager>()->AddHandler(temp.c_str(), m_moveUpHandler);
    temp.clear();

    temp = "MoveDown" + std::to_string(m_pOwner->GetId());
    //down
    m_moveDownHandler.first = Event::kPriorityLow; // Process low allow for thing to happen before position change request
    m_moveDownHandler.second = [this](auto&& event) { MoveDown(std::forward<decltype(event)>(event)); };
    m_pCoreSystems->GetCoreSystem<EventManager>()->AddHandler(temp.c_str(), m_moveDownHandler);
    temp.clear();

    temp = "MoveLeft" + std::to_string(m_pOwner->GetId());
    //left
    m_moveLeftHandler.first = Event::kPriorityLow; // Process low allow for thing to happen before position change request
    m_moveLeftHandler.second = [this](auto&& event) { MoveLeft(std::forward<decltype(event)>(event)); };
    m_pCoreSystems->GetCoreSystem<EventManager>()->AddHandler(temp.c_str(), m_moveLeftHandler);
    temp.clear();

    temp = "MoveRight" + std::to_string(m_pOwner->GetId());
    //right
    m_moveRightHandler.first = Event::kPriorityLow; // Process low allow for thing to happen before position change request
    m_moveRightHandler.second = [this](auto&& event) { MoveRight(std::forward<decltype(event)>(event)); };
    m_pCoreSystems->GetCoreSystem<EventManager>()->AddHandler(temp.c_str(), m_moveRightHandler);

    return true;
}

void Brokkr::BrokkrVelocity::Update()
{
    //
}

void Brokkr::BrokkrVelocity::Destroy()
{
    //
}

void Brokkr::BrokkrVelocity::Enable()
{
    Component::Enable();
}

void Brokkr::BrokkrVelocity::Disable()
{
    Component::Disable();
}
