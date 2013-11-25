#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

#include <QMainWindow>
#include <dbstorage/devicestorage.h>

namespace Ui {
class DeviceWindow;
}
namespace tsunami{

class DeviceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeviceWindow(QWidget *parent = 0);
    ~DeviceWindow();

    void openDevice( int deviceId );
private:
    Ui::DeviceWindow *ui;
    db::DeviceStorage* storage_;

    db::DeviceModel* device_;
private slots:
    void clickedOpenDeviceAction();

};
}

#endif // DEVICEWINDOW_H
