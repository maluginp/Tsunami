#ifndef APIOBJECT_H
#define APIOBJECT_H

#include <QObject>
#include <QString>
#include "models/analysismodel.h"

class APIObject : public QObject
{
    Q_OBJECT
public:
    explicit APIObject(QObject *parent = 0);
    void openAnalysis(const tsunami::db::AnalysisModel *analysis );

public slots:
    QString test();
signals:
    void analysisOpened(const QVariantMap& analysis);
public slots:
private:
    tsunami::db::AnalysisModel* analysis_;
};


#endif // APIOBJECT_H
