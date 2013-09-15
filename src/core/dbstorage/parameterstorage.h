#ifndef PARAMETERSTORAGE_H
#define PARAMETERSTORAGE_H

#include <QtCore>
#include <singleton.h>
#include "dbstorage.h"

class LibraryModel;

#define CACHE_SIZE_PARAMETER_STORAGE 10

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
    void addLibraryImpl( const LibraryModel& library );
    LibraryModel getLibraryImpl( const int& libraryId );
    void addParameterToLibraryImpl( const ParameterModel& parameter );

    LibraryModel lastLibrary_;

    mutable QMap<int, LibraryModel> cachedLibraries_;
};

#endif // PARAMETERSTORAGE_H
