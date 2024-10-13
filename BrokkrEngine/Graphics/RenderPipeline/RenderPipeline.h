#pragma once
#include <mutex>
#include <queue>
#include <string>
#include "../../Application/CoreSystems/CoreSystems.h"
#include "../../EventManager/EventManager.h"
#include "../../Graphics/Color/Color.h"
#include "../../Primitives/Rect.h"
// TODO: Maybe use event manager system to alert anything that want to render to pass their data or something?
// TODO: Design a way to use events for getting data to the render Pipe line

struct SDL_Texture;

namespace Brokkr
{
    class WindowHandle;
    class AssetManager;
    class Renderer;
    class Event;

    class RenderPipeline final : public CoreSubsystem
    {
        struct RenderData
        {
            float m_depth = 0.0f;
            SDL_Texture* m_pTexture = nullptr;
            Rect<int> m_transform;
            Color m_color{};

            void Render(Renderer* pRenderer) const;

            bool operator<(const RenderData& other) const
            {
                // Sort based on depth, with smaller depths being drawn first
                return m_depth < other.m_depth;

                // TODO: Idea: for Getting Y values to sort depth
                //  So y values can be use for depth sorting 0 first drawn then 1 2 3 4.... so on
                //  if you have tiles with y-position values ranging from 0 to 100, with 0 being the
                //  top of the screen and 100 being the bottom, then you could set the depth value of
                //  a tile to be its y-position divided by 100. This would give you a depth value
                //  between 0 and 1, with smaller depth values indicating that the tile should be
                //  rendered earlier.

                //  The value of 100 represents the maximum y-position of the tiles in the vertical
                //  direction. So for example, if the game window has a height of 1000 pixels and
                //  tiles are 100 pixels in height, then the maximum number of rows of tiles
                //  you can display is 10 (since 10 * 100 = 1000). In this case, setting the depth
                //  value of a tile to be its y-position divided by 100 would give you a depth value
                //  between 0 and 1, with smaller depth values indicating that the tile should be
                //  rendered earlier.

                // Edge Case: 
                // But y is top left so maybe sort by the bottom of the bounding box 
            }
        };

        Renderer* m_pRenderer = nullptr;
        WindowHandle* m_window = nullptr;
        AssetManager* m_pAssetManager = nullptr;
        EventManager::EventHandler m_handler;

        // Queue of textures that are to be drawn to the screen
        std::priority_queue<RenderData> m_pRenderQueue;
        std::priority_queue<RenderData> m_pLoadingQueue;

        // Define a mutex for synchronization
        std::mutex renderQueueMutex;

    public:
        explicit RenderPipeline(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            //
        }

        void Init();

        void ProcessRenderQueue();
        void ProcessFrame([[maybe_unused]] const Brokkr::Event& event); 

        void QueueToRender(const std::string& textureName, Rect<int> transform, float depth);
        void QueueToRender(Rect<int> transform, Color color, float depth );

        void QueueToRender(const std::string& textureName, Rect<float> transform, float depth);
        void QueueToRender(Rect<float> transform, Color color, float depth);

        virtual void Destroy() override;
    };

}


