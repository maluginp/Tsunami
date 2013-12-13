#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QString>
#include <QVariant>

namespace tsunami {
namespace db{

class ParameterModel {
public:
    ParameterModel();
    ParameterModel(const ParameterModel& other);
    ParameterModel& operator=(const ParameterModel& other);
    void id(int parameterId) { parameterId_ = parameterId; }
    void libraryId(int libraryId) { libraryId_ = libraryId; }
    void name(const QString& name) { name_ = name; }
    void initial(double initial) { initial_ = initial; }
    void fitted( double fitted ) { fitted_ = fitted; }
    void minimum(double minimum ) {minimum_ = minimum;}
    void maximum(double maximum) {maximum_ = maximum;}
    void fixed(bool fixed) {fixed_ = fixed; }
    void enable(bool enable) { enable_ = enable; }

    const int& id() const { return parameterId_;}
    const int& libraryId() const { return libraryId_; }
    const QString&  name() const { return name_; }
    const double& initial() const { return initial_;}
    const double& fitted() const { return fitted_;}
    const double& minimum() const { return minimum_; }
    const double& maximum() const { return maximum_; }
    const bool& fixed() const { return fixed_; }
    const bool& enable() const { return enable_; }

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
