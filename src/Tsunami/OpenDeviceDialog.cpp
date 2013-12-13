#include "OpenDeviceDialog.h"
#include "ui_OpenDeviceDialog.h"
#include "models/DeviceModel.h"
#include "CreateDeviceDialog.h"

namespace tsunami{
OpenDeviceDialog::OpenDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDeviceDialog) {
    ui->setupUi(this);


    storage_ = db::DeviceStorage::instance();

    devicesList_ = new gui::ListItemView();
    ui->devicesListView->setModel(devicesList_);

    updateList();

    connect(ui->devicesListView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(clickedDeviceListItem(QModelIndex)));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(clickedOpenButton()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(clickedRemoveButton()));
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(clickedCreateButton()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->deviceNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(changedDeviceName(QString)));


}

OpenDeviceDialog::~OpenDeviceDialog()
{
    delete ui;
}

int OpenDeviceDialog::getDeviceId(QWidget *parent) {
    OpenDeviceDialog dialog(parent);
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

    showDescription(-1);


}

void OpenDeviceDialog::showDescription(int deviceId) {
    QString desc;

    if( deviceId == -1 ){
        ui->deviceDescriptionText->setText(tr("Choice device"));
        return;
    }


    db::DeviceModel* device = storage_->openDevice(deviceId);
    desc.append( tr("<b>Name:</b><br/>%1<br/>").arg(device->name()) );
    desc.append( tr("<b>Model:</b><br/>%2<br/>").arg(device->model()) );
    desc.append( tr("<b>Created:</b><br/>%1<br/>").arg(device->createAt().toString("dd.MM.yyyy hh:mm")) );
    desc.append( tr("<b>Changed:</b><br/>%1<br/>").arg(device->changeAt().toString("dd.MM.yyyy hh:mm")) );
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
    if(currentDeviceId_ == -1){
        return;
    }

    QString deviceName = devicesList_->data( devicesList_->findByValue(currentDeviceId_),
                                             Qt::DisplayRole ).toString();

    int button =  QMessageBox::question(this,
                                        tr("Remove device"),
                                        tr("Do you want remove %1").arg(deviceName),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::No);
    if(button == QMessageBox::Yes){
        if(storage_->removeDevice( currentDeviceId_ )){
            updateList();
        }
    }

}


void OpenDeviceDialog::clickedCreateButton() {
    CreateDeviceDialog dialog;

    if(dialog.exec() == QDialog::Accepted){
        updateList();

    }

}

void OpenDeviceDialog::changedDeviceName(const QString &name) {
    ui->devicesListView->setCurrentIndex( devicesList_->findByKey(name) );
}

}
