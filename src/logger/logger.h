#ifndef LOGGER_H
#define LOGGER_H

#include <singleton.h>

namespace tsunami{
namespace log{

#define MINIMUM_LEVEL 1

enum LogLevel{
    LOG_TRACE = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_OFF
};

#define logTrace(message) Logger::instance()->log(log::LOG_TRACE,__LINE__,__FILE__,message)
#define logDebug(message) Logger::instance()->log(log::LOG_DEBUG,__LINE__,__FILE__,message)
#define logInfo(message) Logger::instance()->log(log::LOG_INFO,__LINE__,__FILE__,message)
#define logWarn(message) Logger::instance()->log(log::LOG_WARNING,__LINE__,__FILE__,message)
#define logError(message) Logger::instance()->log(log::LOG_ERROR,__LINE__,__FILE__,message)
#define logFatal(message) Logger::instance()->log(log::LOG_FATAL,__LINE__,__FILE__,message)

#define logTraceF(message,...) Logger::instance()->log(log::LOG_TRACE,__LINE__,__FILE__,message,__VA_ARGS__)
#define logDebugF(message,...) Logger::instance()->log(log::LOG_DEBUG,__LINE__,__FILE__,message,__VA_ARGS__)
#define logInfoF(message,...) Logger::instance()->log(log::LOG_INFO,__LINE__,__FILE__,message,__VA_ARGS__)
#define logWarnF(message,...) Logger::instance()->log(log::LOG_WARNING,__LINE__,__FILE__,message,__VA_ARGS__)
#define logErrorF(message,...) Logger::instance()->log(log::LOG_ERROR,__LINE__,__FILE__,message,__VA_ARGS__)
#define logFatalF(message,...) Logger::instance()->log(log::LOG_FATAL,__LINE__,__FILE__,message,__VA_ARGS__)


//#define logTrace Logger::instance()


class Logger : public Singleton<Logger> {
public:
    Logger();
    void log(LogLevel level, int line, const char* fileName, const char *message,...);
//    void assert(bool condition, LogLevel level, int line, const char* fileName, char* message,...);

private:
    char *type(LogLevel level);
    char *dateNow();

};

}
}

#endif // LOGGER_H
