#include "AnalysisWindow.h"
#include "ui_AnalysisWindow.h"

#include <QComboBox>
#include <QLineEdit>
#include "dbstorage/DbStorages.h"
#include "Log.h"
namespace tsunami{
using namespace spice;
using namespace db;

AnalysisWindow::AnalysisWindow(int deviceId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow),
    deviceId_(deviceId)
{
    ui->setupUi(this);
    // Test data

    storage_ = db::AnalysisStorage::instance();

    // Заполняем типы анализов
    ui->analysisTypeComboBox->addItem( "AC", "ac" );
    ui->analysisTypeComboBox->addItem( "DC", "dc" );
    ui->analysisTypeComboBox->addItem( "TRAN", "tran" );

    QVariantMap modes;
    modes.insert(tr("Current"),"current");
    modes.insert(tr("Voltage"),"voltage");
    modes.insert(tr("Ground"),"gnd");
    foreach(QVariant mode,modes.values()){
        ui->node1TypeComboBox->addItem(modes.key(mode),mode);
        ui->node2TypeComboBox->addItem(modes.key(mode),mode);
        ui->node3TypeComboBox->addItem(modes.key(mode),mode);
        ui->node4TypeComboBox->addItem(modes.key(mode),mode);
    }

    changedAnalysisType( ui->analysisTypeComboBox->currentIndex() );

    prepareDevice();

    // Connect
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

    connect(ui->saveButton,SIGNAL(clicked()),
            this,SLOT(clickedSaveAnalysis()));
}

AnalysisWindow::~AnalysisWindow() {
    delete ui;
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
        getNodeCheckBox(nodeId)->setHidden(true);
    }
    getNodeComboBox( nodeId )->setCurrentIndex(index);

}

void AnalysisWindow::showAnalysis() const {
    int indexType = ui->analysisTypeComboBox->findData( analysis_->typeJson() );
    ui->analysisTypeComboBox->setCurrentIndex( indexType );
//    QVariantList sources = analysis_->analyses();
    ui->sourceSecondEnable->setChecked(false);
    if(analysis_->type() == ANALYSIS_DC){

//        int modeId = ui->sourceFirstTypeComboBox->findData(sources[0].toMap().value("mode"));
//        ui->sourceFirstTypeComboBox->setCurrentIndex(modeId);
        int nodeId = ui->sourceFirstNodeComboBox->findData(analysis_->analysis(0).value("node"));
        ui->sourceFirstNodeComboBox->setCurrentIndex(nodeId);
        ui->sourceFirstStartLineEdit->setText( analysis_->analysis(0).value("start").toString() );
        ui->sourceFirstStepLineEdit->setText( analysis_->analysis(0).value("step").toString() );
        ui->sourceFirstStopLineEdit->setText( analysis_->analysis(0).value("stop").toString() );

        if(analysis_->numberAnalyses() == 2){
            ui->sourceSecondEnable->setChecked(true);
//            modeId = ui->sourceSecondTypeComboBox->findData(sources[1].toMap().value("mode"));
//            ui->sourceSecondTypeComboBox->setCurrentIndex(modeId);
            nodeId = ui->sourceSecondNodeComboBox->findData(analysis_->analysis(1).value("node"));
            ui->sourceSecondNodeComboBox->setCurrentIndex(nodeId);
            ui->sourceSecondStartLineEdit->setText( analysis_->analysis(1).value("start").toString() );
            ui->sourceSecondStepLineEdit->setText( analysis_->analysis(1).value("step").toString() );
            ui->sourceSecondStopLineEdit->setText( analysis_->analysis(1).value("stop").toString() );
        }

    }else{
        ui->sourceFirstStartLineEdit->setText( analysis_->analysis(0).value("start").toString() );
        ui->sourceFirstStepLineEdit->setText( analysis_->analysis(0).value("step").toString() );
        ui->sourceFirstStopLineEdit->setText( analysis_->analysis(0).value("stop").toString() );
    }


}

