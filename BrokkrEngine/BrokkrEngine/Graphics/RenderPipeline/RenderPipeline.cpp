#include "RenderPipeline.h"

#include <iostream>

#include "../../AssetManager/AssetManager.h"
#include "../../AssetManager/TextureManager/TextureManager.h"
#include "../../Graphics/Renderer/Renderer.h"
#include "../Texture/Texture.h"
#include "../WindowHandle/WindowHandle.h"

namespace Brokkr
{
    class TextureManager;
}

void Brokkr::RenderPipeline::RenderData::Render(Renderer* pRenderer) const
{
    if (m_pTexture != nullptr)
    {
        ///////////////////////////////////////////////////
        // rendering with texture*
        pRenderer->RenderCopy
        (
            m_pTexture
            , m_transform.GetX()
            , m_transform.GetY()
            , m_transform.GetWidth()
            , m_transform.GetHeight()
        );
        ///////////////////////////////////////////////////
    }
    else
    {
        ///////////////////////////////////////////////////
        // rendering with Rect TODO: redesign the system to add shapes back to be supported the right way
        pRenderer->RenderSquare
        (
            m_transform.GetX()
            , m_transform.GetY()
            , m_transform.GetHeight()
            , m_transform.GetWidth()
            , m_color.m_red
            , m_color.m_green
            , m_color.m_blue
            , m_color.m_opacity
        );
        /*m_pRenderer->SetRenderDrawColor(m_window->GetBackgroundColor());*/
        ///////////////////////////////////////////////////
    }
}

void Brokkr::RenderPipeline::Init()
{
    m_pRenderer = m_pCoreManager->GetCoreSystem<Renderer>();
    m_window = m_pCoreManager->GetCoreSystem<WindowHandle>();
    m_pAssetManager = m_pCoreManager->GetCoreSystem<AssetManager>();

    //TODO: add to logging maybe in base class for subsystems and a to_string requirement so init() logging can be automated a bit
    if (!m_pAssetManager)
    {
        std::cout << "Error: AssetManager not initialized\n";
        return;
    }

    m_handler.first = Event::kPriorityMin;
    m_handler.second = [this](auto&& event) { ProcessFrame(std::forward<decltype(event)>(event)); };

    m_pCoreManager->GetCoreSystem<EventManager>()->AddHandler("FrameComplete", m_handler);

    g_rng.SeedFromTime(); //TODO: add to core systems of something not hacked in 
}


void Brokkr::RenderPipeline::ProcessRenderQueue()
{
    m_pRenderer->ClearRenderer();
    m_pRenderer->SetRenderDrawColor(m_window->GetBackgroundColor());

    while (!m_pRenderQueue.empty())
    {
        m_pRenderQueue.top().Render(m_pRenderer);
        m_pRenderQueue.pop();
        m_pRenderer->SetRenderDrawColor(m_window->GetBackgroundColor());
    }

    m_pRenderer->Render();
}

void Brokkr::RenderPipeline::ProcessFrame([[maybe_unused]] const Event& event)
{
    std::swap(m_pLoadingQueue, m_pRenderQueue);
}

void Brokkr::RenderPipeline::QueueToRender(const std::string& textureName, Rect<int> transform, float depth)
{

    //TODO: Add to logging this helped in debugging an issue already
    if (!m_pAssetManager)
    {
        std::cout << "Error: AssetManager not initialized\n";
        return;
    }

    const auto pTextureManager = m_pAssetManager->GetAssetSystem<TextureManager>();

    m_pLoadingQueue.push({ depth, pTextureManager->GetTexture(textureName)->LoadTexture(), transform });
}


void Brokkr::RenderPipeline::QueueToRender(Rect<int> transform, Color color, float depth)
{
    m_pLoadingQueue.push({depth, nullptr, transform, color });
}

void Brokkr::RenderPipeline::QueueToRender(const std::string& textureName, Rect<float> transform, float depth)
{
    //TODO: Add to logging this helped in debugging an issue already
    if (!m_pAssetManager)
    {
        std::cout << "Error: AssetManager not initialized\n";
        return;
    }
    const auto pTextureManager = m_pAssetManager->GetAssetSystem<TextureManager>();

    Rect<int>tempRect;
    tempRect.SetX(static_cast<int>(transform.GetX()));
    tempRect.SetY(static_cast<int>(transform.GetY()));
    tempRect.SetWidth(static_cast<int>(transform.GetWidth()));
    tempRect.SetHeight(static_cast<int>(transform.GetHeight()));

    m_pLoadingQueue.push({ depth, pTextureManager->GetTexture(textureName)->LoadTexture(), tempRect });
}

void Brokkr::RenderPipeline::QueueToRender(Rect<float> transform, Color color, float depth)
{
    Rect<int>tempRect;
    tempRect.SetX(static_cast<int>(transform.GetX()));
    tempRect.SetY(static_cast<int>(transform.GetY()));
    tempRect.SetWidth(static_cast<int>(transform.GetWidth()));
    tempRect.SetHeight(static_cast<int>(transform.GetHeight()));

    m_pLoadingQueue.push({ depth, nullptr, tempRect, color });
}

void Brokkr::RenderPipeline::Destroy()
{
    //
}

