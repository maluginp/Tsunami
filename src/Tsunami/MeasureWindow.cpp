#include "MeasureWindow.h"
#include "ui_MeasureWindow.h"
#include "views/Views.h"
#include "dbstorage/DbStorages.h"
#include "delegates/DelegateDoubleItem.h"

#include "Log.h"

namespace tsunami{

MeasureWindow::MeasureWindow(int deviceId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasureWindow),
    deviceId_(deviceId),
    headerView_(NULL),
    attributesView_(NULL),
    measureView_(NULL),
    measure_(NULL),
    measureStorage_(NULL)
{

    ui->setupUi( this );

    measureStorage_ = db::MeasureStorage::instance();


    attributesView_ = new gui::KeyValueView();
    ui->attributesTableView->setModel( attributesView_ );
    setHeaderData();

    connect(ui->addButton,SIGNAL(clicked()),
                          SLOT(clickedAddButton()));
    connect(ui->cancelButton,SIGNAL(clicked()),
                             SLOT(close()));

    connect(ui->exportButton,SIGNAL(clicked()),
                             SLOT(clickedExportButton()));
    connect(ui->importButton,SIGNAL(clicked()),
                             SLOT(clickedImportButton()));
    //    openAnalysis( analysisId );
    connect(ui->addAttributeButton,SIGNAL(clicked()),
            SLOT(clickedAddAttributeButton()));


}

void MeasureWindow::createMeasure(int analysisId) {
    if(analysisId == -1){
        return;
    }

    ui->addButton->setText( tr("Add") );

    db::AnalysisStorage* storage = db::AnalysisStorage::instance();
    db::AnalysisModel* analysis = storage->openAnalysis( analysisId );
//    if(!analysis) { return; }
//    qDebug() << analysis->source(0).configurations();
     measure_ = new db::MeasureModel();
     measure_->analyses(analysis->analyses());
    measure_->sources( analysis->sources() );
    measure_->type( analysis->type() );
    measure_->deviceId(deviceId_);
    prepareMeasureData( analysis );
    delete analysis;

    log::logDebug() << QString("Created %1").arg(measure_->debug());

    showMeasure(measure_);
}

void MeasureWindow::updateMeasure(int measureId) {
    if(measureId == -1) { return; }
    ui->addButton->setText( tr("Save") );

    measure_ = measureStorage_->openMeasure(measureId);
    showMeasure(measure_);

}

MeasureWindow::~MeasureWindow() {
    delete ui;

}

void MeasureWindow::showMeasure(db::MeasureModel *measure) {
//    if(measure_){
//        delete measure_;
//        measure_ = 0;
//    }

//    measure_ = measure;

    log::logDebug() << QString("Show %1").arg(measure->debug());

    ui->nameMeasureLineEdit->setText(measure->name());

    headerView_->setValue( "type", measure_->typeJson() );
    headerView_->setValue( "comment", measure_->header().comment );
    headerView_->setValue( "user_date", measure_->header().userDate );
    headerView_->setValue( "fabricate_date", measure_->header().fabricationDate );
    headerView_->setValue( "dubious", measure_->header().dubious );

    QVariantMap attrs = measure_->attrs();

    foreach( QString attrName, attrs.keys() ){
        attributesView_->addPair( attrName, attrs.value(attrName),
                                  gui::KeyValuePair::TYPE_TEXT, attrName.toUpper() );
    }


    measureView_ = new gui::MeasureItemView(measure_);
    ui->dataTableView->setModel( measureView_ );
    int nColumns = measureView_->columnCount();
    for(int i=0; i < nColumns; ++i){
        if(!measureView_->isColumnReadOnly(i)){
            ui->dataTableView->setItemDelegateForColumn(i, new DelegateDoubleItem(ui->dataTableView));
        }
    }

    showSourcesDescription();
}

void MeasureWindow::showSourcesDescription() {
    QString sourcesDescription;
    QList<Source*> sources = measure_->sources();
    foreach(Source* source,sources){
        if(source->direction() == SOURCE_DIRECTION_INPUT){
            sourcesDescription.append(
                        source->title("%MODE %NODE %METHOD %CONFIG\n")
                        );
        }
    }
    ui->measureSources->setText( sourcesDescription );
}
// \fixme stub code :(
 bool MeasureWindow::prepareMeasureData(db::AnalysisModel* analysis) {
//    if(!measure) { return false; }

    log::logTrace() << "Preparing measure data";

    SourceManager* sourceManager = new SourceManager(analysis->sources());
    QStringList columns = getColumns(analysis);
    QVector< QVector<double> > data, analysisData;

    int numberOutputs = sourceManager->outputs().count();

    QList<double> analysisFirstValues = analysis->analysisValues(0);

    if(analysis->type() == ANALYSIS_DC && analysis->numberAnalyses() == 2){
        QList<double> analysisSecondValues = analysis->analysisValues(1);
        foreach(double dc2, analysisSecondValues){
            foreach(double dc1, analysisFirstValues){
                QVector<double> row;
                row.append( dc1 );
                row.append( dc2 );
                analysisData.append( row );
            }
        }
    }else{
        foreach(double value,analysisFirstValues){
            QVector<double> row;
            row.append( value );
            analysisData.append( row );
        }
    }


    foreach(QVector<double> tmpData, analysisData){
        QVector<double> row = tmpData;

        foreach(Source* source,sourceManager->inputs()){
            int index = columns.indexOf( source->name() );
            if(index != -1 && index >= analysis->numberAnalyses()){
//                if(source->hasConfiguration("dc")){
                    row.append( source->configuration("dc",.0).toDouble() );
//                }else{
//                    row.append(.0);
//                }
            }
        }


        for(int i=0; i < numberOutputs; ++i){
            row.append(.0);
        }

        Q_ASSERT( row.count() == columns.count());
        data.append(row);

    }

    measure_->columns( columns );
    measure_->data( data );

    return true;

 }

