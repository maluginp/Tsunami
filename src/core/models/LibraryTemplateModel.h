#ifndef LIBRARYTEMPLATEMODEL_H
#define LIBRARYTEMPLATEMODEL_H

#include "defines.h"

namespace tsunami {
namespace db{

class ParameterModel;
class LibraryModel;
/*!
 * \brief The LibraryTemplateModel class
 *
 * Model for template library. Example: BSIM3, LEVEL1, GUMMEL-POON
 */
class TSUNAMI_EXPORT LibraryTemplateModel {
public:
    LibraryTemplateModel();

    void name(const QString& name) { name_ = name; }
    void devices(const QString& devicesJson);
    void addDevice( const QString& deviceName );
    void addDevice( const QStringList& deviceNames );
//    void devices( const QList<DeviceType>& devices ) { devices_ = devices; }
    void addDevice( DeviceType device );
    void parameters(const QString& parametersJson );
    void addParameter( const ParameterModel& parameter );

    const QString& name() const { return name_; }
    QString devicesJson();
    QString parametersJson();

    LibraryModel* convertToLibraryModel();

    bool satisfyDevice( const QString& device);
    bool satisfyDevice( DeviceType device );

    static QList<LibraryTemplateModel*> parseDirectory();

    static QString TEMPLATE_EXTENSION;
    static QString TEMPLATE_DIRECTORY;
private:

    QVariant checkInfinity( double value );

    QString name_;
    QList<DeviceType> devices_;
    QList<ParameterModel> parameters_;
};

}
}

#endif // LIBRARYTEMPLATEMODEL_H
