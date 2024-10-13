#include "LuaScript.h"
#include <iostream>
#include <string>

#include "../../../Lua/src/lua.hpp"

Brokkr::LuaScript::LuaScript(ScriptAssetLoader* pCoreManager, const char* filePath)
    : Script(pCoreManager)
    , m_filePath(filePath)
{
    std::string testing = "Script built from: ";
    testing += m_filePath;
    m_fileLog.Log(Logger::LogLevel::kDebug, testing);
}

Brokkr::LuaScript::~LuaScript()
{
    std::string testing = "Script Destroyed: ";
    m_fileLog.Log(Logger::LogLevel::kDebug, testing);
}

void Brokkr::LuaScript::Load(lua_State* pLuaState)
{
    // Initialize the Lua state
    lua_State* luaState = pLuaState;

    // Load and execute the Lua script from a file
    const int loadResult = luaL_dofile(luaState, m_filePath.c_str());

    if (loadResult == LUA_OK)
    {
        // Print the success message
        std::string testing = "Script Loaded: " + m_filePath;
        m_fileLog.Log(Logger::LogLevel::kDebug, testing);
    }
    else
    {
        // Print the compilation error message
        const char* pErrorMessage = lua_tostring(luaState, -1);
        std::string testing = "Error loading script: " + m_filePath;
        testing += " ";
        testing += pErrorMessage;
        m_fileLog.Log(Logger::LogLevel::kError, testing);

        lua_pop(luaState, 1);
    }

    // Save the Lua state
    m_pLuaState = pLuaState;
}

bool Brokkr::LuaScript::Execute(lua_State* pLuaState)
{
    Load(pLuaState);
    return true;
}

void Brokkr::LuaScript::Destroy()
{

}

const char* Brokkr::LuaScript::TypeName()
{
    return "Lua\0";
}

#if IN_PROGRESS

Brokkr::LuaScript::LuaScript(ScriptAssetLoader* pCoreManager, const char* filePath)
    : Script(pCoreManager)
    , m_filePath(filePath)
    , m_bytecode(nullptr)
{
    std::string testing = "Script built from: ";
    testing += m_filePath;
    m_fileLog.Log(Logger::LogLevel::kDebug, testing);
}


Brokkr::LuaScript::~LuaScript()
{

    std::string testing = "Script Destroyed: ";
    m_fileLog.Log(Logger::LogLevel::kDebug, testing);
}

void Brokkr::LuaScript::Load(lua_State* pLuaState)
{
    if (!m_filePath.c_str())
    {
        const std::string error = "File Path Error : Convert Lua Script to Bytecode Failed File Path Null";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return;
    }

    m_bytecode.release();
    m_bytecodeSize = 0;
    m_pLuaState = nullptr;

    std::ifstream file(m_filePath, std::ios::binary);
    if (!file)
    {
        const std::string error = "File Loading Error";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return;
    }

    // Get the size of the file
    file.seekg(0, std::ios::end);
    m_bytecodeSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    // Allocate memory for the bytecode and read it from the file
    m_bytecode = std::make_unique<char[]>(m_bytecodeSize);
    if (m_bytecode)
    {
        file.read(m_bytecode.get(), m_bytecodeSize);
    }
    else
    {
        const std::string error = "Memory Allocation Error : After Attempting to Convert Lua Script to Bytecode";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return;
    }

    for (int i = 0; i < m_bytecodeSize; ++i) 
    {
        std::cout << m_bytecode[i];
    }

    Compile(pLuaState, m_bytecode.get(),m_bytecodeSize);


    m_pLuaState = pLuaState; // After the load is complete save the state

}

bool Brokkr::LuaScript::Execute(lua_State* pLuaState)
{
    if (m_bytecode == nullptr)
    {
        std::string error = "Lua Script Bytecode error: Not Loaded!: Attempting to load Script ";
        error += m_filePath;

        m_fileLog.Log(Logger::LogLevel::kError, error);
        Load(pLuaState);
    }

    int result = luaL_loadbuffer(pLuaState, m_bytecode.get(), m_bytecodeSize, m_filePath.c_str());
    if (result == LUA_OK)
    {
        // Execute the entire bytecode by passing -1 as the number of results to return
        result = lua_pcall(pLuaState, 0, -1, 0);
        if (result != LUA_OK)
        {
            const std::string errorMsg = lua_tostring(pLuaState, -1);
            const std::string error = "Lua execution error: " + errorMsg;
            m_fileLog.Log(Logger::LogLevel::kError, error);
            return false;
        }
    }
    else
    {
        const std::string errorMsg = lua_tostring(pLuaState, -1);
        const std::string error = "Lua loading error: " + errorMsg;
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return false;
    }
    return true;
}

