#include "log.h"

#include <iostream>

Log::Log() :
    hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
    d("Created log" , "LOG");
}

void Log::d(std::string message, std::string tag, LogColor tag_color) {
    static Log instance;
    instance.log(message, tag, tag_color);
}

void Log::e(std::string message, std::string tag, LogColor tag_color) {
    static Log instance;
    instance.log(message, tag, tag_color);
}

void Log::log(std::string message, std::string tag, LogColor tag_color) {
    if (tag != "") {
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
