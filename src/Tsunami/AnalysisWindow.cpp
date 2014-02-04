#include "AnalysisWindow.h"
#include "ui_AnalysisWindow.h"
#include "webkit/WebKit.h"
#include <QComboBox>
#include <QLineEdit>
#include "dbstorage/DbStorages.h"

#include "webkit/APIObject.h"
#include "Log.h"
namespace tsunami{
AnalysisWindow::AnalysisWindow(int deviceId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow),
    listAnalysis_(0),api_(0),
    deviceId_(deviceId)
{
    ui->setupUi(this);


    api_ = new APIObject(deviceId_);
    storage_ = db::AnalysisStorage::instance();
//    listAnalysis_ = new gui::ListItemView();
//    ui->analysisItemsListView->setModel( listAnalysis_ );

    ui->analysisTypeComboBox->addItem( "AC", "ac" );
    ui->analysisTypeComboBox->addItem( "DC", "dc" );
    ui->analysisTypeComboBox->addItem( "TRAN", "tran" );
//    connect(ui->webView,SIGNAL(loadStarted()),t   his,SLOT(loadStarted()));
//    loadStarted();
//    connect(ui->webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(loadStarted()) );

//    db::DeviceStorage* deviceStorage = db::DeviceStorage::instance();

//    db::DeviceModel* device = deviceStorage->openDevice( deviceId_ );

//    QString url = "html/";
//    switch(device->type()){
//    case DEVICE_NBJT:
//    case DEVICE_PBJT:
//        url.append("bjt.html"); break;
//    case DEVICE_PFET:
//    case DEVICE_NFET:
//        url.append("jfet.html"); break;
//    case DEVICE_PMOS:
//    case DEVICE_NMOS:
//        url.append("mosfet.html"); break;
//    case DEVICE_CAPACITOR:
//    case DEVICE_RESISTOR:
//    case DEVICE_DIODE:
//        Q_ASSERT(false); break;
//    case DEVICE_VSOURCE:
//    case DEVICE_ISOURCE:
//        Q_ASSERT(false); break;
//    }

//    ui->webView->load( QUrl(url) );
//    ui->webView->setFocus();

//    updateAnalysisList();
//    connect(ui->addButton,SIGNAL(clicked()),
//                     this,SLOT(clickedCreateAnalysis()));
//    connect(ui->openButton,SIGNAL(clicked()),
//                      this,SLOT(clickedOpenAnalysis()));
//    connect(ui->webView,SIGNAL(loadFinished(bool)),
//                   this,SLOT(loadFinished(bool)));
//    connect(ui->analysisItemsListView,SIGNAL(doubleClicked(QModelIndex)),
//                                 this,SLOT(selectedAnalysisItem(QModelIndex)));

}

AnalysisWindow::~AnalysisWindow() {
    delete ui;
}

void AnalysisWindow::updateAnalysisList() {
    QMap<int,QString> list = storage_->listAnalysis(deviceId_);

    listAnalysis_->clear();

    foreach(int analysisId,list.keys()){
        listAnalysis_->addItem( list.value(analysisId), QVariant(analysisId) );
    }

    emit updatedDataBase();
}

void AnalysisWindow::clickedOpenAnalysis() {
    if( analysisId_ != -1 ){
        openAnalysis(analysisId_);
    }
}

void AnalysisWindow::clickedSaveAnalysis(const QList<tsunami::Source> &sources) {
    log::logTrace() << "Saving analysis";
    QString analysisName = ui->analysisNameLineEdit->text();
    if(analysisName.isEmpty()){
        return;
    }


//    if(storage_->exists( deviceId_, analysisName )){
//        // TODO: show message if analysis exists
//        QMessageBox::warning(this,windowTitle(),tr("Analysis is existed"));
//        log::logDebug() << "Analysis existed";
//        return;
//    }

    currentAnalysis_->sources( sources );

    currentAnalysis_->name( analysisName );
    currentAnalysis_->type( ui->analysisTypeComboBox->itemData( ui->analysisTypeComboBox->currentIndex() ).toString() );
    currentAnalysis_->enable( ui->analysisEnableCheckBox->checkState() == Qt::Checked );

    int analysisId = currentAnalysis_->id();

    if(storage_->saveAnalysis( currentAnalysis_ )){
        log::logTrace() << QString("Analysis #%1 has saved ")
                           .arg(currentAnalysis_->id());
        updateAnalysisList();
        if(analysisId == -1){
            openAnalysis(currentAnalysis_->id());
        }
    }else{
        log::logDebug() << "Analysis has not saved. Sql error: "
                        << storage_->lastError();
    }
}

void AnalysisWindow::clickedCreateAnalysis() {
    log::logTrace() << "Creating analysis";
    currentAnalysis_ = new db::AnalysisModel();
    currentAnalysis_->deviceId( deviceId_ );

    ui->analysisNameLineEdit->setText("");
    ui->analysisTypeComboBox->setCurrentIndex(0);
    ui->analysisEnableCheckBox->setCheckState(Qt::Unchecked);

    api_->openAnalysis( currentAnalysis_ );

}

void AnalysisWindow::selectedAnalysisItem(const QModelIndex &index) {
    bool ok;
    int analysisId = index.data(Qt::UserRole).toInt(&ok);
    if(!ok || analysisId == -1){
        return;
    }

    openAnalysis( analysisId );

}

void AnalysisWindow::openAnalysis(int analysisId) {
    log::logTrace() << QString("Opening analysis %1").arg(analysisId);
    if(analysisId == -1){
        clickedCreateAnalysis();
        return;
    }

    analysisId_ = analysisId;

    currentAnalysis_ = storage_->openAnalysis( analysisId );

    if(currentAnalysis_ == 0) return;

    int typeIndex = ui->analysisTypeComboBox->findData( currentAnalysis_->typeJson() );
    ui->analysisTypeComboBox->setCurrentIndex(typeIndex);

    ui->analysisNameLineEdit->setText( currentAnalysis_->name() );

    if(currentAnalysis_->enable()){
        ui->analysisEnableCheckBox->setCheckState(Qt::Checked);
    }else{
        ui->analysisEnableCheckBox->setCheckState(Qt::Unchecked);
    }

    api_->openAnalysis( currentAnalysis_ );
}

void AnalysisWindow::loadStarted() {
    if(!api_){
        log::logError() << "API is not initialized";
        return;
    }
    api_->disconnect( );
    connect(api_, SIGNAL(savedAnalysis(QList<tsunami::Source>)),
            this, SLOT(clickedSaveAnalysis(QList<tsunami::Source>)));

    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("Api",api_);
}

void AnalysisWindow::loadFinished(bool) {
    emit pageLoadFinished();
}

}
