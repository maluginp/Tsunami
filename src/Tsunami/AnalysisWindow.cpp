#include "AnalysisWindow.h"
#include "ui_AnalysisWindow.h"
#include "webkit/WebKit.h"
#include <QComboBox>
#include <QLineEdit>
#include "dbstorage/DbStorages.h"

#include "spice/Devices.h"

#include "webkit/APIObject.h"
#include "Log.h"
namespace tsunami{
using namespace spice;

AnalysisWindow::AnalysisWindow(int deviceId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow),
    listAnalysis_(0),api_(0),
    deviceId_(deviceId),
    sourceConfigurationView_(0)
{
    ui->setupUi(this);
    // Test data
    nodes_ << "D" << "G" << "S" << "B";


    api_ = new APIObject(deviceId_);
    storage_ = db::AnalysisStorage::instance();
//    listAnalysis_ = new gui::ListItemView();
//    ui->analysisItemsListView->setModel( listAnalysis_ );

    // Заполняем типы анализов
    ui->analysisTypeComboBox->addItem( "AC", "ac" );
    ui->analysisTypeComboBox->addItem( "DC", "dc" );
    ui->analysisTypeComboBox->addItem( "TRAN", "tran" );

    ui->sourceNodeComboBox->addItem("","");
    foreach(QString node, nodes_){
        ui->sourceFirstNodeComboBox->addItem( node, node );
        ui->sourceSecondNodeComboBox->addItem( node, node);
        ui->sourceNodeComboBox->addItem(node,node);

        sources_.insert(node,new Source());
    }
//    showSourceNode("");

    QVariantMap modes;
    modes.insert(tr("Current"),"current");
    modes.insert(tr("Voltage"),"voltage");

    foreach(QVariant mode,modes.values()){
        ui->sourceFirstTypeComboBox->addItem( modes.key(mode), mode);
        ui->sourceSecondTypeComboBox->addItem( modes.key(mode), mode);
        ui->sourceModeComboBox->addItem( modes.key(mode), mode );
    }

    ui->sourceFirstStartLineEdit->setText("0.0");
    ui->sourceSecondStartLineEdit->setText("0.0");
    ui->sourceFirstStepLineEdit->setText("1.0");
    ui->sourceSecondStepLineEdit->setText("1.0");
    ui->sourceFirstStopLineEdit->setText("10.0");
    ui->sourceSecondStopLineEdit->setText("10.0");

    changedAnalysisType( ui->analysisTypeComboBox->currentIndex() );

    // Типы источников
    ui->sourceTypeComboBox->addItem(tr("Ground"),"ground");
    ui->sourceTypeComboBox->addItem(tr("CONST"),"const");
    ui->sourceTypeComboBox->addItem(tr("PULSE"),"pulse");
    ui->sourceTypeComboBox->addItem(tr("SIN"),"sin");
    ui->sourceTypeComboBox->addItem(tr("EXP"),"exp");

//    foreach( QVariant modeSource, modeSources.values() ){
//        ui->sourceTypeComboBox->addItem( modeSources.key(modeSource),modeSource);
//    }

    sourceConfigurationView_ = new gui::KeyValueView();
    ui->sourceConfigurationTableView->setModel( sourceConfigurationView_ );

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



    // -- new
    connect(ui->analysisTypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedAnalysisType(int)));
    connect(ui->sourceSecondEnable,SIGNAL(toggled(bool)),
            this,SLOT(checkedSourceSecondEnable(bool)));
    connect(ui->sourceNodeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceNode(int)));
    connect(ui->sourceTypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceType(int)));
    connect(ui->sourceModeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceMode(int)));
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

void AnalysisWindow::showSource(const QString &node, const QString &type, const QString &mode) {
    Source* source = sources_.value(node);

    sourceConfigurationView_->clear();

    source->mode( mode );
    source->type( type );

    for(int i=0; i < source->numberParameters(); ++i){
       gui::KeyValuePair pair;
       pair.key = source->parameter(i).name();
       pair.title = source->parameter(i).title();
       pair.type = gui::KeyValuePair::TYPE_TEXT;
       pair.value = QString::number( source->parameter(i).value().toDouble());
       sourceConfigurationView_->addPair(pair);
    }

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
/**
 * @brief Обработчик изменения типа анализа
 * @param index
 */
void AnalysisWindow::changedAnalysisType(int index) {

    QString analysis = ui->analysisTypeComboBox->itemData( index ).toString();

    if(analysis == "dc"){
        ui->sourceFirstTypeComboBox->setEnabled(true);
        ui->sourceFirstNodeComboBox->setEnabled(true);

        ui->sourceSecondEnable->setEnabled(true);
        if(ui->sourceSecondEnable->isChecked()){
            ui->sourceSecondGroup->setEnabled(true);
        }else{
            ui->sourceSecondGroup->setEnabled(false);
        }

    } else if(analysis == "ac" || analysis == "tran"){
        ui->sourceFirstTypeComboBox->setEnabled(false);
        ui->sourceFirstNodeComboBox->setEnabled(false);
        ui->sourceSecondEnable->setEnabled(false);
        ui->sourceSecondGroup->setEnabled(false);

    }

}
/**
 * @brief Обработчик изменения узла источника
 * @param index
 */
void AnalysisWindow::changedSourceNode(int index) {
//    QString node = ui->sourceNodeComboBox->itemData(index).toString();
    int sourceTypeId = ui->sourceModeComboBox->currentIndex();
    changedSourceType( sourceTypeId );
}
/**
 * @brief Обработчик checkbox включить/выключить дополнительный источник
 * @param checked
 */
void AnalysisWindow::checkedSourceSecondEnable(bool checked) {
    ui->sourceSecondGroup->setEnabled(checked);
}

void AnalysisWindow::changedSourceType(int index) {
    QString type = ui->sourceTypeComboBox->itemData(index).toString();

    if(type == "ground"){
        ui->sourceModeComboBox->setEnabled(false);
        ui->sourceConfigurationTableView->setEnabled(false);
    } else {
        int modeId = ui->sourceModeComboBox->currentIndex();
        changedSourceMode(modeId);

        ui->sourceModeComboBox->setEnabled(true);
        ui->sourceConfigurationTableView->setEnabled(true);
    }

}

void AnalysisWindow::changedSourceMode(int index) {
    int nodeId = ui->sourceNodeComboBox->currentIndex();
    int typeId = ui->sourceTypeComboBox->currentIndex();
    int modeId = ui->sourceModeComboBox->currentIndex();

    QString node = ui->sourceNodeComboBox->itemData(nodeId).toString();
    QString type = ui->sourceTypeComboBox->itemData(typeId).toString();
    QString mode = ui->sourceModeComboBox->itemData(modeId).toString();

    showSource(node,type,mode);

}

}
