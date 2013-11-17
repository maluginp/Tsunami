#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QString>
#include <QVariant>
//class LibraryModel;
namespace tsunami {
namespace db{

class ParameterModel {
public:
    ParameterModel();
    void id(int parameterId);
    void libraryId(int libraryId);
    void name(const QString& name);
    void initial(double initial);
    void fitted( double fitted );
    void minimum(double minimum );
    void maximum(double maximum);
    void fixed(bool fixed);
    void enable(bool enable);

    const int& id() const;
    const int& libraryId() const;
    const QString&  name() const;
    const double& initial() const;
    const double& fitted() const;
    const double& minimum() const;
    const double& maximum() const;
    const bool& fixed() const;
    const bool& enable() const;

private:
    int libraryId_;
    int parameterId_;
    QString name_;
    double initial_;
    double fitted_;
    double minimum_;
    double maximum_;
    bool fixed_;
    bool enable_;

};

}
}
#endif // PARAMETERMODEL_H
