#pragma once

#include "../../Application/CoreSystems/CoreSystems.h"
#include "../../Graphics/Color/Color.h"
#include "../../Logging/Logger.h"

struct SDL_Window;

namespace Brokkr
{
    class Renderer;

    class WindowHandle final : public CoreSubsystem
    {

        Logger m_windowLog;
        SDL_Window* m_pWindow = nullptr;
        Color m_background;

    public:
        explicit WindowHandle(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
            , m_windowLog("WindowLogs")
        {
            //
        }

        void SetBackgroundColor(int red, int green, int blue, int opacity);
        bool InitWindow(const char* gameTitle, int screenWidth, int screenHeight);

        Color* GetBackgroundColor() { return &m_background; }

        [[nodiscard]] SDL_Window* GetWindow() const { return m_pWindow; }

        virtual void Destroy() override;
    };

}

