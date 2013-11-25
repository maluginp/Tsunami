#ifndef OPENDEVICEDIALOG_H
#define OPENDEVICEDIALOG_H

#include <QDialog>
#include <views/listitemview.h>
#include <dbstorage/devicestorage.h>

namespace Ui {
class OpenDeviceDialog;
}
namespace tsunami{

class OpenDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDeviceDialog(QWidget *parent = 0);
    ~OpenDeviceDialog();

    static int getDeviceId();
private:
    void updateList();
    void showDescription(int deviceId);
    Ui::OpenDeviceDialog *ui;
    gui::ListItemView* devicesList_;

    int currentDeviceId_;
    db::DeviceStorage* storage_;
private slots:
    void clickedDeviceListItem(const QModelIndex& index);
    void clickedOpenButton();
    void clickedRemoveButton();
    void clickedCreateButton();
};
}

#endif // OPENDEVICEDIALOG_H
