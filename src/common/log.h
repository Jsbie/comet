#ifndef LOG_H
#define LOG_H

#include <string>

#include "platforms.h"

#ifdef windows
    #define NOMINMAX
    #include <windows.h>
#endif

enum LogType {
    LOG_DEBUG   = 0,
    LOG_INFO    = 1,
    LOG_WARNING = 2,
    LOG_ERROR   = 3
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

class Log
{
public:
    Log();

    // Singleton
    Log(Log const&) = delete;
    void operator=(Log const&) = delete;

    const LogType m_logLevel = LOG_DEBUG;

    static void d(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);
    static void w(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);
    static void e(const char* message, const char* tag = "", LogColor tag_color = LogColor::WHITE);

private:
    void log(const LogType type, const char* message, const char* tag, LogColor tag_color);

    std::string m_logPath;

#ifdef windows
    HANDLE hConsole;
#endif

};

#endif // LOG_H
