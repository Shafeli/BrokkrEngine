#include "Core.h"
#include <chrono>
#include <SDL.h>
#include <SDL_events.h>
#include <thread>

#include "DeltaTime.h"
#include "../Graphics/WindowHandle/WindowHandle.h"
#include "../InputManager/InputManager.h"
#include "../Logging/Logger.h"
#include "../SceneManager/SceneManager.h"
#include "../EventManager/EventManager.h"

#define FRAME_TRACKER 0 // Log fps

void Brokkr::Core::Run()
{
    std::thread renderThread;

    // Lambda function that captures the object pointer and calls the member function
    auto renderFunc = [this]()
    {
        m_pSceneManger->GetActiveState()->Draw();
    };

#if  FRAME_TRACKER
    static int frameCount = 0;
    static double frameTime = 0;
#endif

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    SDL_Event windowEvent;

    while (m_pInputManager->IsKeyPressed(InputManager::KeyCode::ESC) == InputManager::kNoInputCode)
    {

#if  FRAME_TRACKER
        ++frameCount;
#endif

        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }

        m_pSceneManger->ProcessStateChange();

        std::chrono::time_point<std::chrono::steady_clock> thisFrameTime;
        thisFrameTime = std::chrono::high_resolution_clock::now();

        //Time between frames 
        std::chrono::duration<double> lastFrameDuration = thisFrameTime - lastFrameTime;

        //Sets the Master copy of Delta Time
        m_pDeltaTime->SetTime(lastFrameDuration.count());

        lastFrameTime = thisFrameTime;

        // Start the rendering thread
        renderThread = std::thread(renderFunc);

        m_pInputManager->Update(); // update input system // TODO: Deprecated want to move to a event driven system

        m_pSceneManger->GetActiveState()->Update(); // TODO: Deprecated want to move to a event driven system

        // start of this Update 
        m_pEventManager->PushEvent(m_updateLogicEvent); // TODO: Add Payload component to send this frames delta out to systems and components
        m_pEventManager->ProcessEvents(); // Process the update events

        m_pEventManager->PushEvent(m_updatePhysicsEvent); // Reacting to the update events
        m_pEventManager->ProcessEvents(); // Process the physics events

        m_pEventManager->PushEvent(m_loadRenderEvent); // Load the render data
        m_pEventManager->ProcessEvents(); // Process render loading events

        // Wait for the rendering
        renderThread.join();

        // End of this Update 
        m_pEventManager->PushEvent(m_frameCompleteEvent);
        m_pEventManager->ProcessEvents();

#if  FRAME_TRACKER
        frameTime += lastFrameDuration.count() * 1000.0;
        if (frameTime >= 1000)
        {
            std::string testing = "FPS: ";
            testing += std::to_string(frameCount);
            m_logger.Log(Logger::LogLevel::kDebug, testing);

            frameCount = 0;
            frameTime -= 1000.0;
        }
#endif
    }
}

void Brokkr::Core::RunTests()
{
    m_currentTesting.Test();
}

Brokkr::EngineSystemsRef Brokkr::Core::GetEngineSystems() const
{
    return m_systemRef;
}

Brokkr::Core::Core()
    : m_frameCompleteEvent(Event::EventType("FrameComplete", Event::kPriorityNormal)) // Last event to be processed
    , m_updateLogicEvent(Event::EventType("Update", Event::kPriorityMax)) // Update logic
    , m_updatePhysicsEvent(Event::EventType("PhysicsUpdate", Event::kPriorityNormal)) // Reacting to update event if any require physics
    , m_loadRenderEvent(Event::EventType("LoadRender", Event::kPriorityLow)) // loading the render pipe line for swap
    , m_logger("CoreLogs")
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Place to Initialize core systems 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Brokkr::Core::Init(const char* pGameTitle, int screenWidth, int screenHeight)
{
    m_systemRef->Init(pGameTitle, screenWidth, screenHeight);

    m_pDeltaTime = m_systemRef->GetCoreSystem<DeltaTime>();
    m_pInputManager = m_systemRef->GetCoreSystem<InputManager>();
    m_pSceneManger = m_systemRef->GetCoreSystem<SceneManager>();
    m_pEventManager = m_systemRef->GetCoreSystem<EventManager>();

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Place to Destroy core systems 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Brokkr::Core::CleanUp()
{
    m_logger.CloseFile();
    m_systemRef->GetCoreSystem<WindowHandle>()->Destroy();

    delete m_systemRef;

    return EXIT_SUCCESS;
}
