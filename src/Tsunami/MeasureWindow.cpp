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
    if(!analysis) { return; }

    db::MeasureModel* measure = new db::MeasureModel();
    measure->sources( analysis->sources() );
    measure->type( analysis->type() );
//    measure->deviceId(deviceId_);
    prepareMeasureData( measure );
    delete analysis;

    showMeasure(measure);
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
    if(measure_){
        delete measure_;
        measure_ = 0;
    }

    measure_ = measure;

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
                        source.title("%MODE %NODE %METHOD\n")
                        );
        }
    }
    ui->measureSources->setText( sourcesDescription );
}
 bool MeasureWindow::prepareMeasureData(db::MeasureModel* measure) {
     if(!measure) { return false; }

    QStringList columns;
    QVector< QVector<double> > data;

    int nNonLinearSources = 0;
    QMap< int, QVector<double> > nonLinearSourcesData;

    foreach(Source source, measure->sources()){
        columns.append( source.name() );
        if( source.method() == SOURCE_METHOD_LINEAR ) {
            double start = source.configuration("start").toDouble();
            double step = source.configuration("step").toDouble();
            double end = source.configuration("end").toDouble();

            QVector<double> sourceData;
            int count = static_cast<int>(fabs((end-start)/step));
            for(int i=0; i<= count;++i){
                sourceData.append(start+step*i);
            }

            int number= source.configuration("number").toInt();
            if(number > 0){
                nonLinearSourcesData.insert( number, sourceData );
            }
            nNonLinearSources++;
        }
    }

    Q_ASSERT( nNonLinearSources > 0 && nNonLinearSources <= 2);

    if(nNonLinearSources == 1){
        nonLinearSourcesData.insert(2, QVector<double>() << .0 );
    }

//    if( nNonLinearSources == 2 ){
       foreach(double dc2, nonLinearSourcesData[2]){
           foreach(double dc1, nonLinearSourcesData[1]){
               QVector<double> rowData;
               foreach(Source source, measure->sources()){
                   if(source.direction() == SOURCE_DIRECTION_INPUT){
                       switch(source.method()){
                       case SOURCE_METHOD_CONST:
                           rowData.append( source.configuration("const").toDouble() );
                           break;
                       case SOURCE_METHOD_LINEAR:
                           {
                             int num = source.configuration("number").toInt();
                             if(num == 2){
                                 rowData.append( dc2 );
                             }else if(num == 1){
                                 rowData.append( dc1 );
                             }else{
                                 Q_ASSERT(false);
                             }
                           }
                           break;
                       default:
                           rowData.append(.0);
                       }
                   }else{
                       rowData.append(.0);
                   }
               }
               data.append( rowData );
           }
       }
//    }

    measure->columns( columns );
    measure->data( data );

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

