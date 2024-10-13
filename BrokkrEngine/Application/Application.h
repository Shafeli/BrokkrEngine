#pragma once
#include "Core.h"
#include "../Logging/Logger.h"

namespace Brokkr
{
    class Application
    {
        Logger m_appLogger;
        Core m_engineCore;

    public:
        Application();

        void Run(); 

        bool Init(const char* pGameTitle, int screenWidth, int screenHeight); // Init calls for core systems
        int Clean(); // anything that needs to be cleaned or deleted for core systems

        [[nodiscard]] EngineSystemsRef GetEngineSystems() const;
    };
    
}

