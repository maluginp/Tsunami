#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <QTextStream>
#include <QString>
#include <QTextCodec>

namespace tsunami {
namespace log{


enum LogLevel
{
    TraceLevel = 0,
    DebugLevel,
    InfoLevel,
    WarningLevel,
    ErrorLevel,
    FatalLevel,
    OFFLevel
};

struct LogHeader {
    LogLevel level;
//    const void* object;
    const char* funcName;
    const char* fileName;
    int lineNumber;

    LogHeader(LogLevel lev, const char* func, const char* file, int line)
        :level(lev), funcName(func), fileName(file), lineNumber(line)
    {}
};


void Q_DECL_EXPORT outputLog(const LogHeader& h, const QString& msg);

class TsunamiLogger {
public:
 TsunamiLogger();

 ~TsunamiLogger(){}
private:

};


// open a log context ********************************************/
template<class T>
struct TLogOpenCtx
{
    const T& tag;
    TLogOpenCtx(const T& t):tag(t){}
};

template<>
struct TLogOpenCtx<const char*>
{
    const char* tag;
    TLogOpenCtx(const char* t):tag(t){}
};

template<>
struct TLogOpenCtx<char*>
{
    const char* tag;
    TLogOpenCtx(const char* t):tag(t){}
};

template<class T>
inline TLogOpenCtx<T> LogOpenCtx(const T& t) {return TLogOpenCtx<T>(t);}


// close a log context ********************************************/
template<class T>
struct TLogCloseCtx
{
    const T& tag;
    TLogCloseCtx(const T& t):tag(t){}
};

template<>
struct TLogCloseCtx<const char*>
{
    const char* tag;
    TLogCloseCtx(const char* t):tag(t){}
};

template<>
struct TLogCloseCtx<char*>
{
    const char* tag;
    TLogCloseCtx(const char* t):tag(t){}
};

template<class T>
inline TLogCloseCtx<T> LogCloseCtx(const T& t) {return TLogCloseCtx<T>(t);}


// write in a log context ********************************************/
template<class T>
struct TLogInCtx {
    const T& tag;
    TLogInCtx(const T& t):tag(t){}
};

template<>
struct TLogInCtx <const char*>{
    const char* tag;
    TLogInCtx(const char* t):tag(t){}
};

template<>
struct TLogInCtx <char*>{
    const char* tag;
    TLogInCtx(const char* t):tag(t){}
};

template<class T>
inline TLogInCtx<T> LogInCtx(const T& t) {return TLogInCtx<T>(t);}


// LogStream **********************************************************/
class LogStream
{
private:
    struct Stream {
        Stream(const LogHeader& t)
            :ts(&buffer, QIODevice::WriteOnly),
            ref(1),space(true),message_output(true),
            header(t)
            {}

        QTextStream ts;
        QString buffer;
        int ref;
        bool space;
        bool message_output;
        LogHeader header;

    } *stream;

public:
    inline LogStream(const LogHeader& header)
        : stream(new Stream(header))
    {}

    inline LogStream(const LogStream &o):stream(o.stream) { ++stream->ref; }
    inline LogStream &operator=(const LogStream &other);

    inline ~LogStream()
    {
        if (!--stream->ref) {
            if(stream->message_output) outputLog(stream->header,stream->buffer);
            delete stream;
        }
    }

    inline LogStream &space() { stream->space = true; stream->ts << ' '; return *this; }
    inline LogStream &nospace() { stream->space = false; return *this; }
    inline LogStream &maybeSpace() { if (stream->space) stream->ts << ' '; return *this; }

