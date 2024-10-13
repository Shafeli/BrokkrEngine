#include "Logger.h"
#include <iostream>

Brokkr::Logger::Logger(const std::string& logFileName)
    : m_logFilePath("logs\\")
{

    m_logFilePath = m_logFilePath + logFileName;
    m_logFilePath = m_logFilePath + ".txt";

    // Open file in trunc mode
    // might change to a append mode or increase the text number if already in folder
    // too keep the older log
    // or a switch to do that instead will see as project grows and whats needed
    m_logFile.open(m_logFilePath, std::ios::trunc);
}

Brokkr::Logger::~Logger()
{
    if (m_logFile.is_open()) 
    {
        /*Log(LogLevel::kInfo, "File Closed by Destructor Control");*/
        m_logFile.close();
    }
}

void Brokkr::Logger::Log(LogLevel level, const std::string& message)
{
    if (!m_logFile.is_open())
    {
        SetConsoleTextAttribute(m_console, 12);
        std::cout << "Logging Failed... File is closed" << std::endl;
        SetConsoleTextAttribute(m_console, 15);
        return;
    }

    std::string levelString;

    switch (level)
    {
    case LogLevel::kInfo:
        // setting the color to green
        SetConsoleTextAttribute(m_console, 10);
        levelString = "[~INFO~]";
        break;

    case LogLevel::kDebug:
        //setting color to yellow
        SetConsoleTextAttribute(m_console, 14);
        levelString = "[~DEBUG~]";
        break;

    case LogLevel::kWarning:
        //setting color to purple
        SetConsoleTextAttribute(m_console, 13);
        levelString = "[~WARNING~]";
        break;

    case LogLevel::kError:
        // setting color to red 
        SetConsoleTextAttribute(m_console, 12);
        levelString = "[~ERROR~]";
        break;
    }

    const std::string fullLog = levelString + " " + message;

    std::cout << fullLog << std::endl;
    m_logFile << fullLog << std::endl;

    // resets color to base white
    SetConsoleTextAttribute(m_console, 15);

}

void Brokkr::Logger::CloseFile()
{
    if (m_logFile.is_open())
    {
        /*Log(LogLevel::kInfo, "File Closed by user Control");*/
        m_logFile.close();
    }
}
