#include "LuaSystem.h"

#include <string>
#include <variant>
#include "../../ScriptSystemManager.h"
#include "../../../Application/CoreSystems/CoreSystems.h"
#include "../../../AssetManager/AssetManager.h"
#include "../../../AssetManager/ScriptManager/ScriptAssetLoader.h"
#include "../../../AssetManager/ScriptManager/ScriptAssets/LuaScript.h"
#include "../../../Lua/src/lua.hpp"

Brokkr::LuaSystem::LuaSystem(ScriptSystemManager* pSystemManager, CoreSystems* pCoreSystems)
    : ScriptSystem(pSystemManager, pCoreSystems)
{
    //
}


template <>
int Brokkr::LuaSystem::GetGlobalVariable<int>(lua_State* pLuaState, const std::string& name)
{
    lua_getglobal(pLuaState, name.c_str());
    if (lua_isnumber(pLuaState, -1))
    {
        const int result = static_cast<int>(lua_tointeger(pLuaState, -1));
        lua_pop(pLuaState, 1);
        return result;
    }
    else
    {
        // Handle error
        lua_pop(pLuaState, 1);
        return 0; 
    }
}

template <>
bool Brokkr::LuaSystem::GetGlobalVariable<bool>(lua_State* pLuaState, const std::string& name)
{
    lua_getglobal(pLuaState, name.c_str());
    if (lua_isboolean(pLuaState, -1))
    {
        const bool result = lua_toboolean(pLuaState, -1) != 0;
        lua_pop(pLuaState, 1);
        return result;
    }
    else
    {
        // Handle error 
        lua_pop(pLuaState, 1);
        return false; 
    }
}

template <>
std::string Brokkr::LuaSystem::GetGlobalVariable<std::string>(lua_State* pLuaState, const std::string& name)
{
    lua_getglobal(pLuaState, name.c_str());
    if (lua_isstring(pLuaState, -1))
    {
        std::string result(lua_tostring(pLuaState, -1));
        lua_pop(pLuaState, 1);
        return result;
    }
    else
    {
        // Handle error 
        lua_pop(pLuaState, 1);
        return ""; 
    }
}

template <>
float Brokkr::LuaSystem::GetGlobalVariable<float>(lua_State* pLuaState, const std::string& name)
{
    lua_getglobal(pLuaState, name.c_str());
    if (lua_isnumber(pLuaState, -1))
    {
        const float result = static_cast<float>(lua_tonumber(pLuaState, -1));
        lua_pop(pLuaState, 1);
        return result;
    }
    else
    {
        // Handle error 
        lua_pop(pLuaState, 1);
        return 0.0f;
    }
}

// TODO: Finish GetGlobalVariable 
template <typename Type>
Type Brokkr::LuaSystem::GetGlobalVariable(lua_State* pLuaState, const std::string& name)
{
    lua_getglobal(pLuaState, name.c_str());
    Type result = LuaTypeSpecializations::LuaType<Type>(pLuaState, -1);
    lua_pop(pLuaState, 1);
    return result;
}

template <>
void Brokkr::LuaSystem::SetGlobalVariable<int>(const std::string& name, const int& value)
{
    lua_pushinteger(m_pLuaState, value);
    lua_setglobal(m_pLuaState, name.c_str());
}

template <>
void Brokkr::LuaSystem::SetGlobalVariable<bool>(const std::string& name, const bool& value)
{
    lua_pushboolean(m_pLuaState, value);
    lua_setglobal(m_pLuaState, name.c_str());
}

template <>
void Brokkr::LuaSystem::SetGlobalVariable<std::string>(const std::string& name, const std::string& value)
{
    lua_pushstring(m_pLuaState, value.c_str());
    lua_setglobal(m_pLuaState, name.c_str());
}

template <>
void Brokkr::LuaSystem::SetGlobalVariable<float>(const std::string& name, const float& value)
{
    lua_pushnumber(m_pLuaState, value);
    lua_setglobal(m_pLuaState, name.c_str());
}

template <typename Type>
void Brokkr::LuaSystem::SetGlobalVariable(const std::string& name, const Type& value)
{

    if constexpr (std::is_same_v<Type, int>)
    {
        lua_pushinteger(m_pLuaState, value);
    }
    else if constexpr (std::is_same_v<Type, bool>)
    {
        lua_pushboolean(m_pLuaState, value);
    }
    else if constexpr (std::is_same_v<Type, std::string>)
    {
        lua_pushstring(m_pLuaState, value.c_str());
    }
    else if constexpr (std::is_same_v<Type, float>)
    {
        lua_pushnumber(m_pLuaState, value);
    }
    else
    {
        // unsupported type
        std::string error = "Unsupported type for SetGlobalVariable";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return;
    }

    lua_setglobal(m_pLuaState, name.c_str());
}

