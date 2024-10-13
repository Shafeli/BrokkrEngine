#include "TileTransformComponent.h"

#include "../../WorldTile.h"

Brokkr::TileTransformComponent::TileTransformComponent(WorldTile* pOwner, CoreSystems* pCoreSystems)
    :TileComponent(pOwner, pCoreSystems)
{
    //
}

bool Brokkr::TileTransformComponent::IsPointOnTransform(const Vector2<float>& position) const
{
    return m_transform.Contains(position);
}

void Brokkr::TileTransformComponent::SetPosition(const Vector2<float>& position)
{
    m_transform.MoveTo(position);
}

void Brokkr::TileTransformComponent::SetSize(const Vector2<float>& size)
{
    m_transform.Resize(size.x, size.y);
}

bool Brokkr::TileTransformComponent::Init()
{
    return true; //default
}

void Brokkr::TileTransformComponent::Update()
{
    // not needed for this component for now
}

void Brokkr::TileTransformComponent::Destroy()
{
    // not needed for this component for now
}


