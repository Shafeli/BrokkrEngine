#include "LuaTypeSpecializations.h"
#include <string>
#include "../../../Lua/src/lua.hpp"

template <typename Type>
Type Brokkr::LuaTypeSpecializations::LuaType(lua_State* L, int index)
{
    if constexpr (std::is_same_v<Type, int>)
    {
        if (lua_isinteger(L, index))
        {
            return static_cast<int>(lua_tointeger(L, index));
        }
        if (lua_isnumber(L, index))
        {
            return static_cast<int>(lua_tonumber(L, index));
        }
        const char* str = lua_tolstring(L, index, nullptr);
        return str ? std::stoi(str) : 0;
    }
    else if constexpr (std::is_same_v<Type, bool>)
    {
        return static_cast<bool>(lua_toboolean(L, index));
    }
    else if constexpr (std::is_same_v<Type, std::string>)
    {
        size_t len;
        const char* str = lua_tolstring(L, index, &len);
        return str ? std::string(str, len) : "";
    }
    else if constexpr (std::is_same_v<Type, float>)
    {
        if (lua_isnumber(L, index))
        {
            return static_cast<float>(lua_tonumber(L, index));
        }
        const char* str = lua_tolstring(L, index, nullptr);
        return str ? std::stof(str) : 0.0f;
    }
    else
    {
        // unsupported type
        std::string error = "Unsupported type for LuaType";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }
    return {};
}