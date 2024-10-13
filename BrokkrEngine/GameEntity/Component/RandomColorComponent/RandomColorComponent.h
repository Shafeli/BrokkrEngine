#pragma once

#include "../../../Application/Core.h"
#include "../../../GameEntity/Component/Component.h"
#include "../../../GameEntity/GameEntity.h"
#include "../../../Graphics/Color/Color.h"
#include "../../../EventManager/EventManager.h"
#include "../../../Utilities/Clock.h"

class RandomColorComponent final : public Brokkr::Component
{
    Brokkr::Color m_color;
    Brokkr::EventManager::EventHandler m_handler;
    Brokkr::GameEntity* m_pOwner = nullptr;
    Brokkr::EngineSystemsRef m_systemRef;

    Brokkr::Clock m_colorEventClock;
    bool m_masterSwitch = false;

public:
    RandomColorComponent(Brokkr::GameEntity* pOwner, Brokkr::EngineSystemsRef systemRef);

    void OnColorEvent(const Brokkr::Event& event);
    [[nodiscard]] Brokkr::Color GetColor() const { return m_color; }

    virtual bool Init() override;
    virtual void Update() override;
    virtual void Destroy() override;
    virtual void Enable() override;
    virtual void Disable() override;
};

