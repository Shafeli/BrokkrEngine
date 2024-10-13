#pragma once

#include "../EventManager/Event/Event.h"
#include "../UnitTests/FolderAssistantTests.h"
#include "CoreSystems/CoreSystems.h"

// TODO: Tagged for Re-design the whole system / class
namespace Brokkr
{
    class DeltaTime;
    class InputManager;
    class SceneManager;
    class EventManager;

    using EngineSystemsRef = CoreSystems*;

    class Core
    {
        friend class InputManager;
        friend class SceneManager;
        friend class AssetManager;

        EngineSystemsRef m_systemRef = new CoreSystems;

        DeltaTime* m_pDeltaTime = nullptr;
        InputManager* m_pInputManager = nullptr;
        SceneManager* m_pSceneManger = nullptr;
        EventManager* m_pEventManager = nullptr;

        //TODO: Get input updating from its own event structure 

        Event m_frameCompleteEvent;
        Event m_updateLogicEvent;
        Event m_updatePhysicsEvent;
        Event m_loadRenderEvent;

        Logger m_logger;

        FolderAssistantTests m_currentTesting;

    public:
        Core();

        [[nodiscard]] bool Init(const char* pGameTitle, int screenWidth, int screenHeight);
        void Run();
        void RunTests();

        [[nodiscard]] int CleanUp();
        [[nodiscard]] EngineSystemsRef GetEngineSystems() const;
    };
}
