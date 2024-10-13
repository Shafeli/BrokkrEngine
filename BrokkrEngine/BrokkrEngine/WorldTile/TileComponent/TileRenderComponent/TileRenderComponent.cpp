#include "TileRenderComponent.h"

#define STRESS_RENDER_TEST 0 

#include "../../WorldTile.h"

#if  STRESS_RENDER_TEST
#include "../../../Graphics/Color/Color.h"
#endif

#include "../../../Graphics/RenderPipeline/RenderPipeline.h"
#include "../TileTransformComponent/TileTransformComponent.h"



void Brokkr::TileRenderComponent::SetTexture(const std::string& textureName)
{
    m_textureName = textureName;
}

void Brokkr::TileRenderComponent::RenderProcess()
{
    if (m_textureName != "Square")
    {
        const auto pTemp = m_pTransform->GetTransform();
        Rect<int> pTransform;

        pTransform.SetX(static_cast<int>(pTemp.GetX()));
        pTransform.SetY(static_cast<int>(pTemp.GetY()));
        pTransform.SetHeight(static_cast<int>(pTemp.GetHeight()));
        pTransform.SetWidth(static_cast<int>(pTemp.GetWidth()));

        m_pRenderPipeline->QueueToRender
        (
            m_textureName
            , pTransform
            , 1.0f
        );
#if  STRESS_RENDER_TEST
        return;
#endif
    }
#if  STRESS_RENDER_TEST
    Color color;
    color.m_blue = 55;
    color.m_red = 55;

    if (pOwner_DEBUG == nullptr)
    {
        m_log.Log(Logger::LogLevel::kDebug, "Tile Owner is null!");
    }

    const auto pTemp = m_pTransform->GetTransform();
    Rect<int> pTransform;

    pTransform.SetX(static_cast<int>(pTemp.GetX()));
    pTransform.SetY(static_cast<int>(pTemp.GetY()));
    pTransform.SetHeight(static_cast<int>(pTemp.GetHeight()));
    pTransform.SetWidth(static_cast<int>(pTemp.GetWidth()));

    m_pRenderPipeline->QueueToRender
    (
        pTransform
        , color
        , 1.0f
    );
#endif
}

bool Brokkr::TileRenderComponent::Init()
{
    if (!pOwner_DEBUG)
    {
        m_log.Log(Logger::LogLevel::kDebug, "Tile Owner is null!");
    }

    m_pTransform = pOwner_DEBUG->GetComponent<TileTransformComponent>();
    m_pRenderPipeline = m_pCoreSystems->GetCoreSystem<RenderPipeline>();

    return true; // defaulting this for now not sure there is a need for this one to Init anything
}

void Brokkr::TileRenderComponent::Update()
{
}

void Brokkr::TileRenderComponent::Destroy()
{
}
