#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

#include <QMainWindow>
#include <dbstorage/DbStorages.h>
#include <views/Views.h>
#include "librarywindow.h"
#include "addmeasureform.h"
#include "extractorwindow.h"
#include "AnalysisWindow.h"

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
    LibraryWindow* libraryWindow_;
    ExtractorWindow* extractorWindow_;
    addMeasureForm* measuresWindow_;
    AnalysisWindow* analysisWindow_;

    Ui::DeviceWindow *ui;
    db::DeviceStorage* storage_;
    int deviceId_;
    db::DeviceModel* device_;


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

    void selectedMeasure( const QModelIndex& index );
    void selectedAnalysis( const QModelIndex& index );
    void selectedLibrary( const QModelIndex& index );
};
}

#endif // DEVICEWINDOW_H
