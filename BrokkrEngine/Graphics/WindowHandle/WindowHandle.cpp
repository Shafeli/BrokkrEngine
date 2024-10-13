#include "WindowHandle.h"
#include <SDL_image.h>
#include <SDL_video.h>


void Brokkr::WindowHandle::SetBackgroundColor(int red, int green, int blue, int opacity)
{
    m_background.m_red = red;
    m_background.m_green = green;
    m_background.m_blue = blue;
    m_background.m_opacity = opacity;
}

bool Brokkr::WindowHandle::InitWindow(const char* gameTitle, int screenWidth, int screenHeight)
{
    m_pWindow = SDL_CreateWindow
    (
        gameTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (!m_pWindow)
    {
        m_windowLog.Log(Logger::LogLevel::kError, "Window was not created.");
        return false;
    }

    return true;
}

void Brokkr::WindowHandle::Destroy()
{
    SDL_DestroyWindow(m_pWindow);

    // Quit SDL_image
    IMG_Quit();
    SDL_Quit();
}
