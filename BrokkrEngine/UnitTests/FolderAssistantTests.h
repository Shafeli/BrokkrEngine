#pragma once
#include "../Utilities/FolderAssistant.h"
#include "../Logging/Logger.h"

namespace Brokkr
{
    class FolderAssistantTests
    {
        Logger m_folderAssistantTest;
        FolderAssistant m_folderAssistant;

    public:

        FolderAssistantTests();
        void Test();

    private:
        void FindFileTest();
    };
}
