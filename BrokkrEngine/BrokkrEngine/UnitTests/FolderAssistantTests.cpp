#include "FolderAssistantTests.h"

Brokkr::FolderAssistantTests::FolderAssistantTests()
    :m_folderAssistantTest("FolderAssistantTests")
    ,m_folderAssistant("../BrokkrEngine/assets/ZipTest.zip")
{

}

void Brokkr::FolderAssistantTests::Test()
{

    FindFileTest();
}

void Brokkr::FolderAssistantTests::FindFileTest()
{
    auto test = m_folderAssistant.GetDecompressedData("ZipTest.txt");
    auto test2 = m_folderAssistant.GetDecompressedData("Water.png");

    if (test == nullptr)
    {
        m_folderAssistantTest.Log(Logger::LogLevel::kDebug, "Tested Failed : File not found");
        return;
    }
    m_folderAssistantTest.Log(Logger::LogLevel::kInfo, "Test Passed : ZipTest.txt File Found : Decompression system Works!");

    if (test2 == nullptr)
    {
        m_folderAssistantTest.Log(Logger::LogLevel::kDebug, "Tested Failed : File not found");
        return;
    }
    m_folderAssistantTest.Log(Logger::LogLevel::kInfo, "Test Passed : Water.png File Found : Decompression system Works!");

    delete[] test;
    delete[] test2;
}
