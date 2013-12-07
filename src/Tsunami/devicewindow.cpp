#include "devicewindow.h"
#include "ui_devicewindow.h"
#include "opendevicedialog.h"
#include "prepareextractordialog.h"
#include "OpenMeasureDialog.h"
#include "choiceanalysisform.h"
#include <logger.h>
#include "defines.h"
#include <QTreeView>
#include <QMessageBox>

namespace tsunami{

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceWindow), storage_(NULL), device_(NULL),extractorWindow_(NULL),measuresWindow_(NULL),
    measureList_(NULL),libraryList_(NULL),analysisList_(NULL), analysisWindow_(NULL) {

    ui->setupUi(this);
    storage_ = db::DeviceStorage::instance();
    ui->deviceImage->hide();

    libraryWindow_ = NULL;

    db::SettingStorage::instance();

    QVariantMap measureTypes;
    measureTypes.insert("AC","ac");
    measureTypes.insert("DC","dc");
    measureTypes.insert("TRAN","tran");

    measureList_ = new gui::ListTreeView( tr("Measures"),  measureTypes );
    libraryList_ = new gui::ListItemView( tr("Libraries"));
    analysisList_ = new gui::ListTreeView( tr("Analyses"), measureTypes );

    ui->datasetTreeView->setModel( measureList_ );
    ui->libraryTreeView->setModel( libraryList_ );
    ui->analysisTreeView->setModel( analysisList_ );

    connect( ui->actionDeviceOpen, SIGNAL(triggered()), this, SLOT(clickedOpenDeviceAction()) );
    connect( ui->actionEditorLibrary,SIGNAL(triggered()),this,SLOT(clickedParametersEditor()));
    connect( ui->actionExtractionRun,SIGNAL(triggered()),this,SLOT(clickedExtractionRunAction()));
    connect( ui->actionEditMeasure,SIGNAL(triggered()),this,SLOT(clickedMeasureEditor()));
    connect( ui->actionAddMeasure,SIGNAL(triggered()),this,SLOT(clickedMeasureAdd()));
    connect( ui->actionDeviceClose,SIGNAL(triggered()),this,SLOT(clickedDeviceClose()));
    connect( ui->actionDeviceRemove,SIGNAL(triggered()),this,SLOT(clickedDeviceRemove()));

    connect( ui->addAnalysisButton,SIGNAL(clicked()),this,SLOT(clickedAnalysisAdd()) );
    connect( ui->addLibraryButton,SIGNAL(clicked()),this,SLOT(clickedLibraryAdd()));
    connect( ui->addMeasureButton,SIGNAL(clicked()),this,SLOT(clickedMeasureAdd()) );

    connect( ui->libraryTreeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectedLibrary(QModelIndex)));
    connect( ui->datasetTreeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectedMeasure(QModelIndex)));
    connect( ui->analysisTreeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectedMeasure(QModelIndex)));

    statusBar()->showMessage("Tsunami ver0.2");

    openDevice(1);
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

    updateDeviceWindow();

    setWindowTitle( tr("%1 - Devices Manager").arg(device_->name()) );

    QPixmap deviceImage;

    switch( device_->type() ){
    case DEVICE_NBJT: deviceImage.load(":/images/NBJT" ); break;
    case DEVICE_PBJT: deviceImage.load(":/images/PBJT" ); break;
    case DEVICE_NFET: deviceImage.load(":/images/NFET" ); break;
    case DEVICE_PFET: deviceImage.load(":/images/PFET" ); break;
    case DEVICE_NMOS: deviceImage.load(":/images/NMOS" ); break;
    case DEVICE_PMOS: deviceImage.load(":/images/PMOS" ); break;
    default:
        deviceImage.load(":/images/NONE_DEVICE");
    }

    ui->deviceImage->setPixmap( deviceImage );
    ui->deviceImage->show();

    ui->addAnalysisButton->setEnabled(true);
    ui->addLibraryButton->setEnabled(true);
    ui->addMeasureButton->setEnabled(true);
}

