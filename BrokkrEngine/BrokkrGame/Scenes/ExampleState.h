#pragma once
#include "../../BrokkrEngine/Application/Core.h"
#include "../../BrokkrEngine/Utilities/Clock.h"
#include "../../BrokkrEngine/EventManager/EventManager.h"
#include "../../BrokkrEngine/Logging/Logger.h"
#include "../../BrokkrEngine/SceneManager/State.h"

namespace Brokkr
{
    class WorldTileManager;
    class LuaSystem;
    class RenderPipeline;
    class GameEntityManager;
}

class ExampleState final : public Brokkr::State
{
    // State Variable
    Brokkr::EventManager* m_pEventManager = nullptr;
    Brokkr::GameEntityManager* m_pEntityManager = nullptr;
    Brokkr::RenderPipeline* m_pRenderPipeline = nullptr;
    Brokkr::WorldTileManager* m_pWorldTileManager = nullptr;

    Brokkr::Logger m_stateLog;
    Brokkr::EngineSystemsRef m_systemRef;

    Brokkr::Event m_colorEvent;
    Brokkr::Clock m_colorEventClock;

    Brokkr::Event m_massDecayEvent;
    Brokkr::Clock m_massDecayEventClock;

    Brokkr::Event m_massMoveEvent;
    Brokkr::Clock m_massMoveEventClock;

    Brokkr::LuaSystem* m_pLuaSystem = nullptr;

    bool m_test = false;

public:

    ExampleState(Brokkr::EngineSystemsRef systemRef);

    virtual ~ExampleState() override;                   // destructor 
    virtual void Init() override;                       // What needs to be initialized
    virtual void ResetStateValues() override;           // what needs to be reset to what to be coincided reset
    virtual void Update() override;                     // Logic Update
    virtual void Draw() override;                       // Draw calls
    virtual void Destroy() override;                    // Memory destroy place 
    virtual void Pause() override;                      // State Pause requirements 
    virtual void Resume() override;                     // State Resume requirements 
};

