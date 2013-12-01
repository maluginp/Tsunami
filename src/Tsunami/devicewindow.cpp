#include "devicewindow.h"
#include "ui_devicewindow.h"
#include "opendevicedialog.h"
#include "models/devicemodel.h"
#include "librarywindow.h"
#include "prepareextractordialog.h"
#include "extractor/extractorglobal.h"
#include "extractorwindow.h"
#include "../../src/MeasureData/src/addmeasureform.h"

#include "dbstorage/settingstorage.h"

namespace tsunami{

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceWindow), storage_(NULL), device_(NULL),extractorWindow_(NULL),measuresWindow_(NULL) {

    ui->setupUi(this);
    storage_ = db::DeviceStorage::instance();

    deviceId_ = 1;
    device_ = storage_->openDevice(deviceId_);

    libraryWindow_ = NULL;

    db::SettingStorage::instance();

    connect( ui->actionOpen, SIGNAL(triggered()), this, SLOT(clickedOpenDeviceAction()) );
    connect( ui->actionEditorLibrary,SIGNAL(triggered()),this,SLOT(clickedParametersEditor()));
    connect( ui->actionExtractionRun,SIGNAL(triggered()),this,SLOT(clickedExtractionRunAction()));

}

DeviceWindow::~DeviceWindow() {
    delete ui;
}

void DeviceWindow::openDevice(int deviceId) {

    device_ = storage_->openDevice( deviceId );
    if(device_ == 0) Q_ASSERT(false);
    deviceId_ = device_->id();

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

    libraryWindow_ = new LibraryWindow(deviceId_,this);

    libraryWindow_->show();
}

void DeviceWindow::clickedExtractionRunAction() {

    PrepareExtractorDialog dialog(deviceId_);

    if(dialog.exec() != QDialog::Accepted){
        return;
    }

    qDebug() << "[Run extraction] " << dialog.libraryId() << dialog.measures() << dialog.methodExtraction();

    int libraryId = dialog.libraryId();
    QList<int> measureIds = dialog.measures();
    QString optimize = dialog.methodExtraction();


    delete extractorWindow_;
    extractorWindow_ = new ExtractorWindow(device_->type(),libraryId,measureIds, optimize );
    extractorWindow_->show();
}

void DeviceWindow::clickedMeasureEditor() {

    delete measuresWindow_;
//    measuresWindow_ = new tsunami::addMeasureForm(tsunami::addMeasureForm::EDIT,1,0);
//    measuresWindow_->show();
}

}
