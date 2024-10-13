#pragma once

#include "../Component.h"
#include "../../BrokkrEngine/InputManager/InputManager.h"
#include "../../BrokkrEngine/Logging/Logger.h"

namespace Brokkr
{
    class InputComponent;
    class CoreSystems;
    class GameEntity;

    class BrokkrMoveController final : public Component
    {
        static inline constexpr const char kNoInput[] = "NoInput";

        inline static Logger m_fileLog{ "DefaultControllerLog" };

        GameEntity* m_pOwner = nullptr;
        CoreSystems* m_pCoreSystems = nullptr;

        InputManager::KeyCode m_upKeyCode;
        InputManager::KeyCode m_downKeyCode;
        InputManager::KeyCode m_leftKeyCode;
        InputManager::KeyCode m_rightKeyCode;

        InputComponent* m_inputComponentUp = nullptr;
        InputComponent* m_inputComponentDown = nullptr;
        InputComponent* m_inputComponentLeft = nullptr;
        InputComponent* m_inputComponentRight = nullptr;

    public:
        explicit BrokkrMoveController
        (
            GameEntity* pOwner
            , CoreSystems* pCoreSystems
            , const char* up                // == key for input
            , const char* left              // == key for input  
            , const char* right             // == key for input
            , const char* down              // == key for input
        );

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Attach() override;
        virtual void Detach() override;
        virtual void Enable() override;
        virtual void Disable() override;

    };
}