void Brokkr::LuaSystem::PushCFunction(lua_State* pLuaState, const std::function<int(lua_State*)>& func, const std::string& funcName)
{
    m_functions.emplace_back(new std::function(func));

    auto closure = [](lua_State* pLuaState) -> int
    {
        const auto funcPtr = static_cast<std::function<int(lua_State*)>*>(lua_touserdata(pLuaState, lua_upvalueindex(1)));
        return (*funcPtr)(pLuaState);
    };

    lua_pushlightuserdata(pLuaState, m_functions.back().get());
    lua_pushcclosure(pLuaState, closure, 1);
    lua_setglobal(pLuaState, funcName.c_str());
}

template<typename FuncType>
void Brokkr::LuaSystem::PushCFunction(lua_State* pLuaState, FuncType func, const std::string& funcName)
{
    m_functions.emplace_back(new std::function(func));

    lua_pushlightuserdata(pLuaState, m_functions.back().get()); // store a pointer to C++ 

    auto closure = [](lua_State* L) -> int
    {
        // Create a Lua closure for the C++ function
        const auto funcPtr = static_cast<std::function<int(lua_State*)>*>(lua_touserdata(L, lua_upvalueindex(1)));
        return (*funcPtr)(L);
    };

    lua_pushcclosure(pLuaState, closure, 1);
    lua_setglobal(pLuaState, funcName.c_str()); // register the Lua function with the  name
}

template<typename FuncType, typename... Args>
void Brokkr::LuaSystem::PushCFunction(lua_State* pLuaState, FuncType func, const std::string& funcName, Args&&... args)
{
    m_functions.emplace_back(new std::function(std::bind(func, std::forward<Args>(args)...)));
    lua_pushlightuserdata(pLuaState, m_functions.back().get()); // store a pointer to C++ 

    auto closure = [](lua_State* L) -> int
    {
        // Create a Lua closure for the C++ function
        const auto funcPtr = static_cast<std::function<int(lua_State*)>*>(lua_touserdata(L, lua_upvalueindex(1)));
        return (*funcPtr)(L);
    };

    lua_pushcclosure(pLuaState, closure, 1);
    lua_setglobal(pLuaState, funcName.c_str()); // register the Lua function with  name
}

bool Brokkr::LuaSystem::ExecuteScript(const char* scriptName)
{
    if (!m_pLuaState)
    {
        Init();
    }

    const auto script = m_scriptAssets->GetScript(scriptName);

    auto result = dynamic_cast<LuaScript*>(script);
    if (result)
    {
        return result->Execute(m_pLuaState);
    }

    // if cast fails error and show all the scriptNames in the asset system
    const auto ScriptNames = m_scriptAssets->GetScriptNames();
    std::string error = "Error Lua System :  ";
    for (auto& i : ScriptNames)
    {
        error += i;
        error += " ";
    }
    m_fileLog.Log(Logger::LogLevel::kWarning, error);
    return false;
}

std::vector<std::string> Brokkr::LuaSystem::GetScriptAssetNames() const
{
    return m_scriptAssets->GetScriptNames();
}

template <typename... Args>
void Brokkr::LuaSystem::CallLuaVoidFunction(lua_State* pLuaState, const char* functionName, Args... args)
{
    lua_getglobal(pLuaState, functionName);
    if (lua_isfunction(pLuaState, -1))
    {
        // push arguments in order
        (lua_pusharg(pLuaState, args), ...); // Fold expression to push all the arguments in one line

        const int result = lua_pcall(pLuaState, sizeof...(args), 0, 0);
        if (result != LUA_OK)
        {
            const char* errorMessage = lua_tostring(pLuaState, -1);

            std::string error = "Error calling Lua function ";
            error += functionName;
            error += ": ";
            error += errorMessage;

            m_fileLog.Log(Logger::LogLevel::kError, error);
        }
    }
    else
    {
        std::string error = "Lua function ";
        error += functionName;
        error += " not found!";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }
}

template <>
void Brokkr::LuaSystem::CallLuaVoidFunction<>(lua_State* pLuaState, const char* functionName)
{
    lua_getglobal(pLuaState, functionName);
    if (lua_isfunction(pLuaState, -1))
    {

        const int result = lua_pcall(pLuaState, 0, 0, 0);
        if (result != LUA_OK)
        {
            const char* errorMessage = lua_tostring(pLuaState, -1);

            std::string error = "Error calling Lua function ";
            error += functionName;
            error += ": ";
            error += errorMessage;

            m_fileLog.Log(Logger::LogLevel::kError, error);
        }
    }
    else
    {
        std::string error = "Lua function ";
        error += functionName;
        error += " not found!";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }
}

