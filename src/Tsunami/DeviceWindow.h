#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

#include <dbstorage/DbStorages.h>
#include <views/Views.h>
#include "LibraryWindow.h"
#include "AddMeasureForm.h"
#include "ExtractorWindow.h"
#include "AnalysisWindow.h"
#include "defines.h"

namespace Ui {
class DeviceWindow;
}
namespace tsunami{


class DeviceWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit DeviceWindow(QWidget *parent = 0);
    ~DeviceWindow();

    void openDevice( int deviceId );
private:
    Ui::DeviceWindow *ui;

    db::DeviceStorage* storage_;
    db::DeviceModel* device_;

    LibraryWindow* libraryWindow_;
    ExtractorWindow* extractorWindow_;
    addMeasureForm* measuresWindow_;
    AnalysisWindow* analysisWindow_;

    int deviceId_;

    QTranslator* translator_;


    gui::ListTreeView* measureList_;
    gui::ListItemView* libraryList_;
    gui::ListTreeView* analysisList_;

private slots:
    void updateDeviceWindow();

    void clickedOpenDeviceAction();
    void clickedParametersEditor();
    void clickedExtractionRunAction();
    void clickedMeasureEditor();
    void clickedMeasureAdd();
    void clickedAnalysisAdd();
    void clickedLibraryAdd();
    void clickedDeviceClose();
    void clickedDeviceNew();
    void clickedDeviceRemove();
    void clickedSettingsOpen();
    void clickedAboutOpen();

    void selectedMeasure( const QModelIndex& index );
    void selectedAnalysis( const QModelIndex& index );
    void selectedLibrary( const QModelIndex& index );
};
}

#endif // DEVICEWINDOW_H
