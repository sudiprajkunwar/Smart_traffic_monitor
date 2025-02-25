#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

enum class LogLevel
{
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static Logger &getInstance();
    void log(LogLevel level, const std::string &message);

private:
    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    std::ofstream logFile;

    std::string getTimestamp();
    std::string logLevelToString(LogLevel level);
};

#endif
