#include "log.h"

#include <iostream>

Log::Log() :
    hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
    d("Created log" , "LOG");
}

void Log::d(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    instance.log(LOG_DEBUG, message, tag, tag_color);
}

void Log::w(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    instance.log(LOG_WARNING, message, tag, tag_color);
}

void Log::e(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    instance.log(LOG_ERROR, message, tag, tag_color);
}

void Log::log(const LogType type, const char* message, const char* tag, LogColor tag_color) {

    std::string typeText;
    if (type == LOG_ERROR) {
        typeText = "err";
    } else if (type == LOG_WARNING) {
        typeText = "wrg";
    } else if (type == LOG_INFO) {
        typeText = "inf";
    } else if (type == LOG_DEBUG) {
        typeText = "dbg";
    }

    std::cout << typeText << " - ";

    if (strcmp(tag, "") != 0) {
        switch (tag_color) {
            case(LogColor::WHITE):
                break;
            default:
                break;
        }

    #ifdef windows
        SetConsoleTextAttribute(hConsole, 2);
    #endif

        std::cout << tag << " - ";

    #ifdef windows
        SetConsoleTextAttribute(hConsole, 15);
    #endif
    }

    std::cout << message << std::endl;
}