    inline LogStream &operator<<(QChar t) { stream->ts << '\'' << t << '\''; return maybeSpace(); }
    inline LogStream &operator<<(QBool t) { stream->ts << (bool(t != 0) ? "true" : "false"); return maybeSpace(); }
    inline LogStream &operator<<(bool t) { stream->ts << (t ? "true" : "false"); return maybeSpace(); }
    inline LogStream &operator<<(char t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(signed short t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(unsigned short t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(signed int t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(unsigned int t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(signed long t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(unsigned long t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(qint64 t)  { stream->ts << QString::number(t); return maybeSpace(); }
    inline LogStream &operator<<(quint64 t) { stream->ts << QString::number(t); return maybeSpace(); }
    inline LogStream &operator<<(float t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(double t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(const char* t) { stream->ts << QString::fromAscii(t); return maybeSpace(); }
    inline LogStream &operator<<(const QString & t) { stream->ts << t ; return maybeSpace(); }
    inline LogStream &operator<<(const QStringRef & t) { return operator<<(t.toString()); }
    inline LogStream &operator<<(const QLatin1String &t) { stream->ts << t.latin1() ; return maybeSpace(); }
    inline LogStream &operator<<(const QByteArray & t) { stream->ts << t ; return maybeSpace(); }
    inline LogStream &operator<<(const void * t) { stream->ts << t; return maybeSpace(); }
    inline LogStream &operator<<(QTextStreamFunction f) {stream->ts << f; return *this; }
    inline LogStream &operator<<(QTextStreamManipulator m) { stream->ts << m; return *this; }
};

inline LogStream &LogStream::operator=(const LogStream &other)
{
    if (this != &other) {
        LogStream copy(other);
        qSwap(stream, copy.stream);
    }
    return *this;
}

template <class T>
inline LogStream operator<<(LogStream debug, const QList<T> &list)
{
    debug.nospace() << '(';
    for (Q_TYPENAME QList<T>::size_type i = 0; i < list.count(); ++i) {
        if (i)
            debug << ", ";
        debug << list.at(i);
    }
    debug << ')';
    return debug.space();
}

template <typename T>
inline LogStream operator<<(LogStream debug, const QVector<T> &vec)
{
    debug.nospace() << "QVector";
    return operator<<(debug, vec.toList());
}

template <class aKey, class aT>
inline LogStream operator<<(LogStream debug, const QMap<aKey, aT> &map)
{
    debug.nospace() << "QMap(";
    for (typename QMap<aKey, aT>::const_iterator it = map.constBegin();
         it != map.constEnd(); ++it) {
        debug << '(' << it.key() << ", " << it.value() << ')';
    }
    debug << ')';
    return debug.space();
}

template <class aKey>
inline LogStream operator<<(LogStream debug, const QMap<aKey, QVariant> &map)
{
    debug.nospace() << "QMap(";
    for (typename QMap<aKey, QVariant>::const_iterator it = map.constBegin();
         it != map.constEnd(); ++it) {
        debug << '(' << it.key() << ", " << it.value().toString() << ')';
    }
    debug << ')';
    return debug.space();
}

template <class aKey, class aT>
inline LogStream operator<<(LogStream debug, const QHash<aKey, aT> &hash)
{
    debug.nospace() << "QHash(";
    for (typename QHash<aKey, aT>::const_iterator it = hash.constBegin();
            it != hash.constEnd(); ++it)
        debug << '(' << it.key() << ", " << it.value() << ')';
    debug << ')';
    return debug.space();
}

template <class T1, class T2>
inline LogStream operator<<(LogStream debug, const QPair<T1, T2> &pair)
{
    debug.nospace() << "QPair(" << pair.first << ',' << pair.second << ')';
    return debug.space();
}

template <typename T>
inline LogStream operator<<(LogStream debug, const QSet<T> &set)
{
    debug.nospace() << "QSet";
    return operator<<(debug, set.toList());
}

template <class T>
inline LogStream operator<<(LogStream debug, const QFlags<T> &flags)
{
    debug.nospace() << "QFlags(";
    bool needSeparator = false;
    for (uint i = 0; i < sizeof(T) * 8; ++i) {
        if (flags.testFlag(T(1 << i))) {
            if (needSeparator)
                debug.nospace() << '|';
            else
                needSeparator = true;
            debug.nospace() << "0x" << QByteArray::number(T(1 << i), 16).constData();
        }
    }
    debug << ')';
    return debug.space();
}

template <class T>
inline LogStream operator<<(LogStream debug, const TLogOpenCtx<T>& ctx)
{
    debug.nospace() << "[+ " << ctx.tag << "]";
    return debug.space();
}

template <class T>
inline LogStream operator<<(LogStream debug, const TLogCloseCtx<T>& ctx)
{
    debug.nospace() << "[- " << ctx.tag << "]";
    return debug.space();
}

template <class T>
inline LogStream operator<<(LogStream debug, const TLogInCtx<T>& ctx)
{
    debug.nospace() << "[= " << ctx.tag << "]";
    return debug.space();
}

}
}

#endif // LOGSTREAM_H
