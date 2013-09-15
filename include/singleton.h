#ifndef SINGLETON_H
#define SINGLETON_H
#include <QtCore>
template<class T>
class Singleton
{
public:
    T* instance(){
        if(!instance_){
            mutexSingleton.lock();
            if(!instance_){
                instance_ = new T;
            }
            mutexSingleton.unlock();
        }

        return instance_;
    }

private:
    Q_DISABLE_COPY(Singleton)
    static QMutex mutexSingleton;
    static T* instance_;
};

template<class T> T* Singleton<T>::instance_ = NULL;
template<class T> QMutex Singleton<T>::mutxSingletion = QMutex();
#endif // SINGLETON_H