template <typename ReturnType, typename ... Args>
ReturnType Brokkr::LuaSystem::CallLuaFunctionWithReturn(lua_State* pLuaState, const char* functionName, Args... args)
{
    lua_getglobal(pLuaState, functionName);
    if (lua_isfunction(pLuaState, -1))
    {
        // push arguments in order
        (lua_pusharg(pLuaState, args), ...); // Fold expression to push all the arguments in one line

        const int result = lua_pcall(pLuaState, sizeof...(args), 1, 0);
        if (result != LUA_OK)
        {
            const char* errorMessage = lua_tostring(pLuaState, -1);

            std::string error = "Error calling Lua function ";
            error += functionName;
            error += ": ";
            error += errorMessage;

            m_fileLog.Log(Logger::LogLevel::kError, error);

            return ReturnType();
        }

        // Retrieve the return value
        ReturnType returnValue = LuaTypeSpecializations::LuaType<ReturnType>(pLuaState, -1);

        // Pop the return value from the stack
        lua_pop(pLuaState, 1);

        return returnValue;
    }
    else
    {
        std::string error = "Lua function ";
        error += functionName;
        error += " not found!";
        m_fileLog.Log(Logger::LogLevel::kError, error);

        return ReturnType();
    }
}

void Brokkr::LuaSystem::LoadUtilities()
{
    // Register a C++ function with Lua that maybe considered basic like logging

    auto BrokkrLogDebug = [this](lua_State* L) -> int
    {
        const char* message = luaL_checkstring(L, 1); // Retrieve the string

        m_fileLog.Log(Logger::LogLevel::kDebug, message); // Log the custom message
        return 1;
    };

    PushCFunction(m_pLuaState, BrokkrLogDebug, "BrokkrLogDebug");

    auto BrokkrLogInfo = [this](lua_State* L) -> int
    {
        const char* message = luaL_checkstring(L, 1); // Retrieve the string

        m_fileLog.Log(Logger::LogLevel::kInfo, message); // Log the custom message
        return 1;
    };

    PushCFunction(m_pLuaState , BrokkrLogInfo, "BrokkrLogInfo");

    auto BrokkrLogWarning = [this](lua_State* L) -> int
    {
        const char* message = luaL_checkstring(L, 1); // Retrieve the string

        m_fileLog.Log(Logger::LogLevel::kWarning, message); // Log the custom message
        return 1;
    };

    PushCFunction(m_pLuaState, BrokkrLogWarning, "BrokkrLogWarning");

    auto BrokkrLogError = [this](lua_State* L) -> int
    {
        const char* message = luaL_checkstring(L, 1); // Retrieve the string

        m_fileLog.Log(Logger::LogLevel::kError, message); // Log the custom message
        return 1;
    };

    PushCFunction(m_pLuaState, BrokkrLogError, "BrokkrLogError");
}

void Brokkr::LuaSystem::DumpStack()
{
    std::string luaError;
    std::string error;

    const int top = lua_gettop(m_pLuaState); // get the index to the top element / number in the stack

    for (int iElement = 1; iElement <= top; ++iElement)
    {
        switch (const int type = lua_type(m_pLuaState, iElement))
        {
        case LUA_TSTRING:

            luaError = lua_tostring(m_pLuaState, iElement);
            error = "Error Lua System : Stack dump! " + luaError;
            m_fileLog.Log(Logger::LogLevel::kWarning, error);

            break;

        case LUA_TNUMBER:

            luaError = std::to_string(lua_tonumber(m_pLuaState, iElement));
            error = "Error Lua System : Stack dump! " + luaError;
            m_fileLog.Log(Logger::LogLevel::kWarning, error);

            break;

        case LUA_TBOOLEAN:

            luaError = std::to_string(lua_toboolean(m_pLuaState, iElement));
            error = "Error Lua System : Stack dump! " + luaError;
            m_fileLog.Log(Logger::LogLevel::kWarning, error);
            break;

        case LUA_TTABLE:

            lua_pushnil(m_pLuaState); // nil to replace to save from bugs
            while (lua_next(m_pLuaState, -2))
            {
                const auto key = lua_tostring(m_pLuaState, -2);
                const auto value = lua_tostring(m_pLuaState, -1);

                luaError += key;
                luaError += " ";
                luaError += value;

                error = "Error Lua System : Stack dump! " + luaError;
                m_fileLog.Log(Logger::LogLevel::kWarning, error);

                lua_pop(m_pLuaState, 1);
            }
            lua_pop(m_pLuaState, 1); // cleans the last key to clean the stack
            break;

        default:

            luaError = lua_typename(m_pLuaState, type);
            error = "Error Lua System : Stack dump! " + luaError;
            m_fileLog.Log(Logger::LogLevel::kWarning, error);
        }
    }
}

Brokkr::LuaSystem::~LuaSystem()
{
    if (m_pLuaState)
        Destroy();
}

void Brokkr::LuaSystem::Init()
{
    if (m_pLuaState)
        return;

    // Create Lua state
    m_pLuaState = luaL_newstate();

    // Open Lua Libs
    luaL_openlibs(m_pLuaState);

    // Brokkr Libs? i guess
    LoadUtilities();

    m_scriptAssets = m_pCoreSystems->GetCoreSystem<AssetManager>()->GetAssetSystem<ScriptAssetLoader>();

}

void Brokkr::LuaSystem::Destroy()
{
    lua_close(m_pLuaState);
}




