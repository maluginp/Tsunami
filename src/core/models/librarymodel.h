#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H
#include <QDateTime>
#include <Model.h>
#include <QModelIndex>
//#include "../dbstorage/parameterstorage.h"
namespace tsunami {
namespace db{

class ParameterModel;

class LibraryModel : public Model {
//    friend class ParameterStorage;
public:
    void id(int libraryId);
    void deviceId(int deviceId);
    void name(const QString& name);
    void createAt(const QDate& createAt);
    void changeAt(const QDate& changeAt);
    void enable(bool enable);

    const int& id() const;
    const int& deviceId() const;
    const QString& name() const;
    const QDate& createAt() const;
    const QDate& changeAt() const;
    const bool& enable() const;

private:
    int libraryId_;
    int deviceId_;
    QString name_;
    QDate createdAt_;
    QDate changedAt_;
    bool enable_;
};

}
}
#endif // LIBRARYMODEL_H
