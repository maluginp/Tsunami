#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QString>
#include <QVariant>
//class LibraryModel;

class ParameterModel {
public:
    ParameterModel() {};
    ParameterModel(const QString& name, double initial, double minimum, double maximum, bool fixed);

    const int&     id()        const;
    const int&     libraryId() const;
    const QString& name()      const;
    const double&  initial()   const;
    const double&  minimum()   const;
    const double&  maximum()   const;

    ParameterModel& setId(const int& id);
    ParameterModel& setLibraryId(const int& libraryId);
    ParameterModel& setName(const QString& name);
    ParameterModel& setInitial(const double& initial);
    ParameterModel& setMinimum(const double& minimum);
    ParameterModel& setMaximum(const double& maximum);


    static QVariantList getColumns();
private:
    int libraryId_;
    int paramId_;
    QString name_;
    double initial_;
    double minimum_;
    double maximum_;

};

#endif // PARAMETERMODEL_H
