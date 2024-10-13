#include "FolderAssistant.h"
#include <filesystem>
#include <fstream>
#include <iostream>

#define ZLIB_WINAPI
#include "../../lib/zlib/zlib.h"

Brokkr::FolderAssistant::FileNode::FileNode(char* pFileData, const DataHeader dataHeader)
    :m_pFileData(pFileData)
    ,m_fileDataHeader(dataHeader)
{
}

void Brokkr::FolderAssistant::FileNode::Destroy()
{
    if (m_pFileData != nullptr)
    {
        delete[] m_pFileData;
        m_pFileData = nullptr;
        m_fileDataHeader = {}; // Reset data header
    }
}

char* Brokkr::FolderAssistant::FileNode::GetDecompressData() const
{
    if (m_pFileData == nullptr)
    {
        const std::string error =
            "Error file data is not loaded in Node : "
            + std::to_string(m_fileDataHeader.nameLength);

        m_fileLog.Log(Logger::LogLevel::kError, error);

        // The file data is not loaded, return an empty string
        return nullptr;
    }

    char* compressedData = m_pFileData;
    char* uncompressedData = new char[m_fileDataHeader.uncompressedSize];

    if (!DecompressData(compressedData, uncompressedData, m_fileDataHeader.compressedSize, m_fileDataHeader.uncompressedSize))
    {
        const std::string error =
            "Error decompressing data in Node : "
            + std::to_string(m_fileDataHeader.nameLength);

        m_fileLog.Log(Logger::LogLevel::kError, error);

        delete[] uncompressedData;
        return nullptr; // The decompression failed, return an empty string
    }

    return uncompressedData; // Return the uncompressed data
}

bool Brokkr::FolderAssistant::FileNode::DecompressData(char* compressedData,
    char* decompressedData, size_t compressedSize, size_t uncompressedSize)
{

    z_stream zStream;

    zStream.next_in = reinterpret_cast<Bytef*>(compressedData);
    zStream.avail_in = static_cast<uInt>(compressedSize);

    zStream.next_out = reinterpret_cast<Byte*>(decompressedData);
    zStream.avail_out = static_cast<uInt>(uncompressedSize);

    zStream.zalloc = static_cast<alloc_func>(nullptr);
    zStream.zfree = static_cast<free_func>(nullptr);

    const int result = inflateInit2(&zStream, -MAX_WBITS);
    if (result != Z_OK) // check if inflateInit succeeded
    {
        const std::string error = "Error initializing zlib stream";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return false;
    }

    const int inflateResult = inflate(&zStream, Z_FINISH); // try to inflate the compressed data
    if (inflateResult != Z_STREAM_END) // check if inflate succeeded
    {
        inflateEnd(&zStream); // cleanup
        const std::string error = "Error decompressing data";
        m_fileLog.Log(Logger::LogLevel::kError, error);
        return false;
    }

    inflateEnd(&zStream); // cleanup

    return true;
}

Brokkr::FolderAssistant::FolderAssistant(const std::string& folderPath)
    : m_filePath(folderPath)
{
    Init();
}

Brokkr::FolderAssistant::~FolderAssistant()
{
    for (auto& filePair : m_fileData)
    {
        filePair.second.Destroy();
    }
}

bool Brokkr::FolderAssistant::Init()
{
    std::ifstream file(m_filePath, std::ios::in | std::ios::binary); // read in the binary data

    ZipHeader zipHeader;

    constexpr std::streamoff kZipOffset = -static_cast<std::streamoff>(sizeof(zipHeader));

    file.seekg(kZipOffset, std::ios::end);

    file.read(reinterpret_cast<char*>(&zipHeader), sizeof(zipHeader));

    if (zipHeader.signature != kZipSignature)
    {
        const std::string error =
            "Error Reading Folder : "
            + m_filePath
            + " signature does not match Zip : "
            + std::to_string(zipHeader.signature);

        m_fileLog.Log(Logger::LogLevel::kError, error);

        return false;
    }

    const std::streamoff dirOffset = zipHeader.dirOffset;

    file.seekg(dirOffset, std::ios::beg);

    char* pDirData = new char[zipHeader.dirSize];
    memset(pDirData, 0, zipHeader.dirSize);

    file.read(pDirData, zipHeader.dirSize);

    FileHeader* pFileHeader = reinterpret_cast<FileHeader*>(pDirData);

    for (size_t i = 0; i < zipHeader.numFiles; ++i)
    {
        if (pFileHeader->signature != kFileSignature)
        {
            const std::string error =
                "Error Reading file : "
                + m_filePath
                + " signature does not match File : "
                + std::to_string(pFileHeader->signature);

            m_fileLog.Log(Logger::LogLevel::kError, error);
            return false;
        }

        char* pNamePtr = reinterpret_cast<char*>(pFileHeader + 1);
        char* pName = new char[pFileHeader->nameLength + 1];

        memcpy(pName, pNamePtr, pFileHeader->nameLength);

        pName[pFileHeader->nameLength] = '\0';

        DataHeader dataHeader;

        file.seekg(pFileHeader->dataOffset, std::ios::beg);

        file.read(reinterpret_cast<char*>(&dataHeader), sizeof(DataHeader));

        if (dataHeader.signature != kDataSignature)
        {
            const std::string error =
                "Error Data : "
                + m_filePath
                + " signature does not match : "
                + std::to_string(dataHeader.signature)
                + pName;

            m_fileLog.Log(Logger::LogLevel::kError, error);
            return false;
        }

        file.seekg(dataHeader.nameLength, std::ios::cur);
        char* pData = new char[dataHeader.compressedSize];

        file.read(pData, dataHeader.compressedSize);

        m_fileData.emplace(pName, FileNode(pData, dataHeader));

        pFileHeader = reinterpret_cast<FileHeader*>(pNamePtr + pFileHeader->nameLength);
        delete[] pName;
    }

    const std::string passed =
        "Reading file : "
        + m_filePath
        + " signature does match File : "
        + std::to_string(pFileHeader->signature);

    m_fileLog.Log(Logger::LogLevel::kInfo, passed);

    return true;
}

char* Brokkr::FolderAssistant::GetDecompressedData(const std::string& fileName)
{
    if (const auto findIt = m_fileData.find(fileName); findIt != m_fileData.end())
    {
        return m_fileData[fileName].GetDecompressData();
    }

    const std::string error = "File name not found : " + fileName;
    m_fileLog.Log(Logger::LogLevel::kError, error);
    return nullptr;
}
