#pragma once
#include <memory>
#include <vector>
#include "../../Logging/Logger.h"
#include "CoreSubsystem/CoreSubsystem.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Core Systems:
// TODO: Core Systems Tagged for Re-design
// TODO: Docs and how to use
// TODO: Add init() and removal sorting for shut downs & start ups
/////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Brokkr
{
    class CoreSystems
    {
        inline static Logger m_fileLog{ "CoreSystemLog" };
        std::vector<std::unique_ptr<CoreSubsystem>> m_pCoreSubsystems;

    public:

        bool Init(const char* pGameTitle, int screenWidth, int screenHeight); // For when system is building

        // For init() of runtime systems ...TODO: Tagged for re-design
        template <typename System, typename... Args>
        void RuntimeInit(Args&&... args)
        {
            // Initialize each with the given arguments
            for (auto& system : m_pCoreSubsystems)
            {
                if (System* targetSystem = dynamic_cast<System*>(system.get()))
                {
                    targetSystem->Init(std::forward<Args>(args)...);
                }
            }
        }

        template <typename CoreSubsystem>
        CoreSubsystem* GetCoreSystem()
        {
            // Iterate through all components in the vector
            for (auto& subsystem : m_pCoreSubsystems)
            {
                // If the cast is successful
                if (CoreSubsystem* target = dynamic_cast<CoreSubsystem*>(subsystem.get()))
                {
                    target->AddRef(); // TODO: remove ref counting in core systems more then likely will not be removing core systems at run time
                    return target;
                }
            }

            const std::string error = "Error Core System Failed to : Get a Subsystem!";
            m_fileLog.Log(Logger::LogLevel::kError, error);
            return nullptr; // If no system of type is found
        }

        template <typename CoreSubsystem, typename ... Args>
        CoreSubsystem* AddCoreSystem(Args&&... args)
        {
            std::unique_ptr<CoreSubsystem> newCoreSubsystem = std::make_unique<CoreSubsystem>(this, std::forward<Args>(args)...);
            newCoreSubsystem->AddRef(); // increment the reference count

            CoreSubsystem* result = newCoreSubsystem.get(); // Get a raw pointer to the component
            m_pCoreSubsystems.emplace_back(std::move(newCoreSubsystem)); // Add the system to the vector

            if (!result)
            {
                const std::string error = "Error Core System Failed to : Construct a Subsystem!";
                m_fileLog.Log(Logger::LogLevel::kError, error);
            }
            return result; // Return a pointer
        }

        // TODO: remove ref counting in core systems more then likely will not be removing core systems at run time
        //TODO: most liked for removal so far...
        template<typename CoreSubsystem>
        void RemoveCoreSystem(CoreSubsystem* System)
        {
            // Iterate through all components in the vector
            for (size_t i = 0; i < m_pCoreSubsystems.size(); ++i)
            {
                // If the cast is successful
                if (CoreSubsystem* target = dynamic_cast<CoreSubsystem*>(m_pCoreSubsystems[i].get()))
                {
                    if (target == System) // If the found type is the pointer we already have then
                    {
                        const std::string error = "Core System : Removed a Subsystem!";
                        m_fileLog.Log(Logger::LogLevel::kInfo, error);

                        const int remainingRefs = target->Release(); // relase returns a number == the remaining Refs to this system

                        if (remainingRefs <= 0) // If no more ref remove the ptr
                        {
                            target->Destroy();
                            // Swap and pop the element at index i
                            std::swap(m_pCoreSubsystems[i], m_pCoreSubsystems.back());
                            m_pCoreSubsystems.pop_back();
                            break; // Stop searching
                        }
                    }
                }
            }
        }
    };
}