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
//    measure->sources( analysis->sources() );
//    measure->type( analysis->type() );
////    measure->deviceId(deviceId_);
    prepareMeasureData( analysis );
//    delete analysis;

    showMeasure(measure_);
}

void MeasureWindow::updateMeasure(int measureId) {
    if(measureId == -1) { return; }
    ui->addButton->setText( tr("Save") );

    db::MeasureModel* measure = measureStorage_->openMeasure(measureId);
    showMeasure(measure);

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
    QList<Source> sources = measure_->sources();
    foreach(Source source,sources){        
        if(source.direction() == SOURCE_DIRECTION_INPUT){
            sourcesDescription.append(
                        source.title("%MODE %NODE %METHOD %CONFIG\n")
                        );
        }
    }
    ui->measureSources->setText( sourcesDescription );
}
// \fixme stub code :(
 bool MeasureWindow::prepareMeasureData(db::AnalysisModel* analysis) {
//    if(!measure) { return false; }

    SourceManager* sourceManager = new SourceManager(analysis->sources());
    QStringList columns;
    QVector< QVector<double> > data;


    QVariantList sources = analysis->analysis()->sources();
    if(analysis->analysis()->type() == ANALYSIS_DC){

        columns.append( sourceManager->inputByNode(sources[0].toMap().value("node").toString())->title());

        double dcFirstValue = sources[0].toMap().value("start").toDouble();
        double dcFirstStep  = sources[0].toMap().value("step").toDouble();
        double dcFirstStop  = sources[0].toMap().value("stop").toDouble();


        if(analysis->analysis()->numberSources() == 2){
            columns.append( sourceManager->inputByNode(sources[1].toMap().value("node").toString())->title());

            double dcSecondValue = sources[1].toMap().value("start").toDouble();
            double dcSecondStep  = sources[1].toMap().value("step").toDouble();
            double dcSecondStop  = sources[1].toMap().value("stop").toDouble();


            for(; dcSecondValue <= dcSecondStop; dcSecondValue+=dcSecondStep){
                for(; dcFirstValue <= dcFirstStop; dcFirstValue+=dcFirstStep){
                    QVector<double> row;
                    row.append( dcFirstValue );
                    row.append( dcSecondValue );

                    data.append( row );
                }
            }
        }else{
            for(; dcFirstValue <= dcFirstStop; dcFirstValue+=dcFirstStep){
                QVector<double> row;
                row.append( dcFirstValue );
//                row.append( dcSecondValue );
                data.append( row );

            }
        }
    }else{
        columns.append( analysis->analysis()->typeJson() );
        double srcFirstValue = sources[0].toMap().value("start").toDouble();
        double srcFirstStep  = sources[0].toMap().value("step").toDouble();
        double srcFirstStop  = sources[0].toMap().value("stop").toDouble();

        for(; srcFirstValue <= srcFirstStop; srcFirstValue+=srcFirstStep){
            QVector<double> row;
            row.append( srcFirstValue );
//                row.append( dcSecondValue );
            data.append( row );
        }
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

        if(!Source::compare(measure_->sources(),
                            importedMeasure->sources())) {
            log::logError() << "Import not passed by sources";
            return;
        }

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

