#pragma once
#include <memory>
#include <vector>

#include "../Application/CoreSystems/CoreSubsystem/CoreSubsystem.h"
#include "../Logging/Logger.h"
#include "ScriptSystem/ScriptSystem.h"

namespace Brokkr
{
    class CoreSystems;

    class ScriptSystemManager final : public CoreSubsystem
    {
        inline static Logger m_fileLog{ "ScriptSystemManagerLog" };
        std::vector<std::unique_ptr<ScriptSystem>> m_pScriptSystems{ };

    public:
        explicit ScriptSystemManager(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            //
        }

        void Init(); // Currently a place to add Built in Scripting plugins

        //TODO: should still allow runtime adding of plug-ins
        template <typename System, typename... Args>
        void RuntimeInit(Args&&... args)
        {
            // Initialize each ScriptSystem with the given arguments
            for (auto& system : m_pScriptSystems)
            {
                if (System* targetSystem = dynamic_cast<System*>(system.get()))
                {
                    targetSystem->Init(std::forward<Args>(args)...);
                }
            }

        }

        //TODO: Getter for Subsystem Plug-ins
        template <typename System>
        System* GetScriptSystem()
        {
            // Iterate through all components in the vector
            for (auto& component : m_pScriptSystems)
            {
                // If the cast is successful
                if (System* target = dynamic_cast<System*>(component.get()))
                {
                    return target;
                }
            }
            return nullptr; // If no system of type is found
        }

        //TODO: Adding Script Subsystems for LUA and possible plug-ins for supporting others later
        template <typename System, typename ... Args>
        System* AddScriptSystem(Args&&... args)
        {
            // Create a instance of the system
            std::unique_ptr<System> newComponent = std::make_unique<System>(this, m_pCoreManager, std::forward<Args>(args)...);

            System* result = newComponent.get(); // Get a raw pointer

            m_pScriptSystems.emplace_back(std::move(newComponent)); // Add the system to the vector 
            return result; // Return a pointer
        }

        virtual void Destroy() override;
    };
}