 void MeasureWindow::setHeaderData() {

     if(headerView_){
         delete headerView_;
         headerView_ = 0;
     }

     headerView_ = new gui::KeyValueView();
     headerView_->hideHeader();

     headerView_->addPair("type", "dc",
                          gui::KeyValuePair::TYPE_READONLY,tr("Analysis Type"));
     headerView_->addPair("user",  QVariant(""),
                          gui::KeyValuePair::TYPE_READONLY, tr("User"));
     headerView_->addPair("user_date",  QVariant(QDate::currentDate()),
                          gui::KeyValuePair::TYPE_DATE, tr("User date"));
     headerView_->addPair("fabricate_date",  QVariant(QDate::currentDate()),
                          gui::KeyValuePair::TYPE_DATE, tr("Fabrication date"));
     headerView_->addPair("comment",  QVariant(""),
                          gui::KeyValuePair::TYPE_TEXT, tr("Comment"));
     headerView_->addPair("dubious",  QVariant(""),
                          gui::KeyValuePair::TYPE_CHECKBOX, QString("Dubious"));

     ui->headerTableView->setModel( headerView_ );
     ui->headerTableView->setColumnWidth(0,150);
     headerView_->fillDelegates( ui->headerTableView );
 }

 QStringList MeasureWindow::getColumns(db::AnalysisModel *analysis) {
     QStringList columns;
     SourceManager* sourceManager = new SourceManager(analysis->sources());


     if(analysis->type() == ANALYSIS_DC){
         QString node1 = analysis->analysis(0).value("node").toString();
         columns.append( sourceManager->inputByNode(node1)->name() );
         if(analysis->numberAnalyses() == 2){
             QString node2 = analysis->analysis(1).value("node").toString();
             columns.append(sourceManager->inputByNode(node2)->name());
         }
     }else{
         if(analysis->type()==ANALYSIS_AC){
             columns.append("ac");
         }else{
             columns.append("tran");
         }
     }

     foreach(Source* source, sourceManager->inputs()){
         if(!columns.contains(source->name())
                 && source->type() == SOURCE_TYPE_CONST
                 && source->mode() != SOURCE_MODE_GND){
             columns.append(source->name());
         }
     }

     foreach(Source* source, sourceManager->outputs()){
         columns.append(source->name());
     }

     return columns;
 }

void MeasureWindow::clickedAddAttributeButton() {
    attributesView_->addPair("","",gui::KeyValuePair::TYPE_TEXT,"");
}

void MeasureWindow::clickedAddButton() {
    log::logTrace() << "Creating measure";

    QString name = ui->nameMeasureLineEdit->text();

    if(name.isEmpty()){
        return;
    }

    measure_->name( name );

    QVariantMap attributes;
    for(int i=0;i < attributesView_->rowCount(); ++i){
        attributes.insert( attributesView_->getPair( i  ).key,
                           attributesView_->getPair( i  ).value);

    }
    measure_->attrs(attributes );
    measure_->deviceId( deviceId_ );
    db::MeasureHeader header;

    header.comment = headerView_->getPair( "comment" ).value.toString();
    header.userDate = headerView_->getPair( "user_date" ).value.toDate();
    header.fabricationDate = headerView_->getPair("fabricate_date").value.toDate();
    header.dubious = headerView_->getPair("dubious").value.toBool();

    measure_->header( header );
    measure_->changeAt( QDateTime::currentDateTime()  );

    bool isNew = measure_->id() == -1;
    if(measureStorage_->saveMeasure( measure_ )){
        log::logDebug() << "Measure saved";
        emit updatedDataBase();

        // \todo Stub
        if(isNew){
            int measureId = measure_->id();
            updateMeasure(measureId);
        }
    }else{
        log::logError() << "Measure not saved";
    }
}

void MeasureWindow::clickedExportButton() {
    QString fileName = QFileDialog::getSaveFileName( this, tr("Export to file"), QString(), QString("*.tmb") );

    if(!fileName.isEmpty()){
        log::logTrace() << "Exporting to " << fileName;
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly)){
            QByteArray data = db::MeasureModel::exportTo( measure_ );
            file.write(data);
        }
        file.close();
    }
}

void MeasureWindow::clickedImportButton() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import from file"),QString(),QString("*.tmb"));
    if(fileName.isEmpty()){
        return;
    }

    log::logTrace() << "Importing from " << fileName;

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        db::MeasureModel* importedMeasure = db::MeasureModel::importFrom( data );

        log::logDebug() << QString("Imported measure %1 with %2 items(%3 columns)")
                           .arg(importedMeasure->typeJson())
                           .arg(importedMeasure->dataRows())
                           .arg(importedMeasure->dataColumns());

//        Q_ASSERT(false);
//        if(!Source::compare(measure_->sources(),
//                            importedMeasure->sources())) {
//            log::logError() << "Import not passed by sources";
//            return;
//        }

        // Check attributes
        QVariantMap attributes = measure_->attrs();
        foreach( QString name, attributes.keys()){
            if(!importedMeasure->attrs().contains(name)
               || ( importedMeasure->attrs().value(name) != attributes.value(name) )){
                log::logError() << "Import not passed by attributes";
                return;
            }
        }

        measure_->columns( importedMeasure->columns() );
        measure_->data( importedMeasure->data() );

        delete importedMeasure;

        measureView_ = new gui::MeasureItemView( measure_ );
        ui->dataTableView->setModel( measureView_ );

    }
    file.close();

    return;

}


}

