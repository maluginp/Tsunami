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

    bool saveLibrary( LibraryModel* library );

    LibraryModel* openLibrary( int libraryId );


protected:
    QString connectionName() const;
private:
    void testData();

    bool saveLibraryImpl( LibraryModel* library );
    LibraryModel* openLibraryImpl( int libraryId );

    bool createTable( const ParameterTable& table);

    static QString TABLE_NAME_PARAMETERS;
    static QString TABLE_NAME_LIBRARIES;
    static QString CONNECTION_NAME_PARAMETER;
};

}
}


#endif // PARAMETERSTORAGE_H
