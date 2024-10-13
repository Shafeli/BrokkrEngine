#include "SpriteComponent.h"

#include "../../GameEntity.h"
#include "../../../Graphics/Color/Color.h"
#include "../../../Graphics/RenderPipeline/RenderPipeline.h"
#include "../RandomColorComponent/RandomColorComponent.h"
#include "../TransformComponent/TransformComponent.h"


Brokkr::SpriteComponent::SpriteComponent
(
    GameEntity* pOwner
    , std::string textureName
    , const EngineSystemsRef systemRef
)
    : m_pOwner(pOwner)
    , m_systemRef(systemRef)
    , m_textureName(std::move(textureName))
{

}

bool Brokkr::SpriteComponent::Init()
{
    m_pRenderPipeline = m_systemRef->GetCoreSystem<RenderPipeline>();
    return true; // defaulting this for now not sure there is a need for this one to Init anything
}

void Brokkr::SpriteComponent::Update()
{

}

void Brokkr::SpriteComponent::Destroy()
{
    // Not sure there is anything to Destroy in this one 
}

// TODO: split this into a render component and a sprite or something 
void Brokkr::SpriteComponent::Render()
{
    if (m_textureName == "Square")
    {
        if (m_pOwner->GetComponent<RandomColorComponent>())
        {
            m_pRenderPipeline->QueueToRender
            (
                m_pOwner->GetComponent<TransformComponent>()->GetTransform()
                , m_pOwner->GetComponent<RandomColorComponent>()->GetColor()
                , 1.0f
            );
            return;
        }

        Color color;
        color.RandomColors();

        m_pRenderPipeline->QueueToRender
            (
             m_pOwner->GetComponent<TransformComponent>()->GetTransform()
             , color
             , 1.0f
            );
        return;
    }

    m_pRenderPipeline->QueueToRender(m_textureName, m_pOwner->GetComponent<TransformComponent>()->GetTransform(), static_cast<float>(m_pOwner->GetId()));
}

void Brokkr::SpriteComponent::Attach()
{
    // No events for now for this one
}

void Brokkr::SpriteComponent::Detach()
{
    // No events for now for this one
}

void Brokkr::SpriteComponent::Enable()
{
    // No events for now for this one
}

void Brokkr::SpriteComponent::Disable()
{
    // No events for now for this one
}

