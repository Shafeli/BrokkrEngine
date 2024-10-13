#pragma once

#include "../../../Logging/Logger.h"


//TODO: Add more type support here and propagate this to other places in code base
struct lua_State;

namespace Brokkr
{
    class LuaTypeSpecializations
    {
        inline static Logger m_fileLog{ "LuaTypeSpecializationsLog" };
    public:

        template <typename Type>
        Type LuaType(lua_State* L, int index);

    };


}
