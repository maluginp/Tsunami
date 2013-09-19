#ifndef PARAMETERSTORAGE_H
#define PARAMETERSTORAGE_H


#include <singleton.h>
#include "dbstorage.h"

#include "../models/librarymodel.h"
#include "../models/parametermodel.h"

#define CACHE_SIZE_PARAMETER_STORAGE 10
class ParameterStorage : public DbStorage, public Singleton<ParameterStorage> {
    enum ParameterTable{
        TABLE_PARAMETERS,
        TABLE_LIBRARIES
    };
public:
    ParameterStorage();

    QString dbName() const;
    LibraryModel library();
    bool saveLibrary( );
    bool saveLibrary( const LibraryModel& library );
    bool addParameterToLibrary( const ParameterModel& parameter );
    LibraryModel openLibrary( const int& libraryId ) const;
    void setCurrentLibrary(  const int& libraryId ) const;

protected:
    QString connectionName() const;
private:


    bool saveLibraryImpl( const LibraryModel& library );
    LibraryModel openLibraryImpl( const int& libraryId ) const;
    void addParameterToLibraryImpl( const ParameterModel& parameter ) const;

    bool createTable( const ParameterTable& table);

    void saveToCache( const LibraryModel& library );

    LibraryModel lastLibrary_;
    mutable QMap<int, LibraryModel> cachedLibraries_;

    static QString TABLE_NAME_PARAMETERS;
    static QString TABLE_NAME_LIBRARIES;
    static QString CONNECTION_NAME_PARAMETER;
};

#endif // PARAMETERSTORAGE_H
