#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QString>
#include <QVariant>
//class LibraryModel;

class ParameterModel {
public:
    ParameterModel();

    inline const int&     id()        const { return paramId_;   }
    inline const int&     libraryId() const { return libraryId_; }
    inline const QString& name()      const { return name_;      }
    inline const double&  initial()   const { return initial_;   }
    inline const double&  minimum()   const { return minimum_;   }
    inline const double&  maximum()   const { return maximum_;   }

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
