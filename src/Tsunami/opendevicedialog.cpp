#include "opendevicedialog.h"
#include "ui_opendevicedialog.h"
#include "models/devicemodel.h"
#include <QString>
namespace tsunami{
OpenDeviceDialog::OpenDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDeviceDialog) {
    ui->setupUi(this);


    storage_ = db::DeviceStorage::instance();

    devicesList_ = new gui::ListItemView();
    ui->devicesListView->setModel(devicesList_);

    updateList();

    connect(ui->devicesListView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedDeviceListItem(QModelIndex)));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(clickedOpenButton()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(clickedRemoveButton()));
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(clickedCreateButton()));
}

OpenDeviceDialog::~OpenDeviceDialog()
{
    delete ui;
}

int OpenDeviceDialog::getDeviceId() {
    OpenDeviceDialog dialog(0);
    int deviceId = -1;
    if(dialog.exec() == QDialog::Accepted){
        deviceId = dialog.currentDeviceId_;
    }

    dialog.close();

    return deviceId;
}

void OpenDeviceDialog::updateList() {

    devicesList_->clear();
    QMap<QString,int> devices = storage_->listDevices(true);

    foreach(QString name,devices.keys()){
        devicesList_->addItem( name, devices.value(name) );
    }


}

void OpenDeviceDialog::showDescription(int deviceId) {
    QString desc;

    db::DeviceModel* device = storage_->openDevice(deviceId);
    desc.append( QString("<b>ID:</b> %1<br/>").arg(device->id()));
    desc.append( QString("<b>Name:</b> %1").arg(device->name()) );

    ui->deviceDescriptionText->setText( desc );

    delete device;
}

void OpenDeviceDialog::clickedDeviceListItem(const QModelIndex &index) {
    currentDeviceId_ = index.data(Qt::UserRole).toInt();

    showDescription( currentDeviceId_ );

}

void OpenDeviceDialog::clickedOpenButton() {
    accept();
}

void OpenDeviceDialog::clickedRemoveButton() {
    Q_ASSERT(false);
}

void OpenDeviceDialog::clickedCreateButton() {
    Q_ASSERT(false);
}

}
