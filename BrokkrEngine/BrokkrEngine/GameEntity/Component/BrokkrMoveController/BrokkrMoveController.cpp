#include "BrokkrMoveController.h"

#include "../../GameEntity.h"
#include "../BrokkrVelocity/BrokkrVelocity.h"
#include "../InputComponent/InputComponent.h"


Brokkr::BrokkrMoveController::BrokkrMoveController(GameEntity* pOwner, CoreSystems* pCoreSystems, const char* up,
    const char* left, const char* right, const char* down)
    : m_pOwner(pOwner)
    , m_pCoreSystems(pCoreSystems)
{
    m_upKeyCode = (std::strcmp(up, kNoInput) == 0) ? static_cast<InputManager::KeyCode>(-1) : pCoreSystems->GetCoreSystem<InputManager>()->ToKeyCode(up);
    m_downKeyCode = (std::strcmp(down, kNoInput) == 0) ? static_cast<InputManager::KeyCode>(-1) : pCoreSystems->GetCoreSystem<InputManager>()->ToKeyCode(down);
    m_leftKeyCode = (std::strcmp(left, kNoInput) == 0) ? static_cast<InputManager::KeyCode>(-1) : pCoreSystems->GetCoreSystem<InputManager>()->ToKeyCode(left);
    m_rightKeyCode = (std::strcmp(right, kNoInput) == 0) ? static_cast<InputManager::KeyCode>(-1) : pCoreSystems->GetCoreSystem<InputManager>()->ToKeyCode(right);
}


bool Brokkr::BrokkrMoveController::Init()
{
    if (m_upKeyCode != static_cast<InputManager::KeyCode>(-1))
    {
        m_inputComponentUp = m_pOwner->AddComponent<InputComponent>(m_pCoreSystems, m_upKeyCode, "MoveUp");
        m_inputComponentUp->Init();
    }

    if (m_leftKeyCode != static_cast<InputManager::KeyCode>(-1))
    {
        m_inputComponentLeft = m_pOwner->AddComponent<InputComponent>(m_pCoreSystems, m_leftKeyCode, "MoveLeft");
        m_inputComponentLeft->Init();
    }

    if (m_downKeyCode != static_cast<InputManager::KeyCode>(-1))
    {
        m_inputComponentDown = m_pOwner->AddComponent<InputComponent>(m_pCoreSystems, m_downKeyCode, "MoveDown");
        m_inputComponentDown->Init();
    }

    if (m_rightKeyCode != static_cast<InputManager::KeyCode>(-1))
    {
        m_inputComponentRight = m_pOwner->AddComponent<InputComponent>(m_pCoreSystems, m_rightKeyCode, "MoveRight");
        m_inputComponentRight->Init();
    }

    if (const auto pVelocity = m_pOwner->AddComponent<BrokkrVelocity>(m_pCoreSystems))
	    pVelocity->Init();

    return true;
}

void Brokkr::BrokkrMoveController::Update()
{
	//
}

void Brokkr::BrokkrMoveController::Destroy()
{
	//
}

void Brokkr::BrokkrMoveController::Attach()
{
    Component::Attach();
}

void Brokkr::BrokkrMoveController::Detach()
{
    Component::Detach();
}

void Brokkr::BrokkrMoveController::Enable()
{
    Component::Enable();
}

void Brokkr::BrokkrMoveController::Disable()
{
    Component::Disable();
	// TODO: Idea: Add getting all of a Component type then add a way to disable all controls on this call or push an event
}