void DeviceWindow::updateDeviceWindow() {
    if( device_ == 0) return;

    // Updates datasets

    db::MeasureStorage* measureStorage = db::MeasureStorage::instance();
    measureList_->clear();
    QList<db::MeasureModel*> measures = measureStorage->getMeasuresByDeviceId(deviceId_);
    foreach( db::MeasureModel* measure, measures ){
        measureList_->addChild( measure->typeJson() ,measure->name(),
                                measure->id() );
    }
    qDeleteAll( measures );

    analysisList_->clear();
    db::AnalysisStorage* analysisStorage = db::AnalysisStorage::instance();
    QList<db::AnalysisModel*> analyses = analysisStorage->getAnalysesByDeviceId( deviceId_ );
    foreach( db::AnalysisModel* analysis, analyses ){
        analysisList_->addChild( analysis->typeJson(), analysis->name(), analysis->id() );
    }

    qDeleteAll( analyses );

    libraryList_->clear();
    db::ParameterStorage* parameterStorage = db::ParameterStorage::instance();
    QList<db::LibraryModel*> libraries = parameterStorage->getLibrariesByDeviceId( deviceId_ );
    foreach(db::LibraryModel* library, libraries){
        libraryList_->addItem( library->name(), library->id() );
    }

    qDeleteAll( libraries );

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
    int measureId = OpenMeasureDialog::getMeasureId( deviceId_ );

    if(measureId != -1){
        delete measuresWindow_;

        measuresWindow_ = new tsunami::addMeasureForm(tsunami::addMeasureForm::EDIT,1,0);
        measuresWindow_->show();
    }
}

void DeviceWindow::clickedMeasureAdd() {
    int analysisId = 1;
    //    int analysisId = tsunami::ChoiceAnalysisForm::getAnalysisId( deviceId_ );

    if( analysisId != -1){
        delete measuresWindow_;
        measuresWindow_ = new tsunami::addMeasureForm(tsunami::addMeasureForm::NEW,analysisId,0);
        measuresWindow_->show();
    }
}

void DeviceWindow::clickedAnalysisAdd() {
    delete analysisWindow_;
    analysisWindow_ = new AnalysisWindow( deviceId_ );
    analysisWindow_->show();

}

void DeviceWindow::clickedLibraryAdd() {
    clickedParametersEditor();
}

void DeviceWindow::clickedDeviceClose() {

    if(device_ != 0){
        device_->changeAt( QDateTime::currentDateTime() );
        if(!storage_->saveDevice( device_ )){
            Q_ASSERT(false);
        }
        deviceId_;
        delete device_;
        measureList_->clear();
        libraryList_->clear();
        analysisList_->clear();
    }
    ui->deviceNameText->setText( tr("Choice device") );
    ui->deviceTypeText->setText( tr("Choice device") );
    ui->deviceModelText->setText( tr("Choice device") );

    ui->addAnalysisButton->setEnabled(false);
    ui->addLibraryButton->setEnabled(false);
    ui->addMeasureButton->setEnabled(false);

    ui->deviceImage->hide();
}

void DeviceWindow::clickedDeviceRemove() {
    if(device_ == 0 || deviceId_ == -1){
        return;
    }

    int button =  QMessageBox::question(this,
                                        tr("Remove device"),
                                        tr("Do you want remove %1").arg(device_->name()),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::No);
    if(button == QMessageBox::Yes){
        if(storage_->removeDevice( deviceId_ )){
            clickedDeviceClose();

            delete device_;
            device_ = 0;

        }
    }

}

void DeviceWindow::selectedMeasure(const QModelIndex &index) {
    bool ok;
    int measureId =  index.data(Qt::UserRole).toInt(&ok);
    if(!ok || measureId == -1){
        return;
    }
    delete measuresWindow_;
    measuresWindow_ = new addMeasureForm( addMeasureForm::EDIT, measureId );
    measuresWindow_->show();
}

void DeviceWindow::selectedAnalysis(const QModelIndex &index) {
    bool ok;
    int analysisId = index.data( Qt::UserRole ).toInt(&ok);
    if(!ok || analysisId == -1){
        return;
    }
    delete analysisWindow_;
    analysisWindow_ = new AnalysisWindow(deviceId_);
    analysisWindow_->openAnalysis( analysisId );
    analysisWindow_->show();

}

void DeviceWindow::selectedLibrary(const QModelIndex &index) {
    bool ok;
    int libraryId = index.data(Qt::UserRole).toInt(&ok);
    if(!ok || libraryId == -1){
        return;
    }
    delete libraryWindow_;
    libraryWindow_ = new LibraryWindow(deviceId_);
    libraryWindow_->openLibrary( libraryId );
    libraryWindow_->show();
}

}
