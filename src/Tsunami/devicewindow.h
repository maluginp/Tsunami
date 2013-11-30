#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

#include <QMainWindow>
#include <dbstorage/devicestorage.h>

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
private slots:

    void clickedOpenDeviceAction();
    void clickedParametersEditor();
    void clickedExtractionRunAction();
    void clickedMeasureEditor();
};
}

#endif // DEVICEWINDOW_H
