#include "HealthComponent.h"
#include "../../../GameEntityManager/GameEntityManager.h"
#include "../../GameEntity.h"

HealthComponent::HealthComponent(Brokkr::GameEntity* pOwner, Brokkr::EngineSystemsRef systemRef)
    : m_pOwner(pOwner)
    , m_systemRef(systemRef)
{
}

void HealthComponent::OnDeathEvent([[maybe_unused]] const Brokkr::Event& event) const
{
    m_systemRef->GetCoreSystem<Brokkr::GameEntityManager>()->DeleteEntity(m_pOwner->GetId());
}

bool HealthComponent::Init()
{
    m_massDecayHandler.first = Brokkr::Event::kPriorityNormal + m_pOwner->GetId();
    //The lambda function captures the this pointer so it has access to the OnDeathEvent member function
    m_massDecayHandler.second = [this](auto&& event) { OnDeathEvent(std::forward<decltype(event)>(event)); };

    m_systemRef->GetCoreSystem<Brokkr::EventManager>()->AddHandler("MassDecay", m_massDecayHandler);


    const std::string eventStr = "Kill" + std::to_string(m_pOwner->GetId());
    m_killHandler.first = Brokkr::Event::kPriorityNormal;
    m_killHandler.second = [this](auto&& event) { OnDeathEvent(std::forward<decltype(event)>(event)); };

    m_systemRef->GetCoreSystem<Brokkr::EventManager>()->AddHandler(eventStr.c_str(), m_killHandler);

    return true;
}

void HealthComponent::Update()
{
}

void HealthComponent::Destroy()
{
}

void HealthComponent::Enable()
{
    Component::Enable();
}

void HealthComponent::Disable()
{
    Component::Disable();
}
