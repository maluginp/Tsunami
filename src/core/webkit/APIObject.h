#ifndef APIOBJECT_H
#define APIOBJECT_H

#include <QObject>
#include <QString>

class APIObject : public QObject
{
    Q_OBJECT
public:
    explicit APIObject(QObject *parent = 0);
public slots:
    QString test();
signals:

public slots:

};

#endif // APIOBJECT_H
