#ifndef PARAMETERSTORAGE_H
#define PARAMETERSTORAGE_H

#include <singleton.h>
#include "dbstorage.h"

#include "../models/librarymodel.h"
#include "../models/parametermodel.h"

namespace tsunami{
namespace db{

#define CACHE_SIZE_PARAMETER_STORAGE 10

// TODO changed parameters storage

class ParameterStorage : public DbStorage, public Singleton<ParameterStorage> {
    enum ParameterTable{
        TABLE_PARAMETERS,
        TABLE_LIBRARIES
    };
public:
    ParameterStorage();
    ~ParameterStorage();
    QString dbName() const;
    const LibraryModel& library() const;
    bool saveLibrary( );
    bool saveLibrary( const LibraryModel& library );

//    QList<ParameterModel> parameters(int libraryId);
    bool addParameter( const ParameterModel& parameter );
    LibraryModel* openLibrary( int libraryId );
    void setCurrentLibrary(  const int& libraryId );

protected:
    QString connectionName() const;
private:
    void testData();

    bool saveLibraryImpl( const LibraryModel& library );
    LibraryModel* openLibraryImpl( int libraryId );
    bool addParameterImpl( const ParameterModel& parameter );

    bool createTable( const ParameterTable& table);

    void saveCache( const LibraryModel& library ) const;

    mutable LibraryModel currentLibrary_;
    mutable QMap<int, LibraryModel> cachedLibraries_;

    static QString TABLE_NAME_PARAMETERS;
    static QString TABLE_NAME_LIBRARIES;
    static QString CONNECTION_NAME_PARAMETER;
};

}
}


#endif // PARAMETERSTORAGE_H
