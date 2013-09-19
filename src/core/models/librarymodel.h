#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H
#include <QDateTime>
#include <Model.h>

class ParameterModel;

class LibraryModel : public Model {
public:
    LibraryModel();

    inline const QString& name() const        { return name_;      }
    inline const int& id() const              { return libraryId_; }
    inline const int& projectId() const       { return projectId_; }
    inline const int& userId() const          { return userId_;    }
    inline const QDateTime& createAt() const  { return createAt_;  }
    inline const QDateTime& changeAt() const  { return changeAt_;  }
    inline const bool& enable() const         { return enable_;    }

    LibraryModel& setId(const int& id);
    LibraryModel& setProjectId(const int& projectId);
    LibraryModel& setUserId(const int& userId);
    LibraryModel& setCreateAt(const QDateTime& createAt);
    LibraryModel& setChangeAt(const QDateTime& changeAt);
    LibraryModel& setEnable(const bool& enable);

    void setParameter( const QString& parameter, const ParameterModel& model );

    bool parameterExists( const QString& parameter ) const;

    QVariant parameterValue(const int& row,const int& column) const;
    const QString& parameterName(const int &row) const;
    bool setParameterValue(const int& row,const int& column, const QVariant& value);

    QList<ParameterModel> parameters() const;

private:
    int libraryId_;
    int projectId_;
    int userId_;
    QDateTime createAt_;
    QDateTime changeAt_;
    QString name_;
    bool enable_;

    QList<ParameterModel> parameters_;
};

#endif // LIBRARYMODEL_H
