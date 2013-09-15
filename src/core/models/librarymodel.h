#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H
#include <QDateTime>
class ParameterModel;

class LibraryModel {
public:
    LibraryModel();

    QString& name() const;
    int& id() const;
    int& projectId() const;
    int& userId() const;
    QDateTime& createAt() const;
    QDateTime& changeAt() const;

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
