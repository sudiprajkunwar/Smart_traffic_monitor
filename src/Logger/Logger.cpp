#include "Logger.h"

Logger::Logger()
{
    logFile.open("app.log", std::ios::app);
    if (!logFile)
    {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const std::string &message)
{

    std::string timestamp = getTimestamp();
    std::string logMessage = "[" + timestamp + "] [" + logLevelToString(level) + "] " + message;

    // Print to console
    std::cout << logMessage << std::endl;
}

std::string Logger::getTimestamp()
{
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

std::string Logger::logLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}
