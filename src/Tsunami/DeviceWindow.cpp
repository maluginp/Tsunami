#include "DeviceWindow.h"
#include "ui_DeviceWindow.h"
#include "OpenDeviceDialog.h"
#include "PrepareExtractorDialog.h"
#include "OpenMeasureDialog.h"
#include "ChoiceAnalysisForm.h"
#include "Log.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"

namespace tsunami{

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
     ui(new Ui::DeviceWindow),
     storage_(NULL),
     device_(NULL),
     libraryWindow_(NULL),
     extractorWindow_(NULL),
     measuresWindow_(NULL),
     analysisWindow_(NULL),
     measureList_(NULL),
     libraryList_(NULL),
     analysisList_(NULL)  {

    // Load Translator
    translator_ = new QTranslator();
    if( translator_->load(":/i18n/tsunami_ru") ){
        qApp->installTranslator( translator_ );
    }else{
        delete translator_;
        translator_ = 0;
    }

    log::logDebug() << "Tsunami is started";

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
    connect( ui->runExtractButton,SIGNAL(clicked()),this,SLOT(clickedExtractionRunAction()));
    connect( ui->actionEditMeasure,SIGNAL(triggered()),this,SLOT(clickedMeasureEditor()));
    connect( ui->actionAddMeasure,SIGNAL(triggered()),this,SLOT(clickedMeasureAdd()));
    connect( ui->actionDeviceClose,SIGNAL(triggered()),this,SLOT(clickedDeviceClose()));
    connect( ui->actionDeviceRemove,SIGNAL(triggered()),this,SLOT(clickedDeviceRemove()));
    connect( ui->actionSettingsOpen,SIGNAL(triggered()),this,SLOT(clickedSettingsOpen()));
    connect( ui->actionAboutOpen,SIGNAL(triggered()),this,SLOT(clickedAboutOpen()));

    connect( ui->addAnalysisButton,SIGNAL(clicked()),this,SLOT(clickedAnalysisAdd()) );
    connect( ui->addLibraryButton,SIGNAL(clicked()),this,SLOT(clickedLibraryAdd()));
    connect( ui->addMeasureButton,SIGNAL(clicked()),this,SLOT(clickedMeasureAdd()) );

    connect( ui->libraryTreeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectedLibrary(QModelIndex)));
    connect( ui->datasetTreeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectedMeasure(QModelIndex)));
    connect( ui->analysisTreeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectedAnalysis(QModelIndex)));

    statusBar()->showMessage(tr("Tsunami ver %1").arg( TSUNAMI_VERSION ));

//#ifdef QT_DEBUG
//    openDevice(1);
//#else
    clickedDeviceClose();
//#endif
}

DeviceWindow::~DeviceWindow() {
    delete ui;

    if( translator_ != 0 ){
        delete translator_;
    }
}

