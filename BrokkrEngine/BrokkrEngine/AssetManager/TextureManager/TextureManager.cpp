#include "TextureManager.h"
#include <SDL_image.h>

#include "../../Graphics/Renderer/Renderer.h"
#include "../../Graphics/Texture/Texture.h"
#include "../../Graphics/WindowHandle/WindowHandle.h"

Brokkr::TextureManager::TextureManager(AssetManager* assetManager)
    : AssetSubsystem(assetManager)
    , m_textureLog("TextureManagerLog")
{
}

Brokkr::TextureManager::~TextureManager()
{
    Destroy();
}

void Brokkr::TextureManager::Destroy()
{
    for (const auto& [key, pTexture] : m_pImageTexture)
    {
        if (pTexture == nullptr)
            continue;

        pTexture->DestroyTexture();
        delete pTexture;
    }
}

void Brokkr::TextureManager::Unload(const std::string& textureName)
{
    if (IsLoaded(textureName))
    {
        //frees surface 
        m_pImageTexture[textureName]->DestroyTexture();

        // delete texture object
        delete m_pImageTexture[textureName];

        //Remove hash
        m_pImageTexture.erase(textureName);
    }
}

bool Brokkr::TextureManager::IsLoaded(const std::string& textureName) const
{
    //Runs over map
    for (const auto& [key, pTexture] : m_pImageTexture)
    {
        //If the key is a match to another
        if (key == textureName)
        {
            return true;
        }
    }
    return false;//No error
}

Brokkr::Texture* Brokkr::TextureManager::GetTexture(const std::string& textureName)
{
    return m_pImageTexture[textureName];
}

void Brokkr::TextureManager::LoadTexture(const std::string& textureName, const char* fileName)
{
    if (!IsLoaded(textureName))    // If texture is not already loaded
    {
        std::string basicString = fileName;
        m_pImageTexture[textureName] = new Texture(basicString, m_pRenderer);    // Add texture to map
    }
}

void Brokkr::TextureManager::Init(WindowHandle* window, Renderer* render)
{
    m_pWindow = window;
    m_pRenderer = render;

    if (m_pWindow == nullptr)
    {
        const std::string error = "Could not set window pointer";
        m_textureLog.Log(Logger::LogLevel::kError, error);
    }
    if (m_pRenderer == nullptr)
    {
        const std::string error = "Could not set renderer pointer";
        m_textureLog.Log(Logger::LogLevel::kError, error);
    }
}

SDL_Texture* Brokkr::TextureManager::GetTexture(const std::string& textureName) const
{
    return this->m_pImageTexture.at(textureName)->LoadTexture();
}
