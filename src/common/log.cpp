#include "log.h"

#include <iostream>

LogWriter* Log::m_writer = nullptr;

Log::Log()
{
}

void Log::setWriter(LogWriter* writer) {
    static Log instance;
    instance.m_writer = writer;
}

void Log::addException(const char* moduleTag) {
    static Log instance;
    if (instance.m_writer != nullptr)
        instance.m_writer->addException(moduleTag);
}

void Log::t(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    if (instance.m_writer != nullptr)
        instance.m_writer->log(LOG_DETAILED, message, tag, tag_color);
}

void Log::d(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    if (instance.m_writer != nullptr)
        instance.m_writer->log(LOG_DEBUG, message, tag, tag_color);
}

void Log::w(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    if (instance.m_writer != nullptr)
        instance.m_writer->log(LOG_WARNING, message, tag, tag_color);
}

void Log::e(const char* message, const char* tag, LogColor tag_color) {
    static Log instance;
    if (instance.m_writer != nullptr)
        instance.m_writer->log(LOG_ERROR, message, tag, tag_color);
}

LogWriter::LogWriter() :
    hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
}

void LogWriter::addException(const char *moduleTag) {
    m_exceptionModules.insert(moduleTag);
}

void LogWriter::log(const LogType type, const char* message, const char* tag, LogColor tag_color) {

    if (type < m_logLevel) {
        if (m_exceptionModules.find(tag) == m_exceptionModules.end()) {
          return;
        }
    }

    std::string typeText;
    if (type == LOG_ERROR) {
        typeText = "err";
    } else if (type == LOG_WARNING) {
        typeText = "wrg";
    } else if (type == LOG_INFO) {
        typeText = "inf";
    } else if (type == LOG_DEBUG) {
        typeText = "dbg";
    } else if (type == LOG_DETAILED) {
        typeText = "dtd";
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
