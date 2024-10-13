#pragma once
#include <chrono>
#include <string>
#include "../../Logging/Logger.h"

//TODO: Add decompressed binary to Texture support

struct SDL_Texture;

namespace Brokkr
{
    class Renderer;

    class Texture
    {
        static inline Logger s_textureLog{ "Texture" };

        std::string m_texturePath;
        SDL_Texture* m_pTexture = nullptr;
        Renderer* m_pRenderer = nullptr;

    public:

        Texture(std::string& texturePath, Renderer* pRenderer);
        Texture(const void* pData, size_t size, Renderer* pRenderer);
        ~Texture();

        SDL_Texture* LoadTexture();
        bool DestroyTexture();
    };

}
