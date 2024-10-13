#pragma once
#include <vector>
#include "../Application/CoreSystems/CoreSubsystem/CoreSubsystem.h"
#include "../EventManager/EventManager.h"
#include "../Primitives/Vector2.h"
#include "../WorldTile/WorldTile.h"

// TODO: Stretch Goal: Some sort of path finding
// TODO: Stretch Goal: function for taking in a vector position and returning the Tile it collides with better time complexity
constexpr int kWorldTileCount = 1920 * 1080; // TODO: make this a parameter somewhere in loading the maps
namespace Brokkr
{
    class CoreSystems;

    // TODO: Abstract this one more layer back:
    // so that this class job is to manager a collection of these Tile maps currently more then one level at a time is an issue or
    // make it so there is a container for the map files in asset manager subsystem
    // or both make this a level manager and have the potion to pre-load levels as a thread task possibly 
    class WorldTileManager final : public CoreSubsystem
    {
        // TODO: Replace this with Byte* buffer and store the free list and objects in the same buffer
        // TODO: Start abstracting the Container part of these systems to its own base class in next engine Refactor
        // TODO: add more later to this system so tile layer can be supported as well

        std::vector<size_t> m_freeList;
        std::vector<WorldTile> m_tiles;

        EventManager::EventHandler m_renderHandler;

    public:
        explicit WorldTileManager(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            m_tiles.resize(kWorldTileCount);

            for (int i = static_cast<int>(m_tiles.size()); i > 0; --i)
            {
                m_freeList.emplace_back(i - 1);
            }

            /*auto check = m_freeList.back();*/
        }

        void Init();

        // Create Object Creation / Deletion 
        ///////////////////////////////////////////
        WorldTile* GetAvailable();

        // Object Update Components logic
        ///////////////////////////////////////////
        void Update() const;

        // Object Rendering logic 
        ///////////////////////////////////////////
        void RenderProcess([[maybe_unused]] const Event& event);

        // Object Serialize TODO: Flesh out this system more
        ///////////////////////////////////////////

        // Object Deserialize TODO: Flesh out this system more
        ///////////////////////////////////////////

        WorldTile* GetTile(Vector2<float> position);
        WorldTile* GetTile(int ID);

        virtual void Destroy() override;
        virtual ~WorldTileManager() override = default;
    };
}
