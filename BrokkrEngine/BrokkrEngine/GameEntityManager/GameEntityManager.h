#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "../Application/CoreSystems/CoreSystems.h"
#include "../EventManager/EventManager.h"

namespace Brokkr
{
    class GameEntity;

    class GameEntityManager final : public CoreSubsystem
    {
        //TODO: Redo this Object pool to manage memory better to many alloc happening
        //TODO: Make a Free list and stopping delete and new Entities
        //TODO: Make a Reset() or ClearContainer() or something to reset the Entity for reuse

        std::vector<GameEntity*> m_entities;
        std::unordered_map<int, size_t> m_entityLookup;

        EventManager::EventHandler m_renderHandler;
        EventManager::EventHandler m_updateHandler;

    public:
        explicit GameEntityManager(CoreSystems* pCoreManager): CoreSubsystem(pCoreManager){ }
        virtual ~GameEntityManager() override;

        void Init();

        // Create Object Creation / Deletion 
        ///////////////////////////////////////////
        GameEntity* GetNextEntityAvailable();

        void DeleteEntity(int entityID);

        // Object Update Components
        ///////////////////////////////////////////
        void UpdateEntities() const;

        // Object Rendering
        ///////////////////////////////////////////
        void RenderEntities([[maybe_unused]] const Event& event) const;

        // Object Serialize TODO: Flesh out this system more
        ///////////////////////////////////////////
        void SerializeEntities(const std::string& filePath) const;

        // Object Deserialize TODO: Flesh out this system more
        ///////////////////////////////////////////
        void DeserializeEntities(const std::string& filePath) const;

        // Object Get a Entity by ID 
        ///////////////////////////////////////////
        GameEntity* GetEntityById(int entityID);
        virtual void Destroy() override;

    };

}
