#include "WorldTileManager.h"
#include "../WorldTile/WorldTile.h"
#include "../WorldTile/TileComponent/TileTransformComponent/TileTransformComponent.h"


namespace Brokkr
{
    class TileTransformComponent;
}

void Brokkr::WorldTileManager::Init()
{
    m_renderHandler.first = Event::kPriorityLow; // Process render after anything last minute needed before hand
    m_renderHandler.second = [this](auto&& event) { RenderProcess(std::forward<decltype(event)>(event)); };
    m_pCoreManager->GetCoreSystem<EventManager>()->AddHandler("LoadRender", m_renderHandler);
}

Brokkr::WorldTile* Brokkr::WorldTileManager::GetAvailable()
{
    if (!m_freeList.empty())
    {
        const auto pTemp = m_freeList.back();
        
        m_freeList.pop_back();

        return &m_tiles[pTemp];
    }
    return nullptr;
}

void Brokkr::WorldTileManager::Update() const
{
    for (const auto& tile : m_tiles)
    {
        tile.Update();
    }
}

void Brokkr::WorldTileManager::RenderProcess([[maybe_unused]] const Event& event)
{
    // TODO: figure how rendering is going to work
    for (auto& tile : m_tiles)
    {
        tile.RenderProcess();
    }
}

Brokkr::WorldTile* Brokkr::WorldTileManager::GetTile(Vector2<float> position)
{
    for (auto& tile : m_tiles)
    {
        if (tile.GetComponent<TileTransformComponent>()->IsPointOnTransform(position))
        {
            return &tile;
        }
    }
    return nullptr; // no tile hit with that position
}

Brokkr::WorldTile* Brokkr::WorldTileManager::GetTile(int ID)
{
    for (auto& tile : m_tiles)
    {
        if (tile.GetId() == ID)
        {
            return &tile;
        }
    }

    return nullptr; // failed to find
}

void Brokkr::WorldTileManager::Destroy()
{
    for (auto& tile : m_tiles)
    {
        if (!tile.Reset())
        {
            // error
        }
    }
}
