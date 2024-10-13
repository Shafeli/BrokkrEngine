#include "GameEntity.h"

void Brokkr::GameEntity::Init() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Init();
    }
}

Brokkr::GameEntity::~GameEntity()
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Detach();
        m_pComponents[i]->Destroy();
    }
}

void Brokkr::GameEntity::Update() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Update();
    }
}

void Brokkr::GameEntity::Render() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Render();
    }
}

void Brokkr::GameEntity::Serialize([[maybe_unused]] const std::string& filePath) const
{
    /*for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Serialize(filePath);
    }*/
}

void Brokkr::GameEntity::Deserialize([[maybe_unused]] const std::string& filePath) const
{
    /*for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Deserialize(filePath);
    }*/
}
