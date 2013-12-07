#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

#include <QMainWindow>
#include <dbstorage/DbStorages.h>
#include <views/Views.h>

namespace Ui {
class DeviceWindow;
}
namespace tsunami{
class LibraryWindow;
class ExtractorWindow;
class addMeasureForm;
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
};
}

#endif // DEVICEWINDOW_H
