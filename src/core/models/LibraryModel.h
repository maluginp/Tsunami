#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H
#include "defines.h"

namespace tsunami {
namespace db{

class ParameterModel;

class TSUNAMI_EXPORT LibraryModel{
//    friend class ParameterStorage;
public:
    LibraryModel();
    LibraryModel(const QString& name, int deviceId = -1);
    LibraryModel( const LibraryModel& other );
    LibraryModel& operator=(const LibraryModel& other);

    void id(int libraryId) { libraryId_ = libraryId; }
    void deviceId(int deviceId) { deviceId_ = deviceId; }
    void name(const QString& name) { name_ = name; }
    void createAt(const QDateTime& createAt) { createdAt_ = createAt; }
    void changeAt(const QDateTime& changeAt) { changedAt_ = changeAt; }
    void enable(bool enable) { enable_ = enable; }

    const int& id() const { return libraryId_; }
    const int& deviceId() const { return deviceId_; }
    const QString& name() const { return name_; }
    const QDateTime& createAt() const { return createdAt_; }
    const QDateTime& changeAt() const { return changedAt_; }
    const bool& enable() const { return enable_; }

    const ParameterModel& parameter(const QString& name) const;
    const ParameterModel& parameter(int index) const;

    ParameterModel& parameter(const QString& name);
    ParameterModel& parameter(int index);

    const QList<ParameterModel>& parameters() const;

    const ParameterModel& at(int id) const;
    ParameterModel& at(int index);

    void removeParameters(const QList<int>& indexes );

    void addParameter( const ParameterModel& parameter );
    void addParameter( const QList<ParameterModel>& parameters);
    bool parameterExists( const QString& name );
    void setParameter(const QString& name, const ParameterModel& parameter);

    void addEmptyParameter(  );

    int countParameters();

    QString debug();
protected:
    ParameterModel& find(const QString& name);
    ParameterModel& find(int index);

private:
    int libraryId_;
    int deviceId_;
    QString name_;
    QDateTime createdAt_;
    QDateTime changedAt_;
    bool enable_;

    QList<ParameterModel> parameters_;
};

}
}
#endif // LIBRARYMODEL_H
