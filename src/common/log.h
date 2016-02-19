#ifndef LOG_H
#define LOG_H

#include <string>

#include "platforms.h"

#ifdef windows
    #define NOMINMAX
    #include <windows.h>
#endif

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

    static void d(std::string message, std::string tag = "", LogColor tag_color = LogColor::WHITE);
    static void e(std::string message, std::string tag = "", LogColor tag_color = LogColor::WHITE);

private:
    void log(std::string message, std::string tag, LogColor tag_color);

    std::string m_logPath;

#ifdef windows
    HANDLE hConsole;
#endif

};

#endif // LOG_H
