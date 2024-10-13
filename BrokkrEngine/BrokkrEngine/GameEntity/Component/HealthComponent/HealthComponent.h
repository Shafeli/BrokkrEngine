#pragma once
#include "../../../Application/Core.h"
#include "../../../EventManager/EventManager.h"
#include "../../../GameEntity/GameEntity.h"
#include "../../../GameEntity/Component/Component.h"



class HealthComponent final : public Brokkr::Component
{
    Brokkr::EventManager::EventHandler m_massDecayHandler;
    Brokkr::EventManager::EventHandler m_killHandler;

    Brokkr::GameEntity* m_pOwner = nullptr;
    Brokkr::EngineSystemsRef m_systemRef;

    float m_healthPool = 133.7f; // default

public:

    HealthComponent(Brokkr::GameEntity* pOwner, Brokkr::EngineSystemsRef systemRef);

    void OnDeathEvent(const Brokkr::Event& event) const;

    [[nodiscard]] float GetHealthValue() const { return m_healthPool; }

    virtual bool Init() override;
    virtual void Update() override;
    virtual void Destroy() override;
    virtual void Enable() override;
    virtual void Disable() override;

};

