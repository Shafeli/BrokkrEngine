#include "ArchiveManager.h"

#include <cassert>

void Brokkr::ArchiveManager::LoadFolder(const char* filePath)
{
    // Convert input char* to string
    const std::string str_path(filePath);

    // Find position of last '/' character and extract filename from string
    const std::string filename = str_path.substr(str_path.find_last_of("/") + 1);

    // Check if folder with the same name already exists
    if (m_assetArchives.find(filename.c_str()) != m_assetArchives.end())
    {
        assert(false && "Folder with same name already exists!");
        return;
    }

    // File name as key "../BrokkrEngine/assets/ZipTest.zip" becomes "ZipTest.zip"
    m_assetArchives[filename.c_str()] = std::make_unique<FolderAssistant>(filePath);

}

char* Brokkr::ArchiveManager::GetAssetData(const char* assetName)
{
    // Iterate over all FolderAssistant objects in the unordered_map
    for (const auto& [Folder, Asset] : m_assetArchives)
    {
        // Search for the file in the current FolderAssistant object
        if (auto* pFileData = Asset->GetDecompressedData(assetName); pFileData != nullptr)
        {
            // File found
            return pFileData;
        }
    }

    const std::string fileNameToFind = assetName;

    // file is not found
    const std::string error = "File name not found : " + fileNameToFind;
    m_archiveManagerLog.Log(Logger::LogLevel::kError, error);
    return nullptr;
}

char* Brokkr::ArchiveManager::GetAssetData(const char* fileName, const char* assetName)
{
    return m_assetArchives[fileName].get()->GetDecompressedData(assetName);
}

void Brokkr::ArchiveManager::Destroy()
{
    m_assetArchives.clear();
}
