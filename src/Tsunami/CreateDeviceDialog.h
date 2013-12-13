#ifndef CREATEDEVICEDIALOG_H
#define CREATEDEVICEDIALOG_H

#include <QDialog>
#include "dbstorage/DeviceStorage.h"
#include "models/DeviceModel.h"

namespace Ui {
class CreateDeviceDialog;
}
namespace tsunami{

class CreateDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDeviceDialog(QWidget *parent = 0);
    ~CreateDeviceDialog();

private:
    Ui::CreateDeviceDialog *ui;
private slots:
    void clickedCreateButton();
    void changeDeviceType(int index);
    void changeDeviceModel(int index);
};
}
#endif // CREATEDEVICEDIALOG_H
