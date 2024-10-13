#include "RandomColorComponent.h"
#include "../../GameEntity.h"


RandomColorComponent::RandomColorComponent(Brokkr::GameEntity* pOwner, Brokkr::EngineSystemsRef systemRef)
    : m_pOwner(pOwner)
    , m_systemRef(systemRef)
{
    //
}

void RandomColorComponent::OnColorEvent([[maybe_unused]] const Brokkr::Event& event)
{
    m_colorEventClock.Restart();
    m_masterSwitch = true;
}

bool RandomColorComponent::Init()
{
    const auto eventStr = "OnEnter" + std::to_string(m_pOwner->GetId());

    m_handler.first = Brokkr::Event::kPriorityNormal;
    m_handler.second = [this](auto&& event) { OnColorEvent(std::forward<decltype(event)>(event)); };
    m_systemRef->GetCoreSystem<Brokkr::EventManager>()->AddHandler(eventStr.c_str(), m_handler);

    m_color.RandomColors();

    return true;
}

void RandomColorComponent::Update()
{
    if (m_colorEventClock.GetElapsedTimeInSeconds() > 1.0f)
    {
        m_masterSwitch = false;
    }

    if (m_masterSwitch)
    {
        m_color.RandomColors();
    }
}

void RandomColorComponent::Destroy()
{
    //
}

void RandomColorComponent::Enable()
{
    Component::Enable();
    m_masterSwitch = true;
}

void RandomColorComponent::Disable()
{
    Component::Disable();
    m_masterSwitch = false;
}
