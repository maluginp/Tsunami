#include "CreateDeviceDialog.h"
#include "ui_CreateDeviceDialog.h"
#include <QComboBox>

namespace tsunami{
CreateDeviceDialog::CreateDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDeviceDialog)
{
    ui->setupUi(this);


    ui->deviceNameErrorText->setText("");

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
    db::DeviceStorage* storage = db::DeviceStorage::instance();
    ui->deviceNameErrorText->setText("");

    QString deviceName = ui->deviceNameLineEdit->text();
    if(deviceName.isEmpty()){
        ui->deviceNameErrorText->setText(tr("Device name is empty"));
        return;
    }
    if(storage->existsByKey( "name", deviceName )){
        ui->deviceNameErrorText->setText( tr("This is name use for other device.") );
        return;
    }

    if(ui->deviceTypeComboBox->currentIndex() == -1){
        ui->deviceNameErrorText->setText( tr("Not choiced device type") );
        return;
    }
    QString deviceType = ui->deviceTypeComboBox->itemData(
                ui->deviceTypeComboBox->currentIndex() ).toString();

    if(ui->deviceModelComboBox->currentIndex() == -1){
        ui->deviceNameErrorText->setText( tr("Not choiced device model") );
        return;
    }
    QString deviceModel = ui->deviceModelComboBox->itemData(
                ui->deviceModelComboBox->currentIndex() ).toString();


    db::DeviceModel* device = new db::DeviceModel();
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
