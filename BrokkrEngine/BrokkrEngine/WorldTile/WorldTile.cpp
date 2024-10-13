#include "WorldTile.h"

void Brokkr::WorldTile::Init()
{
    for (const auto& pComponent : m_pComponents)
    {
        pComponent->Init();
    }

    m_renderComponent = GetComponent<TileRenderComponent>();
}

bool Brokkr::WorldTile::Reset()
{
    for (const auto& pComponent : m_pComponents)
        pComponent->Destroy();

    m_pComponents.clear();

    if (!m_pComponents.empty())
    {
        return false;
    }

    return true; // no error 
}

Brokkr::WorldTile::~WorldTile()
{
    for (const auto& pComponent : m_pComponents)
    {
        pComponent->Update();
    }
}

void Brokkr::WorldTile::Update() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Update();
    }
}

void Brokkr::WorldTile::RenderProcess()
{
    if (m_renderComponent)
        m_renderComponent->RenderProcess();
}

template <typename ComponentType>
ComponentType* Brokkr::WorldTile::AddComponentFromString(const std::string& str, std::vector<const std::string&> strDataArray)
{
    // Convert the string to the desired component type
    ComponentType component = ComponentType::FromString(str);

    // Create a instance of the component type 
    std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(this, component, strDataArray);

    // Get a raw pointer to the component
    ComponentType* result = newComponent.get();

    // Add the component to the vector
    m_pComponents.emplace_back(std::move(newComponent));

    // Return a pointer
    return result;
}
