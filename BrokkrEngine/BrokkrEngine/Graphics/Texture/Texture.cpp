#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../../AssetManager/AssetManager.h"
#include "../../Graphics/Renderer/Renderer.h"

Brokkr::Texture::Texture(std::string& texturePath, Renderer* pRenderer)
    :m_texturePath(texturePath)
    ,m_pRenderer(pRenderer)
{
    //
}

Brokkr::Texture::Texture(const void* pData, size_t size, Renderer* pRenderer)
    :m_texturePath("BinaryBuilt" + size)
    ,m_pRenderer(pRenderer)
{
    SDL_RWops* pRWops = SDL_RWFromConstMem(pData, static_cast<int>(size));

    // SDL_Surface object
    SDL_Surface* pSurface = IMG_Load_RW(pRWops, 0);

    // Create a surface
    m_pTexture = SDL_CreateTextureFromSurface(pRenderer->GetRenderer(), pSurface);

    // Free the surface and RWops
    SDL_FreeSurface(pSurface);
    SDL_RWclose(pRWops);
}

Brokkr::Texture::~Texture()
{
    DestroyTexture();
}

SDL_Texture* Brokkr::Texture::LoadTexture()
{
    if (m_pTexture)
        return m_pTexture;

    if (m_pTexture == nullptr)    // If texture is not already loaded
    {
        SDL_Surface* pSurface = IMG_Load(m_texturePath.c_str()); // Load the image
        if (pSurface == nullptr)
        {
            const std::string error = "Could not load surface : " + m_texturePath + ", error message: " + IMG_GetError();
            s_textureLog.Log(Logger::LogLevel::kError, error);
            return nullptr;
        }

        // Create texture from surface
        SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pRenderer->GetRenderer(), pSurface);
        if (pTexture == nullptr)
        {
            const std::string error = "Could not load texture : " + m_texturePath + " " + SDL_GetError();
            s_textureLog.Log(Logger::LogLevel::kError, error);
            SDL_FreeSurface(pSurface);
            return nullptr;
        }

        m_pTexture = pTexture;    // Add texture to map

        SDL_FreeSurface(pSurface);  // Free the surface
    }

    return m_pTexture;
}

bool Brokkr::Texture::DestroyTexture()
{
    if (m_pTexture != nullptr)
    {
        SDL_DestroyTexture(m_pTexture);
        return true;
    }

    return false; 
}
