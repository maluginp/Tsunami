#include "DeviceStorageTest.h"

DeviceStorageTest::DeviceStorageTest(QObject *parent) :
    QObject(parent)
{

}

void DeviceStorageTest::initTestCase() {
    storage_ = db::DeviceStorage::instance();
//    storage_= 0 ;
}

void DeviceStorageTest::dbName1() {
    QCOMPARE(1,0);
}

void DeviceStorageTest::dbName() {
//    if(!storage_){
//        QFAIL( "not connect with storage ");
//    }

    QCOMPARE( storage_->dbName(), QString("base.db"));
}
