#include "ScriptAssetLoader.h"
#include <filesystem>
#include <windows.h>

int Brokkr::ScriptAssetLoader::GetNumLoadedScripts() const
{
    return static_cast<int>(m_pScriptAssets.size());
}

std::vector<std::string> Brokkr::ScriptAssetLoader::GetScriptNames() const
{
    std::vector<std::string> scriptNames;
    for (const auto& pair : m_pScriptAssets)
    {
        scriptNames.emplace_back(pair.first);
    }
    return scriptNames;
}

std::string Brokkr::ScriptAssetLoader::GetScriptName(const std::string& scriptPath)
{
    namespace fs = std::filesystem;
    const fs::path path(scriptPath);
    const std::string fileName = path.filename().generic_string();

    if (!fileName.empty())
    {
        return fileName;
    }
    return "";
}

std::vector<std::string> Brokkr::Script::GetAbsolutePathsFromFolder(const std::string& folderPath,
    const std::string& extension)
{
    std::vector<std::string> filePaths;   // Declare a vector to hold the file paths

    // Construct the search path for the files using the folder path and extension
    const std::string searchPath = folderPath + "/*." + extension;

    // Declare a variable to hold information about the found files
    WIN32_FIND_DATAA findData;

    // Search for the files in the directory using the search path and get the handle to the first file
    const HANDLE findHandle = FindFirstFileA(searchPath.c_str(), &findData);

    // Check if any files were found
    if (findHandle != INVALID_HANDLE_VALUE)
    {
        // Loop through all the found files
        do
        {
            // Check if the file is not a directory
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // Extract the file name from the data
                std::string fileName(findData.cFileName);

                // Construct the full file path by appending the file name to the folder path
                std::string filePath = folderPath + "/" + fileName;

                // Add the file path to the vector
                filePaths.push_back(filePath);
            }
        } while (FindNextFileA(findHandle, &findData));  // Get the next file and continue looping

        FindClose(findHandle);  // Close the search handle
    }

    return filePaths;  // Return the vector of file paths
}

std::vector<std::string> Brokkr::Script::GetRelativePathsFromFolder(const char* folderPath, const char* extension)
{
    std::vector<std::string> relativePaths;

    namespace fs = std::filesystem;
    for (const auto& entry : fs::recursive_directory_iterator(folderPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == extension)
        {
            fs::path relativePath = fs::relative(entry.path(), folderPath);
            relativePaths.emplace_back(relativePath.generic_string());
        }
    }

    return relativePaths;
}

Brokkr::Script* Brokkr::ScriptAssetLoader::GetScript(const char* scriptName)
{
    return m_pScriptAssets[scriptName].get();
}

void Brokkr::ScriptAssetLoader::FreeScriptAsset(const char* scriptName)
{
    if (const auto iterator = m_pScriptAssets.find(scriptName); iterator != m_pScriptAssets.end()) // If the item was found
    {
        if (const int remainingRefs = iterator->second->RemoveRef(); remainingRefs <= 0)
        {
            iterator->second->Destroy();
            m_pScriptAssets.erase(iterator); // Remove the item from the map
        }
    }
}

void Brokkr::ScriptAssetLoader::Destroy()
{
    m_scriptFolders.clear();

    for (const auto& scripts : m_pScriptAssets)
    {
        scripts.second->Destroy();
    }

    m_pScriptAssets.clear();
}