void AnalysisWindow::hideSource(int index, bool hide) {
    getNodeButton(index)->setHidden(hide);
    getNodeComboBox(index)->setHidden(hide);
    getNodeCheckBox(index)->setHidden(hide);
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

QCheckBox *AnalysisWindow::getNodeCheckBox(int index){
    switch(index){
    case 4:
        return ui->node4CheckBox;
    case 3:
        return ui->node3CheckBox;
    case 2:
        return ui->node2CheckBox;
    case 1:
        return ui->node1CheckBox;
    }

    return NULL;
}

QVariantList AnalysisWindow::parseAnalysisSources() {
    int analysisTypeIndex = ui->analysisTypeComboBox->currentIndex();
    QString type = ui->analysisTypeComboBox->itemData(analysisTypeIndex).toString();
    QVariantList sources;
    QVariantMap source;
    if(type == "dc"){
        QString mode;
        int nodeId = ui->sourceFirstNodeComboBox->currentIndex();
        source.insert("node", ui->sourceFirstNodeComboBox->itemData(nodeId));
//        int modeId = ui->sourceFirstTypeComboBox->currentIndex();
//        source.insert("mode",ui->sourceFirstTypeComboBox->itemData(modeId));
        source.insert("start",ui->sourceFirstStartLineEdit->text());
        source.insert("step",ui->sourceFirstStepLineEdit->text());
        source.insert("stop",ui->sourceFirstStopLineEdit->text());

        sources.append(source);

        if( ui->sourceSecondEnable->isChecked() ){
            source.clear();
            int nodeId = ui->sourceSecondNodeComboBox->currentIndex();
            source.insert("node", ui->sourceSecondNodeComboBox->itemData(nodeId));
//            int modeId = ui->sourceSecondTypeComboBox->currentIndex();
//            source.insert("mode",ui->sourceSecondTypeComboBox->itemData(modeId));
            source.insert("start",ui->sourceSecondStartLineEdit->text());
            source.insert("step",ui->sourceSecondStepLineEdit->text());
            source.insert("stop",ui->sourceSecondStopLineEdit->text());
            sources.append(source);    \

        }
    }else{
        source.insert("start",ui->sourceFirstStartLineEdit->text());
        source.insert("step",ui->sourceFirstStepLineEdit->text());
        source.insert("stop",ui->sourceFirstStopLineEdit->text());

        sources.append(source);
    }

    return sources;
}


void AnalysisWindow::clickedOpenAnalysis() {
//    if( analysisId_ != -1 ){
//        analysis_ = openAnalysis(analysisId_);
//        return;
//    }
}

void AnalysisWindow::clickedSaveAnalysis() {
    log::logTrace() << "Saving analysis";
    QString analysisName = ui->analysisNameLineEdit->text();
    if(analysisName.isEmpty()){
        return;
    }
    analysis_->deviceId( deviceId_ );
    analysis_->name(analysisName);
    analysis_->enable( ui->analysisEnableCheckBox->isChecked() );

    int analysisTypeIndex = ui->analysisTypeComboBox->currentIndex();
    QString type = ui->analysisTypeComboBox->itemData(analysisTypeIndex).toString();
    analysis_->type(type);


    analysis_->analyses(parseAnalysisSources());

    // Parse outputs

    analysis_->clearSources(SOURCE_DIRECTION_OUTPUT);

    foreach(int nodeId, nodes_.keys()){
        if( !getNodeCheckBox(nodeId)->isHidden() && getNodeCheckBox(nodeId)->isChecked()){
            Source* outputSource = new Source();
            outputSource->node( nodes_.value(nodeId) );
            outputSource->direction( SOURCE_DIRECTION_OUTPUT );
            int index = getNodeComboBox(nodeId)->currentIndex();
            QString sourceMode = getNodeComboBox(nodeId)->itemData(index).toString();
            if(sourceMode == "voltage"){
                outputSource->mode(SOURCE_MODE_CURRENT);
            }else{
                outputSource->mode(SOURCE_MODE_VOLTAGE);
            }

            analysis_->addSource( outputSource );

        }

    }


    storage_->saveAnalysis(analysis_);

    //    if(storage_->exists( deviceId_, analysisName )){
//        // TODO: show message if analysis exists
//        QMessageBox::warning(this,windowTitle(),tr("Analysis is existed"));
//        log::logDebug() << "Analysis existed";
//        return;
//    }

}

//void AnalysisWindow::clickedCreateAnalysis() {
//    log::logTrace() << "Creating analysis";
//    currentAnalysis_ = new db::AnalysisModel();
//    currentAnalysis_->deviceId( deviceId_ );

//    ui->analysisNameLineEdit->setText("");
//    ui->analysisTypeComboBox->setCurrentIndex(0);
//    ui->analysisEnableCheckBox->setCheckState(Qt::Unchecked);

//    api_->openAnalysis( currentAnalysis_ );

//}

//void AnalysisWindow::selectedAnalysisItem(const QModelIndex &index) {
//    bool ok;
//    int analysisId = index.data(Qt::UserRole).toInt(&ok);
//    if(!ok || analysisId == -1){
//        return;
//    }

//    openAnalysis( analysisId );

//}

void AnalysisWindow::openAnalysis(int analysisId) {

    if(analysisId == -1) {
        analysis_ = new AnalysisModel();
        foreach(QString node, nodes_.values()){
            Source* newSource = new Source();
            newSource->node( node );
            analysis_->addSource( newSource );
        }

        analysis_->type( ANALYSIS_DC );

        QVariantMap analysis;
        analysis.insert("node", nodes_.values().first());
        analysis.insert("start","0.0");
        analysis.insert("stop","10.0");
        analysis.insert("step","1.0");

        analysis_->addAnalysis(analysis);
    } else {
        analysis_ = storage_->openAnalysis( analysisId );
    }

    ui->analysisNameLineEdit->setText( analysis_->name() );
    ui->analysisEnableCheckBox->setChecked( analysis_->enable() );

    foreach( Source* source, analysis_->sources(SOURCE_DIRECTION_INPUT) ){
        sources_.insert(source->node(),source);
        showSource(source->node());
    }


    foreach( Source* source, analysis_->sources(SOURCE_DIRECTION_OUTPUT) ){
        int nodeId = nodes_.key(source->node());
        getNodeCheckBox(nodeId)->setChecked(true);
    }


    showAnalysis( );



}

void AnalysisWindow::prepareDevice() {
    DeviceStorage* deviceStorage = DeviceStorage::instance();
    DeviceModel* device = deviceStorage->openDevice( deviceId_ );

    int index = 1;
    foreach(QString node, device->nodes()){
        nodes_.insert(index,node);

        ui->sourceFirstNodeComboBox->addItem(node,node);
        ui->sourceSecondNodeComboBox->addItem(node,node);

        index++;
    }

    // Скрываем оставшиеся
    int MAX_NODES = 4;
    for(int i=index; i <= MAX_NODES; ++i){
        hideSource(i);
    }

    QPixmap deviceImage;
    if(device->type() == DEVICE_NBJT){
        deviceImage = QPixmap(":/images/NBJT");
    }else if(device->type() == DEVICE_PBJT){
        deviceImage = QPixmap(":/images/PBJT");
    }else if(device->type() == DEVICE_NFET){
        deviceImage = QPixmap(":/images/NFET");
    }else if(device->type() == DEVICE_NFET){
        deviceImage = QPixmap(":/images/NFET");
    }else if(device->type() == DEVICE_PFET){
        deviceImage = QPixmap(":/images/PFET");
    }else if(device->type() == DEVICE_NMOS){
        deviceImage = QPixmap(":/images/NMOS");
    }else if(device->type() == DEVICE_PMOS){
        deviceImage = QPixmap(":/images/PMOS");
    }

    ui->imagePixmap->setPixmap( deviceImage );
    ui->imageOutput->setPixmap(deviceImage);
}

/**
 * @brief Обработчик изменения типа анализа
 * @param index
 */
void AnalysisWindow::changedAnalysisType(int index) {

    QString analysis = ui->analysisTypeComboBox->itemData( index ).toString();

    if(analysis == "dc"){
//        ui->sourceFirstTypeComboBox->setEnabled(true);
        ui->sourceFirstNodeComboBox->setEnabled(true);

        ui->sourceSecondEnable->setEnabled(true);
        if(ui->sourceSecondEnable->isChecked()){
            ui->sourceSecondGroup->setEnabled(true);
        }else{
            ui->sourceSecondGroup->setEnabled(false);
        }

    } else if(analysis == "ac" || analysis == "tran"){
//        ui->sourceFirstTypeComboBox->setEnabled(false);
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
        getNodeCheckBox(nodeId)->setHidden(true);
    }else{
        getNodeButton(nodeId)->setHidden(false);
        getNodeCheckBox(nodeId)->setHidden(false);
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

    return;
}





}
