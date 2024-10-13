#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "State.h"
#include "../Application/CoreSystems/CoreSystems.h"
#include "../Logging/Logger.h"

namespace Brokkr
{
    class SceneManager final : public CoreSubsystem
    {
        Logger m_logger;

        std::unordered_map<std::string, std::unique_ptr<State>> m_states;
        std::string m_pActiveStateKey;
        std::string m_pendingStateKey;

        State* m_pActiveState = nullptr;
        bool m_isReplacing = false;

    public:
        SceneManager(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
            , m_logger("SceneManagerLog")
        {
            //
        }

        virtual ~SceneManager() override;

        //TODO: move these voids to bool and return is was successful or not 
        void AddState(const std::string& stateIdentifier, std::unique_ptr<State> newState);
        void RemoveState(const std::string& stateIdentifier);
        void SetActiveState(const std::string& stateIdentifier);
        void ResetState(const std::string& stateIdentifier);

        virtual void Destroy() override;

    protected: // Only calls these two in core main game loop
        friend class Core;
        void ProcessStateChange();
        [[nodiscard]] State* GetActiveState();
    };
}

