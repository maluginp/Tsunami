#ifndef PARAMETERSTORAGE_H
#define PARAMETERSTORAGE_H

#include <QtCore>
#include <singleton.h>
#include "dbstorage.h"

class LibraryModel;

#define CACHE_SIZE_PARAMETER_STORAGE 10
const QString CONNECTION_NAME_PARAMETER = QString("parameter_connection");

class ParameterStorage : public DbStorage, public Singleton<ParameterStorage> {
public:
    ParameterStorage();

    QString dbName() const;
    LibraryModel library();

    bool saveLibrary( const LibraryModel& library = LibraryModel() );
    bool addParameterToLibrary( const ParameterModel& parameter );
    LibraryModel openLibrary( const int& libraryId );
    void setCurrentLibrary(  const int& libraryId );

protected:
    QString connectionName() const;
private:
    void saveLibraryImpl( const LibraryModel& library );
    LibraryModel openLibraryImpl( const int& libraryId );
    void addParameterToLibraryImpl( const ParameterModel& parameter );

    void createTable( const ParameterTable& table);

    LibraryModel lastLibrary_;
    mutable QMap<int, LibraryModel> cachedLibraries_;

    enum ParameterTable{
        TABLE_PARAMETERS,
        TABLE_LIBRARIES
    };

    const QString TABLE_NAME_PARAMETERS;
    const QString TABLE_NAME_LIBRARIES;
};

#endif // PARAMETERSTORAGE_H
