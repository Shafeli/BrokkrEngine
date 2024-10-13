#pragma once
#include "../ScriptAssetLoader.h"
#include "../../../Logging/Logger.h"

#define IN_PROGRESS 0


// TODO: for later https://stackoverflow.com/questions/8936369/compile-lua-code-store-bytecode-then-load-and-execute-it
// has a good idea on how to load the bytecode with out having to deal with the call back nightmare that is the dump
// functions call back
// TODO: later goals getting this to load the bytecode first time its passed out to use then reloaded when ever needed like:
//        any time a function is pushed to lua have the lua scripts re-compile the bytecode
// 
//
struct lua_State;

namespace Brokkr
{
    class LuaScript final : public Script
    {
        inline static Logger m_fileLog{ "LuaScriptLog" };

        std::string m_filePath;
        std::unique_ptr<unsigned char> m_bytecode;
        size_t m_bytecodeSize{ };
        lua_State* m_pLuaState = nullptr;

    public:
        explicit LuaScript(ScriptAssetLoader* pCoreManager, const char* filePath);

        virtual ~LuaScript() override;

        void Load(lua_State* pLuaState);

        bool Execute(lua_State* pLuaState);

        virtual void Destroy() override;
        virtual const char* TypeName() override;

#if IN_PROGRESS

    private:
        bool Compile(lua_State* L, const char* fileData, size_t fileSize)
        static void DumpFunction(lua_State* L, const void* p, size_t size, void* ud);
        static int DumpWriter(lua_State* pLuaState, const void* data, size_t size, void* ud);
        [[nodiscard]] size_t GetBytecodeSize() const { return m_bytecodeSize; }

#endif

    };
}
