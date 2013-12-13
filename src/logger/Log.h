#ifndef LOGGER_H
#define LOGGER_H

#include "LogStream.h"

namespace tsunami{
namespace log{

inline LogStream log(LogLevel level,const char* object, const char* file, int line) {
    return LogStream(LogHeader(level,object,file,line));
}


#define logT() log(tsunami::log::TraceLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logD() log(tsunami::log::DebugLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logI() log(tsunami::log::InfoLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logW() log(tsunami::log::WarningLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logE() log(tsunami::log::ErrorLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logF() log(tsunami::log::FatalLevel,Q_FUNC_INFO,__FILE__,__LINE__)

#define logTrace() log(tsunami::log::TraceLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logDebug() log(tsunami::log::DebugLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logInfo() log(tsunami::log::InfoLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logWarning() log(tsunami::log::WarningLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logError() log(tsunami::log::ErrorLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logFatal() log(tsunami::log::FatalLevel,Q_FUNC_INFO,__FILE__,__LINE__)
#define logCritical() log(tsunami::log::FatalLevel,Q_FUNC_INFO,__FILE__,__LINE__)

//class AutoLog
//{
//protected:
//    const char* mTag;
//    LogHeader mHeader;

//public:
//    AutoLog(LogLevel level, const char* name, const void* obj, const char* tag, const char* file, int line)
//        :mTag(tag),mHeader(level,name,obj,file,line)
//    {
//        LogStream(mHeader) << LogOpenCtx(tag);
//    }

//    AutoLog(LogLevel level, QObject* obj, const char* tag, const char* file, int line)
//        :mTag(tag),mHeader(level,obj->metaObject()->className(),obj,file,line)
//    {
//        LogStream(mHeader) << LogOpenCtx(tag);
//    }

//    AutoLog(LogLevel level, const char* name,  const char* tag, const char* file, int line)
//        :mTag(tag),mHeader(level,name,0,file,line)
//    {
//        LogStream(mHeader) << LogOpenCtx(tag);
//    }

//    ~AutoLog()
//    {
//        LogStream(mHeader) << LogCloseCtx(mTag);
//    }

//    LogStream log()
//    {
//        LogStream stream(mHeader);
//        return stream << LogInCtx(mTag);
//    }
//};

//#define AutoLogTrace(obj,...)      AutoLog obj(TraceLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogDebug(obj,...)      AutoLog obj(DebugLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogInfo(obj,...)       AutoLog obj(InfoLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogWarning(obj,...)    AutoLog obj(WarningLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogError(obj,...)      AutoLog obj(ErrorLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogFatal(obj,...)      AutoLog obj(FatalLevel,__VA_ARGS__,__FILE__,__LINE__)

//#define AutoLogT(obj,...)	AutoLog obj(TraceLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogD(obj,...)   AutoLog obj(DebugLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogI(obj,...)   AutoLog obj(InfoLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogW(obj,...)   AutoLog obj(WarningLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogE(obj,...)	AutoLog obj(ErrorLevel,__VA_ARGS__,__FILE__,__LINE__)
//#define AutoLogF(obj,...)   AutoLog obj(FatalLevel,__VA_ARGS__,__FILE__,__LINE__)


enum AppenderType {

    //! for Log4Qt::DailyRollingFileAppender
    DAILY_ROLLING_FILE
};


}
}

#endif // LOGGER_H
