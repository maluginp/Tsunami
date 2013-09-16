#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H
#include <QDateTime>
#include <Model.h>

class ParameterModel;

class LibraryModel : public Model {
public:
    LibraryModel();

    QString& name() const;
    int& id() const;
    int& projectId() const;
    int& userId() const;
    QDateTime& createAt() const;
    QDateTime& changeAt() const;


    QVariant parameterValue(const int& row,const int& column) const;
    QString parameterName(const int& row);
    bool setParameterValue(const int& row,const int& column, const QVariant& value);
    QList<ParameterModel> parameters();

private:
    int libraryId_;
    int projectId_;
    int userId_;
    QDateTime createAt_;
    QDateTime changeAt_;
    QString name_;

    QList<ParameterModel> parameters_;
};

#endif // LIBRARYMODEL_H
