#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QString>
#include <QVariant>
class ParameterModel {
public:
    ParameterModel(const QString& name, double initial, double minimum, double maximum, bool fixed);
    int&     id()        const;
    int&     libraryId() const;
    QString& name()      const;
    double&  initial()   const;
    double&  minimum()   const;
    double&  maximum()   const;
    bool&    fixed()     const;
private:
    int libraryId_;
    int paramId_;
    QString name_;
    double initial_;
    double minimum_;
    double maximum_;
    bool fixed_;


};

#endif // PARAMETERMODEL_H
