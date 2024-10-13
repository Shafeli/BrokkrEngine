#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include "../Logging/Logger.h"

namespace Brokkr
{

    class FolderAssistant
    {
        inline static Logger m_fileLog{ "FolderAssistantLog" };

#pragma region HeadersAndSignatures
        static constexpr uint32_t kZipSignature = 0x06054b50;
        static constexpr uint32_t kFileSignature = 0x02014b50;
        static constexpr uint32_t kDataSignature = 0x04034b50;

#pragma pack(1)
        struct ZipHeader
        {
            uint32_t signature = 0;
            uint16_t diskIndex = 0;
            uint16_t startDisk = 0;
            uint16_t numFiles = 0;
            uint16_t totalFiles = 0;
            uint32_t dirSize = 0;
            uint32_t dirOffset = 0;
            uint16_t commentLength = 0;
        };
#pragma pack()

#pragma pack(1)
        struct FileHeader
        {
            uint32_t signature = 0;
            uint16_t versionMade = 0;
            uint16_t versionNeeded = 0;
            uint16_t flag = 0;
            uint16_t compression = 0;
            uint16_t time = 0;
            uint16_t date = 0;
            uint32_t crcCode = 0;
            uint32_t compressedSize = 0;
            uint32_t uncompressedSize = 0;
            uint16_t nameLength = 0;
            uint16_t extraLength = 0;
            uint16_t commentLength = 0;
            uint16_t startDisk = 0;
            uint16_t internalAttributes = 0;
            uint32_t externalAttributes = 0;
            uint32_t dataOffset = 0;
        };
#pragma pack()

#pragma pack(1)
        struct DataHeader
        {
            uint32_t signature = 0;
            uint16_t version = 0;
            uint16_t flag = 0;
            uint16_t compression = 0;
            uint16_t time = 0;
            uint16_t date = 0;
            uint32_t crcCode = 0;
            uint32_t compressedSize = 0;
            uint32_t uncompressedSize = 0;
            uint16_t nameLength = 0;
            uint16_t extraLength = 0;
        };
#pragma pack()
#pragma endregion

    public:

        class FileNode
        {
            char* m_pFileData = nullptr;
            DataHeader m_fileDataHeader; // trivially copyable 

        public:
            FileNode() : m_pFileData(nullptr) {}
            FileNode(char* pFileData, DataHeader dataHeader);

            void Destroy();

            [[nodiscard]] char* GetDecompressData() const;

            static bool DecompressData(char* compressedData, char* decompressedData,
                                        size_t compressedSize, size_t uncompressedSize);
        };

    private:

        std::string m_filePath;
        std::unordered_map<std::string, FileNode> m_fileData;

    public:

        FolderAssistant(const std::string& folderPath);

        FolderAssistant(const FolderAssistant&) = delete; // Copy constructor
        FolderAssistant& operator=(const FolderAssistant&) = delete; // Copy assignment operator
        FolderAssistant(FolderAssistant&&) = delete; // Move constructor
        FolderAssistant& operator=(FolderAssistant&&) = delete; // Move assignment operator

        ~FolderAssistant(); // Destructor

        bool Init(); // can be private?

        char* GetDecompressedData(const std::string& fileName);

    };
}