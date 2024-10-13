#pragma once
#include <functional>
#include <vector>

#include "../../../Logging/Logger.h"
#include "../../../ScriptSystemManager/ScriptSystem/ScriptSystem.h"
#include "LuaTypeSpecializations.h"

struct lua_State;

namespace Brokkr
{
    class CoreSystems;
    class ScriptAssetLoader;

    class LuaSystem final : public ScriptSystem
    {
        inline static Logger m_fileLog{ "LuaSystemLog" };

        lua_State* m_pLuaState = nullptr;
        ScriptAssetLoader* m_scriptAssets = nullptr;

        std::vector<std::unique_ptr<std::function<int(lua_State*)>>> m_functions;

    public:
        explicit LuaSystem(ScriptSystemManager* pSystemManager, CoreSystems* pCoreSystems);

        virtual ~LuaSystem() override;
        virtual void Init() override;
        virtual void Destroy() override;

        //TODO: Still need to add array and table support for lua
        // Get / SetArrayElement(...);
        // Get / SetTableElement(...);

        [[nodiscard]] lua_State* GetActiveState()
        {
            if (!m_pLuaState)
                Init();

            return m_pLuaState;
        } // currently not sure if there's a benefit to having more then one state

        // Getting Variable
        template<typename Type>
        Type GetGlobalVariable(lua_State* pLuaState, const std::string& name);

        // Template specializations
        template <>
        int GetGlobalVariable<int>(lua_State* pLuaState, const std::string& name);

        template <>
        bool GetGlobalVariable<bool>(lua_State* pLuaState, const std::string& name);

        template <>
        std::string GetGlobalVariable<std::string>(lua_State* pLuaState, const std::string& name);

        template <>
        float GetGlobalVariable<float>(lua_State* pLuaState, const std::string& name);

        // Setting Variable
        template<typename Type>
        void SetGlobalVariable(const std::string& name, const Type& value);

        // Template specializations
        template <>
        void SetGlobalVariable<int>(const std::string& name, const int& value);

        template <>
        void SetGlobalVariable<bool>(const std::string& name, const bool& value);

        template <>
        void SetGlobalVariable<std::string>(const std::string& name, const std::string& value);

        template <>
        void SetGlobalVariable<float>(const std::string& name, const float& value);

        // PushCFunction(...); // Push a C function so Lua scripts can call it. std::fuction object
        void PushCFunction(lua_State* pLuaState, const std::function<int(lua_State*)>& func, const std::string& funcName);

        // PushCFunction(...); // Push a C function so Lua scripts can call it. pointer no args
        template <class FuncType>
        void PushCFunction(lua_State* pLuaState, FuncType func, const std::string& funcName);

        // PushCFunction(...); // Push a C function so Lua scripts can call it. pointer with args
        template <class FuncType, class ... Args>
        void PushCFunction(lua_State* pLuaState, FuncType func, const std::string& funcName, Args&& ... args);

        bool ExecuteScript(const char* scriptName);
        [[nodiscard]] std::vector<std::string> GetScriptAssetNames() const;

        template<typename... Args>
        void CallLuaVoidFunction(lua_State* pLuaState, const char* functionName, Args... args);

        // Template specializations
        template<>
        void CallLuaVoidFunction(lua_State* pLuaState, const char* functionName);

        template <typename ReturnType, typename... Args>
        ReturnType CallLuaFunctionWithReturn(lua_State* pLuaState, const char* functionName, Args... args);

    private:
        void LoadUtilities();

        void DumpStack();
    };
}
