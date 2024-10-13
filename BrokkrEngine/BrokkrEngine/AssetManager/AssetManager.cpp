#include "AssetManager.h"

#include "../Application/CoreSystems/CoreSystems.h"
#include "../Graphics/WindowHandle/WindowHandle.h"
#include "../Graphics/Renderer/Renderer.h"
#include "ArchiveManager/ArchiveManager.h"
#include "TextureManager/TextureManager.h"
#include "XMLManager/XMLManager.h"
#include "ScriptManager/ScriptAssetLoader.h"

void Brokkr::AssetManager::Init()
{

    const auto pWindow = m_pCoreManager->GetCoreSystem<WindowHandle>();
    const auto pRender = m_pCoreManager->GetCoreSystem<Renderer>();

    const auto pArchiveManager = AddAssetSystem<ArchiveManager>();
    if (!pArchiveManager)
    {
        const std::string error = "Error Asset System Failed : ArchiveManager did not Construct!";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }

    const auto pTextureManager = AddAssetSystem<TextureManager>();
    if (!pTextureManager)
    {
        const std::string error = "Error Asset System Failed : TextureManager did not Construct!";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }

    const auto pXMLManager = AddAssetSystem<XMLManager>();
    if (!pXMLManager)
    {
        const std::string error = "Error Asset System Failed : XMLManager did not Construct!";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }

    const auto pScriptAssetManager = AddAssetSystem<ScriptAssetLoader>();
    if (!pScriptAssetManager)
    {
        const std::string error = "Error Asset System Failed : ScriptAssetLoader did not Construct!";
        m_fileLog.Log(Logger::LogLevel::kError, error);
    }

    pTextureManager->Init(pWindow, pRender);
    pXMLManager->Init(m_pCoreManager);
}

void Brokkr::AssetManager::Destroy()
{
    // TODO: 
}
