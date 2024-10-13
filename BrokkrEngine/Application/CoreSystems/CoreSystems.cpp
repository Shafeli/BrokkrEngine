#include "CoreSystems.h"
#include "../DeltaTime.h"
#include "../../AssetManager/AssetManager.h"
#include "../../EventManager/EventManager.h"
#include "../../GameEntityManager/GameEntityManager.h"
#include "../../Graphics/Renderer/Renderer.h"
#include "../../Graphics/RenderPipeline/RenderPipeline.h"
#include "../../Graphics/WindowHandle/WindowHandle.h"
#include "../../InputManager/InputManager.h"
#include "../../Logging/Logger.h"
#include "../../SceneManager/SceneManager.h"
#include "../../ScriptSystemManager/ScriptSystemManager.h"
#include "../../WorldTileManager/WorldTileManager.h"
#include "../../PhysicsManager/PhysicsManager.h"




bool Brokkr::CoreSystems::Init(const char* pGameTitle, int screenWidth, int screenHeight)
{
    AddCoreSystem<EventManager>();

    const auto pWindowHandle = AddCoreSystem<WindowHandle>();
    const auto pRenderer = AddCoreSystem<Renderer>();
    AddCoreSystem<InputManager>();
    AddCoreSystem<DeltaTime>();
    AddCoreSystem<SceneManager>();

    const auto pGameEntities = AddCoreSystem<GameEntityManager>();
    const auto pAssetManager = AddCoreSystem<AssetManager>();

    const auto pRenderPipeline = AddCoreSystem<RenderPipeline>();
    if (pRenderPipeline)
    {
        const std::string pass = "Pass Core System : Construct a RenderPipeline Subsystem!";
        m_fileLog.Log(Logger::LogLevel::kInfo, pass);
    }

    const auto pScriptManager = AddCoreSystem<ScriptSystemManager>();
    if (pScriptManager)
    {
        const std::string pass = "Pass Core System : Construct a Script Manager Subsystem!";
        m_fileLog.Log(Logger::LogLevel::kInfo, pass);
    }
    pScriptManager->Init();

    if (!pWindowHandle->InitWindow(pGameTitle, screenWidth, screenHeight))
    {
        return false;
    }

    const auto pWorldTileManager = AddCoreSystem<WorldTileManager>();
    if (pWorldTileManager)
    {
        const std::string pass = "Pass Core System : Construct a World Tile Manager Subsystem!";
        m_fileLog.Log(Logger::LogLevel::kInfo, pass);
    }

    const auto pPhysics = AddCoreSystem<PhysicsManager>();
    if (pPhysics)
    {
        const std::string pass = "Pass Core System : Construct a Physics Manager Subsystem!";
        m_fileLog.Log(Logger::LogLevel::kInfo, pass);
    }

    //TODO: add Init() to interface and run a init() loop maybe with sorting for future issue
    pWorldTileManager->Init();
    pGameEntities->Init();
    pRenderer->Init(pWindowHandle);
    pWindowHandle->SetBackgroundColor(0, 0, 255, 255);
    pAssetManager->Init();
    pPhysics->Init();
    pRenderPipeline->Init();



    /*for (auto& i : m_pCoreSubsystems)
    {
        i.get()->Init();
    }*/

    //RemoveCoreSystem(pScriptManager);
    return true;
}



