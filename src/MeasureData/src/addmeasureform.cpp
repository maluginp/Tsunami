#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <QtCore>
#include <dbstorage/analysisstorage.h>
#include <delegates/delegatereadonly.h>
#include <views/listitemview.h>

const int addMeasureForm::nPairs_ = 6;
KeyValuePair addMeasureForm::headerPairs_[] = {
    KeyValuePair("type",  QVariant("dc"),  KeyValuePair::TYPE_READONLY, tr("Analysis Type")),
    KeyValuePair("user",  QVariant(""),  KeyValuePair::TYPE_TEXT, tr("User")),
    KeyValuePair("user_date",  QVariant(QDate::currentDate()),  KeyValuePair::TYPE_DATE, tr("User date")),
    KeyValuePair("fabric_date",  QVariant(QDate::currentDate()),  KeyValuePair::TYPE_DATE, tr("Fabrication date")),
    KeyValuePair("comment",  QVariant(""),  KeyValuePair::TYPE_TEXT, tr("Comment")),
    KeyValuePair("dubious",  QVariant(""),  KeyValuePair::TYPE_CHECKBOX, tr("Dubious"))

};


addMeasureForm::addMeasureForm(const int &analysisId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm) {



    ui->setupUi(this);

    headerView_ = new KeyValueView();
    headerView_->setPairs( headerPairs_, nPairs_ );

    ui->headerTableView->setModel( headerView_ );

    headerView_->fillDelegates( ui->headerTableView );

    measureView_ = new MeasureItemView( -1 );



    // Nodes
    ListItemView* nodesView = new ListItemView();
    nodesView->addItem( trUtf8("И"), "s"  ).
            addItem( trUtf8("З"), "g"  ).
            addItem( trUtf8("С"), "d"  ).
            addItem( trUtf8("П"), "b"  );

    ui->nodesListView->setModel( nodesView );
    prepareAnalysis( analysisId );


    attributesView_ = new KeyValueView();
    attributesView_->addPair( "w", QVariant(),KeyValuePair::TYPE_TEXT, trUtf8("W") );
    attributesView_->addPair( "l", QVariant(),KeyValuePair::TYPE_TEXT, trUtf8("L") );

    ui->attributesTableView->setModel( attributesView_ );
    attributesView_->fillDelegates( ui->attributesTableView );
    //    data.items.append( QVector<double>() << 1.0 << 2.0 );



    ui->dataTableView->setModel( measureView_ );


    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addButtonClick()));


}

addMeasureForm::~addMeasureForm()
{
    delete ui;
}

bool sortItems(IAnalysisItem* item1, IAnalysisItem* item2) {
    if(item2->getItemType() == item1->getItemType()){
        if(item2->getItemType() == ANALYSIS_ITEM_SWEEP){
            return static_cast<AnalysisItemSweep*>(item2)->number() >
                    static_cast<AnalysisItemSweep*>(item1)->number();

        }
        return true;
    }else if(item2->getItemType() > item1->getItemType()){
        return true;
    }
    return false;
}

void addMeasureForm::prepareAnalysis(const int &analysisId) {
    MeasureData data = measureView_->model().measureData();
    AnalysisModel analysis = AnalysisStorage::instance()->openAnalysis( analysisId );


    QList<IAnalysisItem*> items;

    QList<IAnalysisItem*> inputs = analysis.inputs();
    qSort( inputs.begin(), inputs.end(), sortItems );
    ui->measureInputsText->setText("");
    foreach(IAnalysisItem* item, inputs){
        ui->measureInputsText->setText( QString("%1%2\n").arg(ui->measureInputsText->text()).arg(item->title()) );
    }


    items.append( inputs );
    QList<IAnalysisItem*> outputs = analysis.outputs();
    qSort( outputs.begin(), outputs.end(), sortItems );




    items.append( outputs );

    int nColumns = items.size();
    QVector< QVector<double> > dataItems;
    QVector<double> rowItems(nColumns);

    int maxSweepNumber = countAnalysisItem( items, ANALYSIS_ITEM_SWEEP );
    QVector<double> current;


    // MAX[number()] <= 2
    if( maxSweepNumber == 2){
        double dc2 = static_cast<AnalysisItemSweep*>(items[1])->start();
        while(dc2 <= static_cast<AnalysisItemSweep*>(items[1])->stop()){
            rowItems.fill(0.0);
            double dc1 = static_cast<AnalysisItemSweep*>(items[0])->start();
            while( dc1 <= static_cast<AnalysisItemSweep*>(items[0])->stop()){
                rowItems[0] = dc1;
                rowItems[1] = dc2;

                for(int i=0; i < nColumns; ++i){
                    if(items[i]->getItemType() == ANALYSIS_ITEM_CONST){
                        rowItems[i] = static_cast<AnalysisItemConst*>(items[i])->constant();
                    }
                }

                dataItems.append( rowItems );
                dc1+=static_cast<AnalysisItemSweep*>(items[0])->step();
            }
            dc2 += static_cast<AnalysisItemSweep*>(items[0])->step();
        }
    }

    data.columns.clear();
    for(int i=0; i < nColumns;++i){
        data.columns.append( items[i]->name() );
    }
    data.items = dataItems;

    measureView_->model().setMeasureData( data );



    for(int i=0; i < nColumns;++i){
        if( items[i]->getItemType() != ANALYSIS_ITEM_OUTPUT &&
                items[i]->getItemType() != ANALYSIS_ITEM_NONE ){

            ui->dataTableView->setItemDelegateForColumn( i, new DelegateReadOnly() );
        }
    }



}

int addMeasureForm::countAnalysisItem(const QList<IAnalysisItem *> &items, const AnalysisItemType &type) {
    int nMatched = 0;
    foreach(IAnalysisItem* item, items){
        if(item->getItemType() == type){
            ++nMatched;
        }
    }
    return nMatched;
}

void addMeasureForm::addButtonClick() {

    qDebug() << "Add button click";
    qDebug() << headerView_->getPair( "dubious" ).value;
}


