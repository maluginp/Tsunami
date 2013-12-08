#include "CreateDeviceDialog.h"
#include "ui_CreateDeviceDialog.h"
#include <QComboBox>

namespace tsunami{
CreateDeviceDialog::CreateDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDeviceDialog)
{
    ui->setupUi(this);

    ui->deviceTypeComboBox->addItem( QString("NBJT"), "nbjt" );
    ui->deviceTypeComboBox->addItem("PBJT","pbjt");
    ui->deviceTypeComboBox->addItem("NFET","nfet");
    ui->deviceTypeComboBox->addItem("PFET","pfet");
    ui->deviceTypeComboBox->addItem("NMOS","nmos");
    ui->deviceTypeComboBox->addItem("PMOS","pmos");
    ui->deviceTypeComboBox->addItem("DIODE","diode");

    changeDeviceType(0);

    connect(ui->deviceTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDeviceType(int)));
    connect(ui->deviceModelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDeviceModel(int)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(clickedCreateButton()));

}

CreateDeviceDialog::~CreateDeviceDialog() {
    delete ui;
}

void CreateDeviceDialog::clickedCreateButton() {

    QString deviceName = ui->deviceNameLineEdit->text();
    QString deviceType = ui->deviceTypeComboBox->itemData(
                ui->deviceTypeComboBox->currentIndex() ).toString();
    QString deviceModel = ui->deviceModelComboBox->itemData(
                ui->deviceModelComboBox->currentIndex() ).toString();
    db::DeviceStorage* storage = db::DeviceStorage::instance();

    if(storage->exists( "name", deviceName )){
        ui->deviceNameErrorText->setText( tr("This is name use for other device.") );
        return;
    }



    db::DeviceModel* device;
    device->name(deviceName);
    device->model(deviceModel);
    device->type(deviceType);
    device->createAt(QDateTime::currentDateTime());
    device->changeAt(QDateTime::currentDateTime());
    device->enable(true);



    if(storage->saveDevice( device )){
        accept();
    }

    delete device;

}

void CreateDeviceDialog::changeDeviceType(int index) {
    QString device = ui->deviceTypeComboBox->itemData(index).toString();
    ui->deviceModelComboBox->clear();
    if(device == "nbjt" || device == "pbjt"){
        ui->deviceModelComboBox->addItem( tr("Gummel-Poon"), "gummelpoon" );
        ui->deviceModelComboBox->addItem( tr("VBIC"), "vbic" );
    }else if(device == "nfet" || device == "pfet"){
        ui->deviceModelComboBox->addItem( tr("Shichman-Hodges"), "shichmanhodges" );
    }else if(device == "nmos" || device == "pmos"){
        ui->deviceModelComboBox->addItem( tr("LEVEL2"), "level2" );
        ui->deviceModelComboBox->addItem( tr("LEVEL3"), "level3" );
        ui->deviceModelComboBox->addItem( tr("BSIM3v3"), "bsim3" );
        ui->deviceModelComboBox->addItem( tr("BSIM4"), "bsim4" );
    }else if(device == "diode"){
        ui->deviceModelComboBox->addItem( tr("Diode"),"diode");
    }

}

void CreateDeviceDialog::changeDeviceModel(int index) {

}
}
