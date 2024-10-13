#include "GameEntityManager.h"
#include "../GameEntity/GameEntity.h"


Brokkr::GameEntityManager::~GameEntityManager()
{
    for (const auto& pEntity: m_entities)
    {
        delete pEntity;
    }
}

void Brokkr::GameEntityManager::Init()
{
    m_renderHandler.first = Event::kPriorityLow; // Process render after anything last minute needed before hand
    m_renderHandler.second = [this](auto&& event) { RenderEntities(std::forward<decltype(event)>(event)); };
    m_pCoreManager->GetCoreSystem<EventManager>()->AddHandler("LoadRender", m_renderHandler);
}

Brokkr::GameEntity* Brokkr::GameEntityManager::GetNextEntityAvailable()
{
    auto pEntity = new GameEntity;

    m_entities.push_back(pEntity);
    m_entityLookup[pEntity->GetId()] = m_entities.size() - 1;

    return pEntity;
}

void Brokkr::GameEntityManager::DeleteEntity(int entityID)
{
    const auto it = m_entityLookup.find(entityID);
    if (it == m_entityLookup.end())
    {
        return;
    }

    const size_t index = it->second;

    // Delete the entity being removed
    delete m_entities[index];

    // Swap the entity to be deleted with the last entity in the vector
    m_entities[index] = std::move(m_entities.back());

    // Update the lookup map
    m_entityLookup[m_entities[index]->GetId()] = index;

    // Remove the last entity in the vector
    m_entities.pop_back();

    // Remove the entity from the lookup map
    m_entityLookup.erase(it);
}

void Brokkr::GameEntityManager::UpdateEntities() const
{
    for (auto& pEntity : m_entities)
    {
        pEntity->Update();
    }
}

void Brokkr::GameEntityManager::RenderEntities([[maybe_unused]] const Event& event) const
{
    for (auto& pEntity : m_entities)
    {
        pEntity->Render();
    }
}

void Brokkr::GameEntityManager::SerializeEntities([[maybe_unused]] const std::string& filePath) const
{
    // TODO: Implement entity serialization
}

void Brokkr::GameEntityManager::DeserializeEntities([[maybe_unused]] const std::string& filePath) const
{
    // TODO: Implement entity serialization
}

Brokkr::GameEntity* Brokkr::GameEntityManager::GetEntityById(int entityID)
{
    auto it = m_entityLookup.find(entityID);
    if (it == m_entityLookup.end())
    {
        return nullptr;
    }

    return m_entities[it->second];
}

void Brokkr::GameEntityManager::Destroy()
{
    //TODO:
}

