#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <views/keyvalueview.h>
#include <views/measureitemview.h>
#include <models/analysisitems.h>
#include "dbstorage/measurestorage.h"
#include "delegates/delegatereadonly.h"
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
    //    openAnalysis( analysisId );
}

addMeasureForm::~addMeasureForm() {
    delete ui;

}
// TODO: rechange code
bool addMeasureForm::isReadOnlyColumn(const QString &column) {
    if(column.startsWith("V")){
        return true;
    }
    return false;
}

void addMeasureForm::openAnalysis(int analysisId) {
    analysisId_ = analysisId;

    headerView_->setPairs( headerPairs_, nPairs_ );



}

void addMeasureForm::openMeasure(int measureId) {

    measureStorage_ = db::MeasureStorage::instance();
    measure_ = measureStorage_->openMeasure( measureId );

    if(measure_ == 0){
        Q_ASSERT(false);
    }

    measureView_ = new gui::MeasureItemView( measure_ );
    ui->dataTableView->setModel( measureView_ );

    int col = 0;
    foreach( QString column, measure_->columns()){
        if(isReadOnlyColumn(column)){
            ui->dataTableView->setItemDelegateForColumn(col,new gui::DelegateReadOnly(ui->dataTableView));
        }
        col++;
    }




    headerView_->setPairs( headerPairs_, nPairs_ );
    headerView_->setValue( "type", measure_->typeJson() );
    headerView_->setValue( "comment", measure_->header().comment );
    headerView_->setValue( "user_date", measure_->header().userDate );
    headerView_->setValue( "fabricate_date", measure_->header().fabricationDate );
    headerView_->setValue( "dubious", measure_->header().dubious );

    QVariantMap attrs = measure_->attrs();
    qDebug() << attrs;
    foreach( QString attrName, attrs.keys() ){
        attributesView_->addPair( attrName, attrs.value(attrName),
                                  gui::KeyValuePair::TYPE_TEXT, attrName.toUpper() );
    }


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


//    ui->setupUi(this);

//    headerView_ = new KeyValueView();
//    headerView_->setPairs( headerPairs_, nPairs_ );

//    ui->headerTableView->setModel( headerView_ );

//    headerView_->fillDelegates( ui->headerTableView );

//    measureView_ = new MeasureItemView( -1 );





//    // Nodes
//    ListItemView* nodesView = new ListItemView();
//    nodesView->addItem( trUtf8("И"), "s"  ).
//            addItem( trUtf8("З"), "g"  ).
//            addItem( trUtf8("С"), "d"  ).
//            addItem( trUtf8("П"), "b"  );

//    ui->nodesListView->setModel( nodesView );
//    prepareAnalysis( analysisId );


//    attributesView_ = new KeyValueView();
//    attributesView_->addPair( "w", QVariant(),KeyValuePair::TYPE_TEXT, trUtf8("W") );
//    attributesView_->addPair( "l", QVariant(),KeyValuePair::TYPE_TEXT, trUtf8("L") );

//    ui->attributesTableView->setModel( attributesView_ );
//    attributesView_->fillDelegates( ui->attributesTableView );
//    //    data.items.append( QVector<double>() << 1.0 << 2.0 );



//    ui->dataTableView->setModel( measureView_ );


//    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addButtonClick()));
//}

//addMeasureForm::~addMeasureForm()
//{
//    delete ui;
//}

//bool sortItems(IAnalysisItem* item1, IAnalysisItem* item2) {
//    if(item2->getItemType() == item1->getItemType()){
//        if(item2->getItemType() == ANALYSIS_ITEM_SWEEP){
//            return static_cast<AnalysisItemSweep*>(item2)->number() >
//                    static_cast<AnalysisItemSweep*>(item1)->number();

//        }
//        return true;
//    }else if(item2->getItemType() > item1->getItemType()){
//        return true;
//    }
//    return false;
//}

//void addMeasureForm::prepareAnalysis(const int &analysisId) {
//    MeasureData data = measureView_->model().measureData();
//    AnalysisModel analysis = AnalysisStorage::instance()->openAnalysis( analysisId );


//    QList<IAnalysisItem*> items;

//    QList<IAnalysisItem*> inputs = analysis.inputs();
//    qSort( inputs.begin(), inputs.end(), sortItems );
//    ui->measureInputsText->setText("");
//    foreach(IAnalysisItem* item, inputs){
//        ui->measureInputsText->setText( QString("%1%2\n").arg(ui->measureInputsText->text()).arg(item->title()) );
//    }


//    items.append( inputs );
//    QList<IAnalysisItem*> outputs = analysis.outputs();
//    qSort( outputs.begin(), outputs.end(), sortItems );




//    items.append( outputs );

//    int nColumns = items.size();
//    QVector< QVector<double> > dataItems;
//    QVector<double> rowItems(nColumns);

//    int maxSweepNumber = countAnalysisItem( items, ANALYSIS_ITEM_SWEEP );
//    QVector<double> current;

//    // TODO bullshit code, need change
//    // MAX[number()] <= 2
//    if( maxSweepNumber == 2){
//        double dc2 = static_cast<AnalysisItemSweep*>(items[1])->start();
//        while(dc2 <= static_cast<AnalysisItemSweep*>(items[1])->stop()){
//            rowItems.fill(0.0);
//            double dc1 = static_cast<AnalysisItemSweep*>(items[0])->start();
//            while( dc1 <= static_cast<AnalysisItemSweep*>(items[0])->stop()){
//                rowItems[0] = dc1;
//                rowItems[1] = dc2;

//                for(int i=0; i < nColumns; ++i){
//                    if(items[i]->getItemType() == ANALYSIS_ITEM_CONST){
//                        rowItems[i] = static_cast<AnalysisItemConst*>(items[i])->constant();
//                    }
//                }

//                dataItems.append( rowItems );
//                dc1+=static_cast<AnalysisItemSweep*>(items[0])->step();
//            }
//            dc2 += static_cast<AnalysisItemSweep*>(items[0])->step();
//        }
//    }else if(maxSweepNumber == 1){
//        double dc1 = static_cast<AnalysisItemSweep*>(items[0])->start();
//        while( dc1 <= static_cast<AnalysisItemSweep*>(items[0])->stop()){
//            rowItems[0] = dc1;

//            for(int i=0; i < nColumns; ++i){
//                if(items[i]->getItemType() == ANALYSIS_ITEM_CONST){
//                    rowItems[i] = static_cast<AnalysisItemConst*>(items[i])->constant();
//                }
//            }

//            dataItems.append( rowItems );
//            dc1+=static_cast<AnalysisItemSweep*>(items[0])->step();
//        }
//    }

//    data.columns.clear();
//    for(int i=0; i < nColumns;++i){
//        data.columns.append( items[i]->name() );
//    }
//    data.items = dataItems;

//    measureView_->model().setMeasureData( data );



//    for(int i=0; i < nColumns;++i){
//        if( items[i]->getItemType() != ANALYSIS_ITEM_OUTPUT &&
//                items[i]->getItemType() != ANALYSIS_ITEM_NONE ){

//            ui->dataTableView->setItemDelegateForColumn( i, new DelegateReadOnly() );
//        }
//    }



//}

//int addMeasureForm::countAnalysisItem(const QList<IAnalysisItem *> &items, const AnalysisItemType &type) {
//    int nMatched = 0;
//    foreach(IAnalysisItem* item, items){
//        if(item->getItemType() == type){
//            ++nMatched;
//        }
//    }
//    return nMatched;
//}

//void addMeasureForm::addButtonClick() {

//    MeasureHeader header;
//    qDebug() << headerView_->getPair("type").value.toString();
//    if(headerView_->getPair("type").value.toString().compare("dc")==0){
//        header.type = TYPE_DC;
//    }else if(headerView_->getPair("type").value.toString().compare("ac")==0){
//        header.type = TYPE_AC;
//    }else if(headerView_->getPair("type").value.toString().compare("tran")==0){
//        header.type = TYPE_TRAN;
//    }else{
//        header.type = TYPE_UNKNOWN;
//    }

//

//    measureView_->model().setChangeAt( );
//    measureView_->model().setCreateAt( QDateTime::currentDateTime() );
//    measureView_->model().setEnable( true );
//    measureView_->model().setHeader( header );

//    if(!measureView_->saveMeasure()){
//        qDebug() << "Measurement did not save to db.";
//    }

//}

}

