#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <views/keyvalueview.h>
#include <views/measureitemview.h>
#include <models/analysisitems.h>
#include "dbstorage/measurestorage.h"
#include "dbstorage/analysisstorage.h"
#include "models/analysismodel.h"
#include <QTableView>
#include "delegates/delegatedoubleitem.h"
#include <QFileDialog>
namespace tsunami{

const int addMeasureForm::nPairs_ = 6;
gui::KeyValuePair addMeasureForm::headerPairs_[] = {
    gui::KeyValuePair("type",  QVariant("dc"),  gui::KeyValuePair::TYPE_READONLY, QString("Analysis Type")),
    gui::KeyValuePair("user",  QVariant(""),  gui::KeyValuePair::TYPE_READONLY, QString("User")),
    gui::KeyValuePair("user_date",  QVariant(QDate::currentDate()),  gui::KeyValuePair::TYPE_DATE, QString("User date")),
    gui::KeyValuePair("fabricate_date",  QVariant(QDate::currentDate()),  gui::KeyValuePair::TYPE_DATE, QString("Fabrication date")),
    gui::KeyValuePair("comment",  QVariant(""),  gui::KeyValuePair::TYPE_TEXT, QString("Comment")),
    gui::KeyValuePair("dubious",  QVariant(""),  gui::KeyValuePair::TYPE_CHECKBOX, QString("Dubious"))
};


addMeasureForm::addMeasureForm(addMeasureForm::Action action, int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm) {

    ui->setupUi( this );

    headerView_ = new gui::KeyValueView();
    attributesView_ = new gui::KeyValueView();
    measureView_ = NULL;

    action_ = action;

    ui->headerTableView->setModel( headerView_ );
    ui->attributesTableView->setModel( attributesView_ );


    if(action == NEW){
        openAnalysis(id);
        ui->addButton->setText( tr("Add") );
    }else if(action == EDIT){
        openMeasure(id);
        ui->addButton->setText( tr("Save") );
    }
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addButtonClick()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));

    connect(ui->exportButton,SIGNAL(clicked()),this,SLOT(clickedExportButton()));
    connect(ui->importButton,SIGNAL(clicked()),this,SLOT(clickedImportButton()));
    //    openAnalysis( analysisId );
}

addMeasureForm::~addMeasureForm() {
    delete ui;

}

void addMeasureForm::showSourcesDescription() {
    QString sourcesDescription;
    QList<Source> sources = measure_->sources();
    foreach(Source source,sources){
        if(source.direction() == SOURCE_DIRECTION_INPUT){
            sourcesDescription.append( QString("%1 %2 %3\n").arg(source.node()).arg(source.modeJson().toUpper())
                                       .arg(source.methodJson().toUpper()));
        }
    }
    ui->measureSources->setText( sourcesDescription );
}
 void addMeasureForm::prepareNewMeasureData() {
    Q_ASSERT(measure_ != 0);

    QStringList columns;

    QVector< QVector<double> > data;

    int nNonLinearSources = 0;
    QMap< int, QVector<double> > nonLinearSourcesData;

    foreach(Source source, measure_->sources()){
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
            qDebug() << source.configurations();

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
               foreach(Source source, measure_->sources()){
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

    qDebug() << data.size();
    qDebug() << columns.size();
    measure_->columns( columns );
    measure_->data( data );

}

void addMeasureForm::openAnalysis(int analysisId) {
    analysisId_ = analysisId;

    headerView_->setPairs( headerPairs_, nPairs_ );

    db::AnalysisStorage* storage = db::AnalysisStorage::instance();
    db::AnalysisModel* analysis = storage->openAnalysis( analysisId_ );

    QList<Source> sources = analysis->sources();

    measure_ = new db::MeasureModel();
    measure_->sources( analysis->sources() );

    measure_->type( analysis->type() );

    prepareNewMeasureData();


    measureView_ = new gui::MeasureItemView( measure_ );
    ui->dataTableView->setModel( measureView_ );
    int nColumns = measureView_->columnCount();
    for(int i=0; i < nColumns; ++i){
        if(!measureView_->isColumnReadOnly(i)){
            ui->dataTableView->setItemDelegateForColumn(i, new DelegateDoubleItem(ui->dataTableView));
        }
    }

    showSourcesDescription();
}

void addMeasureForm::openMeasure(int measureId) {

    measureStorage_ = db::MeasureStorage::instance();
    measure_ = measureStorage_->openMeasure( measureId );

    if(measure_ == 0){
        Q_ASSERT(false);
    }

    measureView_ = new gui::MeasureItemView( measure_ );
    ui->dataTableView->setModel( measureView_ );

    int nColumns = measureView_->columnCount();
    for(int i=0; i < nColumns; ++i){
        if(!measureView_->isColumnReadOnly(i)){
            ui->dataTableView->setItemDelegateForColumn(i, new DelegateDoubleItem(ui->dataTableView));
        }
    }

    headerView_->setPairs( headerPairs_, nPairs_ );
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

    showSourcesDescription();


}

void addMeasureForm::addButtonClick() {
    QVariantMap attributes;
    for(int i=0;i < attributesView_->rowCount(); ++i){
        attributes.insert( attributesView_->getPair( i  ).key,
                           attributesView_->getPair( i  ).value);

    }
    measure_->attrs(attributes );

    db::MeasureHeader header;

    header.comment = headerView_->getPair( "comment" ).value.toString();
    header.userDate = headerView_->getPair( "user_date" ).value.toDate();
    header.fabricationDate = headerView_->getPair("fabricate_date").value.toDate();
    header.dubious = headerView_->getPair("dubious").value.toBool();

    measure_->header( header );
    measure_->changeAt( QDateTime::currentDateTime()  );

    measureStorage_->saveMeasure( measure_ );
}

void addMeasureForm::clickedExportButton() {
    QString fileName = QFileDialog::getSaveFileName( this, tr("Export to file"), QString(), QString("*.tmb") );

    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly)){
            QByteArray data = db::MeasureModel::exportTo( measure_ );
            file.write(data);
        }
        file.close();
    }
}

void addMeasureForm::clickedImportButton() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import from file"),QString(),QString("*.tmb"));
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly)){
            QByteArray data = file.readAll();
            measure_ = db::MeasureModel::importFrom( data );

            measureView_ = new gui::MeasureItemView( measure_ );
            ui->dataTableView->setModel( measureView_ );

        }
        file.close();
    }


}


}

