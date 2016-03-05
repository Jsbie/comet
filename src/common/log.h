#ifndef LOG_H
#define LOG_H

#include <string>
#include <unordered_set>

#include "platforms.h"

#ifdef windows
    #define NOMINMAX
    #include <windows.h>
#endif

enum LogType {
    LOG_DETAILED    = 0,
    LOG_DEBUG       = 1,
    LOG_INFO        = 2,
    LOG_WARNING     = 3,
    LOG_ERROR       = 4
};

enum class LogColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

class LogWriter
{
public:
    LogWriter();

    void addException(const char* moduleTag);
    void log(const LogType type, const char* message, const char* tag, LogColor tag_color);

private:
    const LogType m_logLevel = LOG_DEBUG;
    std::unordered_set<std::string> m_exceptionModules;     ///< Modules that will log all their messages overriding global log level

#ifdef windows
    HANDLE hConsole;
#endif
};

class Log
{
public:
    Log();

    // Singleton
    Log(Log const&) = delete;
    void operator=(Log const&) = delete;

    static void setWriter(LogWriter* writer);
    static void addException(const char* moduleTag);
    static void t(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);
    static void d(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);
    static void w(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);
    static void e(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);

private:
    static LogWriter* m_writer;
    //static std::string m_logPath;
};

#endif // LOG_H
