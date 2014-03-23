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

    ui->sourceVariationComboBox->addItem("DEC", "dec");
    ui->sourceVariationComboBox->addItem("OCT", "oct");
    ui->sourceVariationComboBox->addItem("LINEAR", "linear");


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

void AnalysisWindow::showAnalysis() {
    setAnalysisParameter("analysis",analysis_->typeJson());

    ui->sourceSecondEnable->setChecked(false);

    if(analysis_->type() == ANALYSIS_DC){
        setAnalysisParameters(analysis_->analysis(0),1);
        if(analysis_->numberAnalyses() == 2){
            setAnalysisParameter("enable_source2",true);
            setAnalysisParameters(analysis_->analysis(1),2);
        }
    }else{
        setAnalysisParameters(analysis_->analysis(0),1);
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

// @todo добавить нормальные анализаторы
QVariantList AnalysisWindow::parseAnalysisSources() {
    QString type = analysisParameter("analysis").toString();

    QVariantList sources;
    QVariantMap source;
    if(type == "dc"){
        source.insert("node",  analysisParameter("node"));
        source.insert("start", analysisParameter("start"));
        source.insert("step",  analysisParameter("step"));
        source.insert("stop",  analysisParameter("stop"));
        source.insert("name",  analysisParameter("name"));
        sources.append(source);

        if( analysisParameter("enable_source2").toBool() ){
            source.clear();
            source.insert("node",  analysisParameter("node",2));
            source.insert("start", analysisParameter("start",2));
            source.insert("step",  analysisParameter("step",2));
            source.insert("stop",  analysisParameter("stop",2));
            source.insert("name",  analysisParameter("name",2));
            sources.append(source);

        }
    }else if(type == "ac"){
        source.insert("variation", analysisParameter("variation"));
        source.insert("start",     analysisParameter("start"));
        source.insert("points",    analysisParameter("points"));
        source.insert("stop",      analysisParameter("stop"));
        sources.append(source);
    }else if(type == "tran"){
        source.insert("start", analysisParameter("start"));
        source.insert("step",  analysisParameter("step"));
        source.insert("stop",  analysisParameter("stop"));
        sources.append(source);
    }

    return sources;
}

void AnalysisWindow::hideAnalysisParameter(const QString &name,bool hide,int source) {

    if(name == "start"){
        if(source == 1){
            ui->sourceFirstStartLineEdit->setHidden(hide);
            ui->sourceStartLabel->setHidden(hide);
        }else if(source == 2){
            ui->sourceSecondStartLabel->setHidden(hide);
            ui->sourceSecondStartLineEdit->setHidden(hide);
        }
    }else if(name == "step"){
        if(source == 1){
            ui->sourceFirstStepLineEdit->setHidden(hide);
            ui->sourceStepLabel->setHidden(hide);
        }else if(source == 2){
            ui->sourceSecondStepLabel->setHidden(hide);
            ui->sourceSecondStepLineEdit->setHidden(hide);
        }
    }else if(name == "stop"){
        if(source == 1){
            ui->sourceFirstStopLineEdit->setHidden(hide);
            ui->sourceStopLabel->setHidden(hide);
        }else if(source == 2){
            ui->sourceSecondStopLabel->setHidden(hide);
            ui->sourceSecondStopLineEdit->setHidden(hide);
        }
    }else if(name == "node"){
        if(source == 1){
            ui->sourceFirstNodeComboBox->setHidden(hide);
            ui->sourceNodeLabel->setHidden(hide);
        }else if(source == 2){
            ui->sourceSecondNodeComboBox->setHidden(hide);
            ui->sourceSecondNodeLabel->setHidden(hide);
        }
    }else if(name == "points"){
        ui->sourcePointsLabel->setHidden(hide);
        ui->sourcePointsLineEdit->setHidden(hide);
    }else if(name=="variation"){
        ui->sourceVariationComboBox->setHidden(hide);
        ui->sourceVariationLabel->setHidden(hide);
    }

}

QVariant AnalysisWindow::analysisParameter(const QString &name, int source) {
    if(name == "start"){
        if(source == 1){
            return ui->sourceFirstStartLineEdit->text();

        }else if(source == 2){
            return ui->sourceSecondStartLineEdit->text();
        }
    }else if(name == "step"){
        if(source == 1){
            return ui->sourceFirstStepLineEdit->text();
        }else if(source == 2){
            return ui->sourceSecondStepLineEdit->text();
        }
    }else if(name == "stop"){
        if(source == 1){
            return ui->sourceFirstStopLineEdit->text();
        }else if(source == 2){
            return ui->sourceSecondStopLineEdit->text();
        }
    }else if(name == "node"){
        if(source == 1){
            int index = ui->sourceFirstNodeComboBox->currentIndex();
            return ui->sourceFirstNodeComboBox->itemData(index);
        }else if(source == 2){
            int index = ui->sourceSecondNodeComboBox->currentIndex();
            return ui->sourceSecondNodeComboBox->itemData(index);
        }
    }else if(name == "points"){
        return ui->sourcePointsLineEdit->text();
    }else if(name=="variation"){
        int index = ui->sourceVariationComboBox->currentIndex();
        return ui->sourceVariationComboBox->itemData(index);
    }else if(name =="enable_source2"){
        return ui->sourceSecondEnable->isChecked();
    }else if(name == "name"){
        QString node = analysisParameter("node",source).toString();
        Source* source = sources_.value(node);

        if(source->isGnd()){
            return QString();
        }
        return source->name();
    }else if(name == "analysis"){
        int analysisTypeIndex = ui->analysisTypeComboBox->currentIndex();
        return ui->analysisTypeComboBox->itemData(analysisTypeIndex).toString();

    }

    return QVariant();
}

void AnalysisWindow::setAnalysisParameter(const QString &name, const QVariant &value, int source) {
    log::logTrace() << QString("setAnalysisParameter(%1,%2,%3)")
                       .arg(name,value.toString()).arg(source);
    if(name == "start"){
        if(source == 1){
            ui->sourceFirstStartLineEdit->setText(value.toString());

        }else if(source == 2){
            ui->sourceSecondStartLineEdit->setText(value.toString());
        }
    }else if(name == "step"){
        if(source == 1){
            ui->sourceFirstStepLineEdit->setText(value.toString());
        }else if(source == 2){
            ui->sourceSecondStepLineEdit->setText(value.toString());
        }
    }else if(name == "stop"){
        if(source == 1){
            ui->sourceFirstStopLineEdit->setText(value.toString());
        }else if(source == 2){
            ui->sourceSecondStopLineEdit->setText(value.toString());
        }
    }else if(name == "node"){
        if(source == 1){
            int index = ui->sourceFirstNodeComboBox->findData(value);
            ui->sourceFirstNodeComboBox->setCurrentIndex(index);
        }else if(source == 2){
            int index = ui->sourceSecondNodeComboBox->findData(value);
            ui->sourceSecondNodeComboBox->setCurrentIndex(index);
        }
    }else if(name == "points"){
        ui->sourcePointsLineEdit->setText(value.toString());
    }else if(name=="variation"){
        int index = ui->sourceVariationComboBox->findData(value);
        ui->sourceVariationComboBox->setCurrentIndex(index);
    }else if(name =="enable_source2"){
        ui->sourceSecondEnable->setChecked( value.toBool() );
    }else if(name == "analysis"){
        int index = ui->analysisTypeComboBox->findData(value);
        ui->analysisTypeComboBox->setCurrentIndex(index);

    }
}

void AnalysisWindow::setAnalysisParameters(const QVariantMap &parameters, int source) {
    foreach(QString key,parameters.keys()){
        setAnalysisParameter(key,parameters.value(key),source);
    }
}


void AnalysisWindow::clickedOpenAnalysis() {
//    if( analysisId_ != -1 ){
//        analysis_ = openAnalysis(analysisId_);
//        return;
//    }
}

void AnalysisWindow::clickedSaveAnalysis() {
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

    log::logDebug() << QString("Saving %1").arg(analysis_->debug());

    if(storage_->saveAnalysis(analysis_)){
        emit updatedDataBase();
    }

    //    if(storage_->exists( deviceId_, analysisName )){
//        // TODO: show message if analysis exists
//        QMessageBox::warning(this,windowTitle(),tr("Analysis is existed"));
//        log::logDebug() << "Analysis existed";
//        return;
//    }

}

void AnalysisWindow::openAnalysis(int analysisId) {
    log::logDebug() << QString("Open analysis %1").arg(analysisId);

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

    log::logDebug() << QString("Opening %1").arg(analysis_->debug());

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

    if(!device) {
        log::logError() << QString("Can not open device %1").arg(deviceId_);
        return;
    }

    int index = 1;
    foreach(QString node, device->nodes()){
        nodes_.insert(index,node);

        ui->sourceFirstNodeComboBox->addItem(node,node);
        ui->sourceSecondNodeComboBox->addItem(node,node);

        index++;
    }

    log::logDebug() << QString("Device %1 (Nodes:%2)").arg(device->name())
                       .arg(device->nodes().join(","));

    // Скрываем оставшиеся
    int MAX_NODES = 4;
    for(int i=index; i <= MAX_NODES; ++i){
        hideSource(i);
    }

    QPixmap deviceImage( device->image() );
    log::logTrace() << QString("Device has image: %1").arg(deviceImage.isNull());

    if(!deviceImage.isNull()){
        ui->imagePixmap->setPixmap(deviceImage);
        ui->imageOutput->setPixmap(deviceImage);
    }
}

/**
 * @brief Обработчик изменения типа анализа
 * @param index
 */
void AnalysisWindow::changedAnalysisType(int index) {

    QString analysis = ui->analysisTypeComboBox->itemData( index ).toString();

    ui->sourceSecondEnable->setEnabled(false);
    ui->sourceSecondGroup->setEnabled(false);

    if(analysis == "dc" || analysis == "tran"){
        hideAnalysisParameter("points");
        hideAnalysisParameter("variation");
        hideAnalysisParameter("step",false);
        hideAnalysisParameter("node");
        if(analysis == "dc"){
            ui->sourceSecondEnable->setEnabled(true);
            hideAnalysisParameter("node",false);
            if(analysisParameter("enable_source2").toBool()){
                ui->sourceSecondGroup->setEnabled(true);
            }else{
                ui->sourceSecondGroup->setEnabled(false);
            }
        }
    }else if(analysis == "ac"){
        hideAnalysisParameter("points",false);
        hideAnalysisParameter("variation",false);
        hideAnalysisParameter("step",true);
        hideAnalysisParameter("node");
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
