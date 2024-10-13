#pragma once
#include <fstream>
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// when creating a logger, the file name you give will be the name of the txt file
//
// Logger logger("example.txt");
// in the log folder
//
// logger.Log(Logger::LogLevel::Info,       "Starting insert system something");
// logger.Log(Logger::LogLevel::Debug,      "Loading something game assets failed");
// logger.Log(Logger::LogLevel::Warning,    "Something is a warning");
// logger.Log(Logger::LogLevel::Error,      "Something crashed!");
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Brokkr
{

    class Logger
    {
        std::ofstream m_logFile;
        std::string m_logFilePath;

        // handle for the console
        HANDLE m_console = GetStdHandle(STD_OUTPUT_HANDLE);
    public:

        enum class LogLevel
        {
            kDebug,
            kInfo,
            kWarning,
            kError
        };

        explicit Logger(const std::string& logFileName);
        ~Logger();

        void Log(LogLevel level, const std::string& message);
        void CloseFile();

    };

}