bool Brokkr::LuaScript::Compile(lua_State* L, const char* fileData, size_t fileSize)
{
    // Load the Lua script from a buffer
    const int loadResult = luaL_loadfile(L, m_filePath.c_str());

    if (loadResult != LUA_OK)
    {
        // If the script failed to load, log the error message and return false

        std::string errorMessage = lua_tostring(L, -1);
        std::string error = "Lua Compile Error: Failed to load script " + m_filePath;
        error += " " + errorMessage;

        m_fileLog.Log(Logger::LogLevel::kError, error);
        lua_pop(L, 1);
        return false;
    }

    // Dump the compiled bytecode into a buffer
    m_bytecodeSize = lua_dump(L, DumpWriter, m_bytecode.get(), true);

    // Allocate a new buffer to hold the compiled bytecode
    if (!m_bytecode)
    {

        std::string error = "Lua Compile Error: Failed to allocate memory for compiled script " + m_filePath;
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return false;
    }

    return true;
}



/*bool Brokkr::LuaScript::Compile(lua_State* pLuaState)
{
    // Create a new buffer to hold the compiled bytecode
    char* bytecode = nullptr;
    size_t size = 0;

    // Compile the script into bytecode
    luaL_loadbuffer(pLuaState, m_bytecode.get(), size, m_filePath.c_str());
    lua_dump(pLuaState, DumpWriter, &bytecode, true);

    // Replace the old bytecode with the compiled bytecode
    m_bytecode.release();
    m_bytecode.reset(bytecode + sizeof(char));
    m_bytecodeSize = size;

    return true;
}*/

int Brokkr::LuaScript::DumpWriter(lua_State* pLuaState, const void* data, size_t size, void* ud)
{
    char** bytecode = static_cast<char**>(ud);
    *bytecode = static_cast<char*>(std::realloc(*bytecode, size));
    std::memcpy(*bytecode, data, size);

    return 0;
}

void Brokkr::LuaScript::Destroy()
{
    m_bytecode.release();
}

const char* Brokkr::LuaScript::TypeName()
{
    return "Lua\0";
}


/*void Brokkr::LuaScript::LoadFromBinary(lua_State* pLuaState)
{
    std::ifstream file(m_filePath, std::ios::binary);
    if (!file)
    {
        const std::string error = "File Loading Error";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return;
    }

    // Determine file size
    const auto fileSize = static_cast<std::size_t>(file.seekg(0, std::ios::end).tellg());
    file.seekg(0, std::ios::beg);

    // Allocate buffer for file data
    std::vector<char> fileData(fileSize);
    if (static_cast<size_t>(file.read(fileData.data(), fileSize).gcount()) != fileSize)
    {
        const std::string error = "Failed to read file data";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return;
    }

    // Convert Lua script to bytecode
    const auto result = luaL_loadbuffer(pLuaState, fileData.data(), fileSize, m_filePath);

    if (result != LUA_OK)
    {
        const std::string errorMsg = lua_tostring(pLuaState, -1);
        const std::string error = "Lua loading error: " + errorMsg;
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }
    else
    {
        // Save the bytecode in m_bytecode
        m_bytecodeSize = 0;
        const char* bytecode = lua_tolstring(pLuaState, -1, &m_bytecodeSize);
        m_bytecode = std::make_unique<char[]>(m_bytecodeSize);
        if (m_bytecode)
        {
            std::memcpy(m_bytecode.get(), bytecode, m_bytecodeSize);
        }
        else
        {
            const std::string error = "Memory Allocation Error : Lua Script to Bytecode";
            m_fileLog.Log(Logger::LogLevel::kError, error);
            return;
        }
        m_pLuaState = pLuaState; // After the load is complete save the state
    }
}*/

#endif
