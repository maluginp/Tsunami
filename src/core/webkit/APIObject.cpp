#include "APIObject.h"

APIObject::APIObject(QObject *parent) :
    QObject(parent) {

    setObjectName("Api");

}

QString APIObject::test() {
    return "APIObject::test()";
}
