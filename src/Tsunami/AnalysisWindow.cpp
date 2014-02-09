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



    api_ = new APIObject(deviceId_);
    storage_ = db::AnalysisStorage::instance();

    // Заполняем типы анализов
    ui->analysisTypeComboBox->addItem( "AC", "ac" );
    ui->analysisTypeComboBox->addItem( "DC", "dc" );
    ui->analysisTypeComboBox->addItem( "TRAN", "tran" );

    //    showSourceNode("");

    QVariantMap modes;
    modes.insert(tr("Current"),"current");
    modes.insert(tr("Voltage"),"voltage");

    foreach(QVariant mode,modes.values()){
        ui->sourceFirstTypeComboBox->addItem( modes.key(mode), mode);
        ui->sourceSecondTypeComboBox->addItem( modes.key(mode), mode);
    }
    modes.insert(tr("Ground"),"gnd");
    foreach(QVariant mode,modes.values()){
        ui->node1TypeComboBox->addItem(modes.key(mode),mode);
        ui->node2TypeComboBox->addItem(modes.key(mode),mode);
        ui->node3TypeComboBox->addItem(modes.key(mode),mode);
        ui->node4TypeComboBox->addItem(modes.key(mode),mode);
    }

    ui->sourceFirstStartLineEdit->setText("0.0");
    ui->sourceSecondStartLineEdit->setText("0.0");
    ui->sourceFirstStepLineEdit->setText("1.0");
    ui->sourceSecondStepLineEdit->setText("1.0");
    ui->sourceFirstStopLineEdit->setText("10.0");
    ui->sourceSecondStopLineEdit->setText("10.0");

    changedAnalysisType( ui->analysisTypeComboBox->currentIndex() );

    sourceConfigurationView_ = new gui::KeyValueView();


    nodes_.insert(1, "D");
    nodes_.insert(2, "G");
    nodes_.insert(3, "S");
    nodes_.insert(4, "B");

    foreach(QString node,nodes_.values()){
        Source* source = new Source();
        source->node( node );
        sources_.insert(node,source);
        showSource(node);

        ui->sourceFirstNodeComboBox->addItem(node,node);
        ui->sourceSecondNodeComboBox->addItem(node,node);
    }



    connect(ui->analysisTypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedAnalysisType(int)));
    connect(ui->sourceSecondEnable,SIGNAL(toggled(bool)),
            this,SLOT(checkedSourceSecondEnable(bool)));

    connect(ui->node1TypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceTypeNode1(int)));
    connect(ui->node2TypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceTypeNode2(int)));
    connect(ui->node3TypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceTypeNode3(int)));
    connect(ui->node4TypeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedSourceTypeNode4(int)));


    connect(ui->node1ParamsButton,SIGNAL(clicked()),
            this,SLOT(clickedSourceParamsNode1()));
    connect(ui->node2ParamsButton,SIGNAL(clicked()),
            this,SLOT(clickedSourceParamsNode2()));
    connect(ui->node3ParamsButton,SIGNAL(clicked()),
            this,SLOT(clickedSourceParamsNode3()));
    connect(ui->node4ParamsButton,SIGNAL(clicked()),
            this,SLOT(clickedSourceParamsNode4()));


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

void AnalysisWindow::showSource(const QString &node) {
    Source* source = sources_.value(node);

    int nodeId = nodes_.key(node);
    int index = -1;
    getNodeButton(nodeId)->setHidden(false);
    if(source->mode() == SOURCE_MODE_VOLTAGE){
        index = getNodeComboBox( nodeId )->findData( "voltage" );
    }else if(source->mode() == SOURCE_MODE_CURRENT){
        index = getNodeComboBox( nodeId )->findData( "current" );
    }else{
        index = getNodeComboBox( nodeId )->findData( "gnd" );
        getNodeButton(nodeId)->setHidden(true);
    }
    getNodeComboBox( nodeId )->setCurrentIndex(index);

}

QPushButton *AnalysisWindow::getNodeButton(int index) {
    switch(index){
    case 4:
        return ui->node4ParamsButton;
    case 3:
        return ui->node3ParamsButton;
    case 2:
        return ui->node2ParamsButton;
    case 1:
        return ui->node1ParamsButton;
    }

    return NULL;
}

QComboBox *AnalysisWindow::getNodeComboBox(int index) {
    switch(index){
    case 4:
        return ui->node4TypeComboBox;
    case 3:
        return ui->node3TypeComboBox;
    case 2:
        return ui->node2TypeComboBox;
    case 1:
        return ui->node1TypeComboBox;
    }

    return NULL;
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

//    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("Api",api_);
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
 * @brief Обработчик checkbox включить/выключить дополнительный источник
 * @param checked
 */
void AnalysisWindow::checkedSourceSecondEnable(bool checked) {
    ui->sourceSecondGroup->setEnabled(checked);
}

void AnalysisWindow::changedSourceTypeNode1(int index) {
    QString type = ui->node1TypeComboBox->itemData(index).toString();
    changeSourceType(1,type);
}

void AnalysisWindow::changedSourceTypeNode2(int index) {
    QString type = ui->node2TypeComboBox->itemData(index).toString();
    changeSourceType(2,type);

}

void AnalysisWindow::changedSourceTypeNode3(int index) {
    QString type = ui->node3TypeComboBox->itemData(index).toString();
    changeSourceType(3,type);

}

void AnalysisWindow::changedSourceTypeNode4(int index) {
    QString type = ui->node4TypeComboBox->itemData(index).toString();
    changeSourceType(4,type);

}

void AnalysisWindow::changeSourceType(int nodeId, const QString &type) {

    if(type == "gnd"){
        getNodeButton(nodeId)->setHidden(true);
    }else{
        getNodeButton(nodeId)->setHidden(false);
    }

    QString node = nodes_.value(nodeId);

    sources_[node]->mode( type );
}

void AnalysisWindow::clickedSourceParamsNode1() {
    int index = getNodeComboBox(1)->currentIndex();
    QString type = getNodeComboBox(1)->itemData(index).toString();
    clickedSourceParams(1,type);
}

void AnalysisWindow::clickedSourceParamsNode2() {
    int index = getNodeComboBox(2)->currentIndex();
    QString type = getNodeComboBox(2)->itemData(index).toString();
    clickedSourceParams(2,type);
}

void AnalysisWindow::clickedSourceParamsNode3() {
    int index = getNodeComboBox(3)->currentIndex();
    QString type = getNodeComboBox(3)->itemData(index).toString();
    clickedSourceParams(3,type);
}

void AnalysisWindow::clickedSourceParamsNode4() {
    int index = getNodeComboBox(4)->currentIndex();
    QString type = getNodeComboBox(4)->itemData(index).toString();
    clickedSourceParams(4,type);
}

void AnalysisWindow::clickedSourceParams(int nodeId, const QString& type) {
    QString node = nodes_.value(nodeId);
    Source* source = sources_.value(node);

    IndepedentSourceDialog dlg(source);

    dlg.exec();
}





}
