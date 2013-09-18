#ifndef SINGLETON_H
#define SINGLETON_H
#include <QMutex>

template<class T>
class Singleton {

public:

    static T*
    instance() {
        if (instance_ == 0) {
            mutex_.lock();
            if (instance_ == 0) instance_ = new T;
        }
        return instance_;
    }

    static void
    drop() {
        if (instance_) {
            mutex_.lock();
            if (instance_) {
                delete instance_;
                instance_ = 0;
            }
        }
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

    Q_DISABLE_COPY(Singleton<T>)

private:
    static T* instance_;
    static QMutex mutex_;
};

template <typename T>
T* Singleton<T>::instance_ = NULL;

template <typename T>
QMutex Singleton<T>::mutex_;

#endif // SINGLETON_H
