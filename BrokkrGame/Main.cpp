
#include "../BrokkrEngine/Application/Application.h"
#include "../BrokkrEngine/AssetManager/AssetManager.h"
#include "../BrokkrEngine/AssetManager/ScriptManager/ScriptAssetLoader.h"
#include "../BrokkrEngine/AssetManager/ScriptManager/ScriptAssets/LuaScript.h"
#include "../BrokkrEngine/SceneManager/SceneManager.h"
#include "../BrokkrGame/Scenes/ExampleState.h"

int main()  // NOLINT(clang-diagnostic-c++17-attribute-extensions)
{
    Brokkr::Application application; // game engine app

    application.Init("BrokkrGame", 1920, 1080); // init systems

    auto engineSystems = application.GetEngineSystems(); // get a handle to systems

    // load game states here passing systems pointer to them
    const auto pSceneManager = engineSystems->GetCoreSystem<Brokkr::SceneManager>();

    pSceneManager->AddState
    (
        "Example",
        std::make_unique<ExampleState>(engineSystems)
    );

    // Set the active state to Example
    pSceneManager->SetActiveState("Example");

    const auto pScriptAssetLoader = engineSystems->GetCoreSystem<Brokkr::AssetManager>()->GetAssetSystem<Brokkr::ScriptAssetLoader>();
    pScriptAssetLoader->AddAssetsFromFolder<Brokkr::LuaScript>("../BrokkrEngine/assets/Scripts/LuaScripts", ".lua");

    // then call run after loading game states to scene manager and setting one to active
    application.Run(); //TODO: Add a check to run making sure there is an active state before running loop

    // then at close clean / delete memory 
    const int flag = application.Clean();

    return flag;
}