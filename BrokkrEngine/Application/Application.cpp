
#include "Application.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

Brokkr::Application::Application()
    :m_appLogger("AppLogs")
{
}

void Brokkr::Application::Run()
{
    m_engineCore.Run();
}

bool Brokkr::Application::Init(const char* pGameTitle, int screenWidth, int screenHeight)
{
    m_appLogger.Log(Logger::LogLevel::kInfo, "Welcome to Brokkr Game Engine");
    SDL_Init(SDL_INIT_EVERYTHING);

    // attempt to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        m_appLogger.Log(Logger::LogLevel::kError, "SDL could not initialize! Check SDL_Error");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        m_appLogger.Log(Logger::LogLevel::kError, "SDL_IMG could not initialize! Check SDL_Error");
        return false;
    }

    [[maybe_unused]] int imgFlags = IMG_INIT_PNG;

    m_appLogger.Log(Logger::LogLevel::kInfo, "SDL initialized");

    if (!m_engineCore.Init(pGameTitle, screenWidth, screenHeight))
    {
        m_appLogger.Log(Logger::LogLevel::kError, "Core failed to initialize");
        return false;
    }

#ifdef _DEBUG 

    m_engineCore.RunTests();

#endif


    return true;
}

int Brokkr::Application::Clean()
{
    return m_engineCore.CleanUp();
}

Brokkr::EngineSystemsRef Brokkr::Application::GetEngineSystems() const
{
    return m_engineCore.GetEngineSystems();
}
