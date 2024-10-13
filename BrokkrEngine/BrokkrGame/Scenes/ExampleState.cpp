#include "ExampleState.h"

#include "../../BrokkrEngine/AssetManager/TextureManager/TextureManager.h"
#include "../../BrokkrEngine/AssetManager/XMLManager/XMLManager.h"
#include "../../BrokkrEngine/EventManager/Event/Event.h"
#include "../../BrokkrEngine/EventManager/Event/PayloadComponent/PayloadComponent.h"
#include "../../BrokkrEngine/GameEntity/Component/InputComponent/InputComponent.h"
#include "../../BrokkrEngine/GameEntityManager/GameEntityManager.h"
#include "../../BrokkrEngine/Graphics/RenderPipeline/RenderPipeline.h"
#include "../../BrokkrEngine/Graphics/WindowHandle/WindowHandle.h"
#include "../../BrokkrEngine/ScriptSystemManager/ScriptSystemManager.h"
#include "../../BrokkrEngine/ScriptSystemManager/ScriptSystem/LuaSystem/LuaSystem.h"
#include "../../BrokkrEngine/WorldTileManager/WorldTileManager.h"

namespace Brokkr
{
    class WorldTileManager;
}

ExampleState::ExampleState(Brokkr::EngineSystemsRef systemRef)
    : m_stateLog("ExampleStateLog")
    , m_systemRef(systemRef)
    /*, m_updateEvent(Brokkr::Event::EventType("UpdateComplete", Brokkr::Event::kPriorityNormal))*/
    , m_colorEvent(Brokkr::Event::EventType("ColorSwitch", Brokkr::Event::kPriorityNormal))
    , m_massDecayEvent(Brokkr::Event::EventType("MassDecay", Brokkr::Event::kPriorityNormal))
    , m_massMoveEvent(Brokkr::Event::EventType("MassMove", Brokkr::Event::kPriorityNormal))
{
    m_colorEventClock.Restart();
    m_massDecayEventClock.Restart();
    m_massMoveEventClock.Restart();
}

ExampleState::~ExampleState()
{
    // destructor 
}

void ExampleState::Init()
{

    // What needs to be initialized
    m_pEventManager = m_systemRef->GetCoreSystem<Brokkr::EventManager>();
    m_pEntityManager = m_systemRef->GetCoreSystem<Brokkr::GameEntityManager>();
    m_pRenderPipeline = m_systemRef->GetCoreSystem<Brokkr::RenderPipeline>();
    m_pWorldTileManager = m_systemRef->GetCoreSystem<Brokkr::WorldTileManager>();

    const auto pAssetManager = m_systemRef->GetCoreSystem<Brokkr::AssetManager>();
    const auto pTextureManager = pAssetManager->GetAssetSystem<Brokkr::TextureManager>();

    pTextureManager->LoadTexture("Test", "../BrokkrEngine/assets/Water.png");

    m_systemRef->GetCoreSystem<Brokkr::WindowHandle>()->SetBackgroundColor(0, 0, 0, 255);

    const auto pXMLManager = pAssetManager->GetAssetSystem<Brokkr::XMLManager>();
    pXMLManager->Load("../BrokkrEngine/assets/EntityBuildTest.XML");
    pXMLManager->Load("../BrokkrEngine/assets/Forst_Town_Map.tmx");

    /*if (m_updateEvent.AddComponent<Brokkr::PayloadTest>())
    {
        m_stateLog.Log(Brokkr::Logger::LogLevel::kInfo, "Success the Payload component was added to the event");
    }*/
    m_pLuaSystem = m_systemRef->GetCoreSystem<Brokkr::ScriptSystemManager>()->GetScriptSystem<Brokkr::LuaSystem>();
}

void ExampleState::ResetStateValues()
{
    // what needs to be reset to be coincided reset state
}

void ExampleState::Update()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Lua
    //
    // Notes for later :
    // research in to what people want in a scriptable object & what is expected for a scripter to have access to
    //
    // https://www.youtube.com/watch?v=szTPWbVh2YE - Using Scriptable Objects as Types for Gameplay - Unity GameDev Tutorial
    // https://www.youtube.com/watch?v=Gkb_tcVXHJo&t=3s - Game Architecture Tips - Practical Uses Of Scriptable Objects
    // https://www.youtube.com/watch?v=WLDgtRNK2VE - Game architecture with ScriptableObjects | Open Projects Devlog
    // https://www.youtube.com/watch?v=VtuSKmfrFDU - Unite '17 Seoul - ScriptableObjects What they are and why to use them
    // https://www.youtube.com/watch?v=raQ3iHhE_Kk - Unite Austin 2017 - Game Architecture with Scriptable Objects
    // https://www.youtube.com/watch?v=6vmRwLYWNRo - Unite 2016 - Overthrowing the MonoBehaviour Tyranny in a Glorious Scriptable Object Revolution
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!m_test)
    {
        // Register a C++ function with Lua
        auto func = [this]([[maybe_unused]] lua_State* L) -> int
        {
            m_stateLog.Log(Brokkr::Logger::LogLevel::kDebug, "Hello to Lua from Brokkr Game");
            return 1;
        };

        const std::function funcWrapper = func;     // Wrap function in std::function this is
                                                    // for binding the lambda and this to a object for scripting to use it 

        m_pLuaSystem->PushCFunction(m_pLuaSystem->GetActiveState(), funcWrapper, "Hello");

        int leet = 1337;
        m_pLuaSystem->SetGlobalVariable("Test", leet);

        int testInt = m_pLuaSystem->GetGlobalVariable<int>(m_pLuaSystem->GetActiveState(), "Test");
        std::string resultString = "GlobalVariable : " + std::to_string(testInt);
        m_stateLog.Log(Brokkr::Logger::LogLevel::kInfo, resultString);

        auto result = m_pLuaSystem->ExecuteScript("Lua.lua");
        if (result)
        {
            m_stateLog.Log(Brokkr::Logger::LogLevel::kInfo, "Success Lua.lua Executed");
        }

        testInt = m_pLuaSystem->GetGlobalVariable<int>(m_pLuaSystem->GetActiveState(), "Test");
        resultString = "GlobalVariable : " + std::to_string(testInt);
        m_stateLog.Log(Brokkr::Logger::LogLevel::kInfo, resultString);

        m_pLuaSystem->CallLuaVoidFunction(m_pLuaSystem->GetActiveState(), "TestLuaCalling");

        m_test = true; // test over
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Entity / Events TODO: Get Entity manager updating and rendering from events
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Logic Update

    m_pEntityManager->UpdateEntities();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering currently but this in the next refactor should be taken out and done out of the Game code and
// into the engine code
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExampleState::Draw()
{
    // TODO: Research Hardware instancing
    // The OpenGL wiki introduction to hardware instancing: https://www.khronos.org/opengl/wiki/Vertex_Rendering#Instancing
    // The SDL2_gfx library software - based instancing, which might be useful http://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/_s_d_l2__gfx_primitives_8h.html#a57aeb6d301d1f98cf6f49c6d5c50b981
    // The Lazy Foo' Productions SDL tutorials rendering textured quads using SDL2 and OpenGL: https://lazyfoo.net/tutorials/SDL/index.php

    m_pRenderPipeline->ProcessRenderQueue(); // Draws window
}

void ExampleState::Destroy()
{
    // Memory destroy place 
}

void ExampleState::Pause()
{
    // State Pause requirements 
}

void ExampleState::Resume()
{
    // State Resume requirements 
}
