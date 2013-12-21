#ifndef AUTOTEST_H
#define AUTOTEST_H

#include <QtCore>
#include <QtTest>

typedef QList<QObject*> TestList;
inline TestList& getTestList(){
    static TestList tests;
    return tests;
}

class AutoTest{

public:
    static void addTest(QObject *object){
        TestList& list = getTestList();
        if (!findObject(object))
        {
            list.append(object);
        }
    }

    static int run(int argc, char *argv[]) {
        int ret = 0;

        foreach (QObject* test, getTestList()){
            ret += QTest::qExec(test, argc, argv);
        }

        return ret;
    }

private:
    static bool findObject(QObject *object){
        TestList& list = getTestList();
        if (list.contains(object)) {
            return true;
        }
        foreach (QObject* test, list) {
            if (test->objectName() == object->objectName()) {
                return true;
            }
        }
        return false;
    }
};

template <class T>
class Test {
public:
    QSharedPointer<T> child;

    Test(const QString& name) : child(new T) {
        child->setObjectName(name);
        AutoTest::addTest(child.data());
    }
};

#define DECLARE_TEST(className) static Test<className> t(#className);

#endif // AUTOTEST_H
