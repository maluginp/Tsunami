#include "LogStream.h"
#include "log4qt/log4qt.h"
#include "log4qt/logger.h"
#include "log4qt/loggerrepository.h"
#include "log4qt/fileappender.h"
#include "log4qt/logmanager.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/ttcclayout.h"

namespace tsunami {
namespace log{



void outputLog(const LogHeader &h, const QString &msg) {
    static TsunamiLogger inst;
    Q_UNUSED(inst)

    Log4Qt::Level level;
    switch(h.level) {
    case TraceLevel: level = Log4Qt::Level::TRACE_INT; break;
    case DebugLevel: level = Log4Qt::Level::DEBUG_INT; break;
    case InfoLevel: level = Log4Qt::Level::INFO_INT; break;
    case WarningLevel: level = Log4Qt::Level::WARN_INT; break;
    case ErrorLevel: level = Log4Qt::Level::ERROR_INT; break;
    case FatalLevel: level = Log4Qt::Level::FATAL_INT; break;
    case OFFLevel: return;
    }

    Log4Qt::Logger* logger;
    logger = Log4Qt::LogManager::instance()->logger(h.funcName);


    if (h.fileName) {
//        logger->log(level, QString("[0x%1] %2 [%3/%4]").arg((unsigned long)h.object,2*sizeof(void*),16,QLatin1Char('0'))
//                    .arg(msg).arg(h.fileName).arg(h.lineNumber));
        logger->log(level, QString("%1 [%2/%3]").arg(msg).arg(h.fileName).arg(h.lineNumber));

    } else {
        logger->log(level, QString("%1").arg(msg));
    }
}

TsunamiLogger::TsunamiLogger() {
     Log4Qt::Logger* rootLogger = Log4Qt::LogManager::rootLogger();
//     rootLogger->setName("tsunami");
     rootLogger->setLevel( Log4Qt::Level::TRACE_INT );

     Log4Qt::FileAppender *fileAppender = new Log4Qt::FileAppender();

     Log4Qt::TTCCLayout* layout = new Log4Qt::TTCCLayout(Log4Qt::TTCCLayout::ISO8601);
     layout->setCategoryPrefixing(true);
     layout->setThreadPrinting(false);
     fileAppender->setLayout( layout  );


     fileAppender->setAppendFile( true );
     fileAppender->setFile( "tsunami.log" );
     fileAppender->activateOptions();
     rootLogger->addAppender( fileAppender );
}








}
}

