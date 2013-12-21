#ifndef DEVICESTORAGETEST_H
#define DEVICESTORAGETEST_H

#include <QObject>
#include "dbstorage/DeviceStorage.h"

#include "AutoTest.h"

using namespace tsunami;


class DeviceStorageTest : public QObject {
    Q_OBJECT
public:
    explicit DeviceStorageTest(QObject *parent = 0);

private:
    db::DeviceStorage* storage_;
private slots:

    void initTestCase();

    void dbName();
    void dbName1();
    void cleanupTestCase() {}
};

DECLARE_TEST(DeviceStorageTest)

#endif // DEVICESTORAGETEST_H
