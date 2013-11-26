#include "devicewindow.h"
#include "ui_devicewindow.h"
#include "opendevicedialog.h"
#include "models/devicemodel.h"
#include "librarywindow.h"
namespace tsunami{

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceWindow), storage_(NULL), device_(NULL) {

    ui->setupUi(this);
    storage_ = db::DeviceStorage::instance();

    libraryWindow_ = NULL;
    connect( ui->actionOpen, SIGNAL(triggered()), this, SLOT(clickedOpenDeviceAction()) );
    connect( ui->actionEditorLibrary,SIGNAL(triggered()),this,SLOT(clickedParametersEditor()));
}

DeviceWindow::~DeviceWindow() {
    delete ui;
}

void DeviceWindow::openDevice(int deviceId) {

    device_ = storage_->openDevice( deviceId );
    if(device_ == 0) Q_ASSERT(false);

    ui->deviceNameText->setText( device_->name() );
    ui->deviceTypeText->setText( device_->typeJson().toUpper() );

    QString nodeText;
    foreach(QString node, device_->nodes()){
        nodeText.append( QString("<b>%1</b><br/>").arg(node) );
    }

    ui->deviceNodesText->setText( nodeText );

}

void DeviceWindow::clickedOpenDeviceAction() {
    int deviceId = OpenDeviceDialog::getDeviceId();

    if(deviceId == -1) Q_ASSERT(false);

    openDevice(deviceId);


}

void DeviceWindow::clickedParametersEditor() {
    delete libraryWindow_;
    int deviceId = 1;
    libraryWindow_ = new LibraryWindow(deviceId,this);

    libraryWindow_->show();
}

}