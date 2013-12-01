#include "logger.h"
#include <QDebug>
namespace tsunami{
namespace log{

Logger::Logger() {

}

void Logger::log(LogLevel level, int line, const char *fileName, const char *message,...) {
    char logBuffer[1024] = {0};
    va_list args;
    va_start(args,message);
    std::sprintf(logBuffer,message,args);
    va_end(args);

    if(MINIMUM_LEVEL <= level){
        qDebug() << QString("[%1] %2 %3 [%4/%5]").arg(dateNow()).arg(type(level))
                    .arg(logBuffer).arg(fileName).arg(line);
    }

}

//void Logger::assert(bool condition, LogLevel level, int line, const char *fileName, char *message,...) {
//    char logBuffer[1024] = {0};
//    va_list args;
//    va_start(args,message);
//    std::sprintf(logBuffer,message,args);
//    va_end(args);

//    qDebug() << QString("[%1] %2 %3 [%4/%5]").arg(dateNow()).arg(type(level))
//                .arg(logBuffer).arg(fileName).arg(line);
//}



char *Logger::type(LogLevel level) {
    switch(level){
    case LOG_TRACE: return "TRACE";
    case LOG_DEBUG: return "DEBUG";
    case LOG_INFO: return "INFO";
    case LOG_WARNING: return "WARN";
    case LOG_ERROR: return "ERROR";
    case LOG_FATAL: return "FATAL";
    }

    return "";
}

char *Logger::dateNow() {
    return "00-00-00 00:00:00";
}

}
}
