#ifndef PARAMETERSTORAGE_H
#define PARAMETERSTORAGE_H
#include "DbStorage.h"
#include <singleton.h>
#include "models/LibraryModel.h"
#include "models/LibraryTemplateModel.h"
#include "models/ParameterModel.h"

namespace tsunami{
namespace db{

#define CACHE_SIZE_PARAMETER_STORAGE 10

class TSUNAMI_EXPORT ParameterStorage : public DbStorage, public Singleton<ParameterStorage> {
    enum ParameterTable{
        TABLE_PARAMETERS,
        TABLE_LIBRARIES,
        TABLE_TEMPLATE_LIBRARIES
    };
public:
    ParameterStorage();
    ~ParameterStorage();
    QString dbName() const;
    const LibraryModel& library() const;

    bool saveLibrary( LibraryModel* library );
    bool saveTemplateLibrary( LibraryTemplateModel* templateLibrary );

    LibraryModel* openLibrary( int libraryId );

    QMap<int,QString> listLibraries(int deviceId);
    QList<LibraryModel*> getLibrariesByDeviceId( int deviceId );
    QList<LibraryModel*> getTemplateLibrariesByDeviceType( DeviceType type);
    LibraryModel* openTemplateLibrary( const QString& templateName );

    bool removeLibrary( int libraryId );
    bool exists( const QString& name );
protected:
    QString connectionName() const;
    int lastInsertId(const QString& table);
private:
    void testData();
    void syncParameters(LibraryModel* library);
    QMap<int,QString> listLibrariesImpl(int deviceId);
    bool saveLibraryImpl( LibraryModel* library );
    LibraryModel* openLibraryImpl( int libraryId );
    QList<LibraryModel*> getLibrariesByDeviceIdImpl( int deviceId );
    LibraryTemplateModel* openTemplateLibraryImpl( const QString& templateName );
    bool removeLibraryImpl( int libraryId );
    bool saveTemplateLibraryImpl( LibraryTemplateModel* templateLibrary );

    bool createTable( const ParameterTable& table);

    void syncTemplateDirectory();

    static QString TABLE_NAME_PARAMETERS;
    static QString TABLE_NAME_LIBRARIES;
    static QString TABLE_NAME_TEMPLATE_LIBRARIES;
    static QString CONNECTION_NAME_PARAMETER;
};

}
}


#endif // PARAMETERSTORAGE_H
