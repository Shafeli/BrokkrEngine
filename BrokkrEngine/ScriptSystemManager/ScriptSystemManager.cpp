#include "ScriptSystemManager.h"
#include "ScriptSystem/LuaSystem/LuaSystem.h"

void Brokkr::ScriptSystemManager::Init()
{
    // Place to Add Engine Script systems to the manager
    AddScriptSystem<LuaSystem>();
}

void Brokkr::ScriptSystemManager::Destroy()
{
    //
}