void DeviceWindow::openDevice(int deviceId) {
    device_ = storage_->openDevice( deviceId );
    if(device_ == 0) Q_ASSERT(false);
    deviceId_ = device_->id();

    ui->deviceNameText->setText( device_->name() );
    ui->deviceTypeText->setText( device_->typeJson().toUpper() );

    updateDeviceWindow();

    ui->deviceModelText->setText(  db::DeviceModel::modelNameToTitle( device_->model() ) );
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

bool DeviceWindow::createLibraryWindow() {
    if(libraryWindow_){
        libraryWindow_->disconnect();
        delete libraryWindow_;
    }

    libraryWindow_ = 0;
    libraryWindow_ = new LibraryWindow(deviceId_);
    libraryWindow_->show();

    connect(libraryWindow_,SIGNAL(updatedDataBase()),
            SLOT(updateDeviceWindow()));
    return (libraryWindow_ != 0);
}

bool DeviceWindow::createAnalysisWindow() {
    if(analysisWindow_) {
        analysisWindow_->disconnect();
        delete analysisWindow_;
        analysisWindow_ = 0;
    }

    analysisWindow_ = new AnalysisWindow( deviceId_);
    analysisWindow_->show();
    QEventLoop eventLoop;
    connect(analysisWindow_,SIGNAL(pageLoadFinished()),&eventLoop,SLOT(quit()));
    eventLoop.exec();

    connect(analysisWindow_,SIGNAL(updatedDataBase()),
            SLOT(updateDeviceWindow()));
    return (analysisWindow_ != 0);
}

bool DeviceWindow::createMeasureWindow(addMeasureForm::Action action, int measureId) {
    if(measuresWindow_){
        measuresWindow_->disconnect();
        delete measuresWindow_;
        measuresWindow_ = 0;
    }
    measuresWindow_ = new addMeasureForm(action,measureId);
    measuresWindow_->show();
    connect(measuresWindow_,SIGNAL(updatedDataBase()),
            SLOT(updateDeviceWindow()));

    return (measuresWindow_ != 0);
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
    ui->datasetTreeView->expandAll();

    analysisList_->clear();
    db::AnalysisStorage* analysisStorage = db::AnalysisStorage::instance();
    QList<db::AnalysisModel*> analyses = analysisStorage->getAnalysesByDeviceId( deviceId_ );
    foreach( db::AnalysisModel* analysis, analyses ){
        analysisList_->addChild( analysis->typeJson(), analysis->name(), analysis->id() );
    }
    qDeleteAll( analyses );
    ui->analysisTreeView->expandAll();

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

    if(deviceId != -1){
        openDevice(deviceId);
    }

    return;
}

void DeviceWindow::clickedParametersEditor() {
    createLibraryWindow();
    return;
}
void DeviceWindow::clickedExtractionRunAction() {
    if(db::ParameterStorage::instance()->numberLibraries(deviceId_) > 0
            && db::MeasureStorage::instance()->numberMeasures(deviceId_) > 0 ) {

        PrepareExtractorDialog dialog(deviceId_);

        if(dialog.exec() != QDialog::Accepted){
            return;
        }

        int libraryId = dialog.libraryId();
        QList<int> measureIds = dialog.measures();
        db::SettingStorage* settings = db::SettingStorage::instance();
        QString optimize = settings->value("optimize/method").toString();


        delete extractorWindow_;
        extractorWindow_ = new ExtractorWindow(device_->type(),libraryId,measureIds, optimize );


        extractorWindow_->show();
    }else{
        QMessageBox::information(this,windowTitle(),tr("Not enough measures or "
                                                       "libraries for extraction"));
    }
    return;
}

void DeviceWindow::clickedMeasureEditor() {
    int measureId = OpenMeasureDialog::getMeasureId( deviceId_, this );

    if(measureId != -1) {
        createMeasureWindow(addMeasureForm::EDIT,measureId);
    }

    return;
}

void DeviceWindow::clickedMeasureAdd() {
    db::AnalysisStorage* storage = db::AnalysisStorage::instance();

    if (storage->numberAnalysis(deviceId_) == 0) {
        QMessageBox::information(this,windowTitle(),tr("Analysis null"));
        return;
    }

    int analysisId = ChoiceAnalysisForm::getAnalysisId( deviceId_);

    if( analysisId != -1){
        createMeasureWindow(addMeasureForm::NEW,analysisId);
    }

    return;
}

void DeviceWindow::clickedAnalysisAdd() {
    if(createAnalysisWindow()){
        analysisWindow_->openAnalysis( -1 );
    }
}

void DeviceWindow::clickedLibraryAdd() {
//    clickedParametersEditor();
    if(createLibraryWindow()) {
        libraryWindow_->openLibrary(-1);
        libraryWindow_->clickedNewLibraryAction();
    }
}

void DeviceWindow::clickedDeviceClose() {
    if(device_ != 0){
        device_->changeAt( QDateTime::currentDateTime() );
        if(!storage_->saveDevice( device_ )){
            log::logError() << "Can not save device";
        }
        deviceId_ = -1;
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

void DeviceWindow::clickedDeviceNew() {
//    OpenDeviceDialog dialog;
//    dialog.clickedCreateButton();
//    if(dialog.exec() == QDialog::Accepted){
//    }
}

void DeviceWindow::clickedDeviceRemove() {
    if(device_ == 0 || deviceId_ == -1){
        return;
    }

    int button =  QMessageBox::question(this,
                                        tr("Remove device"),
                                        tr("Do you want remove %1")
                                        .arg(device_->name()),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::No);
    if (button == QMessageBox::Yes) {
        if (storage_->removeDevice( deviceId_ )) {
            clickedDeviceClose();

            delete device_;
            device_ = 0;

        }
    }

}

void DeviceWindow::clickedSettingsOpen() {
    SettingsDialog settings;
    settings.exec();
}

void DeviceWindow::clickedAboutOpen() {
    AboutDialog dialog;
    dialog.exec();
}

void DeviceWindow::selectedMeasure(const QModelIndex &index) {
    bool ok;
    int measureId =  index.data(Qt::UserRole).toInt(&ok);
    if(!ok || measureId == -1){
        return;
    }

    createMeasureWindow(addMeasureForm::EDIT, measureId);
    return;
}

void DeviceWindow::selectedAnalysis(const QModelIndex &index) {
    bool ok;
    int analysisId = index.data( Qt::UserRole ).toInt(&ok);
    if(!ok || analysisId == -1){
        return;
    }

    if(createAnalysisWindow()){
        analysisWindow_->openAnalysis( analysisId );
    }
}

void DeviceWindow::selectedLibrary(const QModelIndex &index) {
    bool ok;
    int libraryId = index.data(Qt::UserRole).toInt(&ok);
    if(!ok || libraryId == -1){
        return;
    }

    if(createLibraryWindow()){
        libraryWindow_->openLibrary( libraryId );
    }
    return;
}

}
