#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <QtCore>
#include <dbstorage/analysisstorage.h>
#include <delegates/delegatereadonly.h>

const int addMeasureForm::nPairs_ = 0;
KeyValuePair addMeasureForm::headerPairs_[] = {
    KeyValuePair("test",  QVariant(),  KeyValuePair::TYPE_TEXT, tr("Test")),
};


addMeasureForm::addMeasureForm(const int &analysisId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm) {



    ui->setupUi(this);

    headerView_ = new KeyValueView();
    headerView_->setPairs( headerPairs_, nPairs_ );

    ui->headerTableView->setModel( headerView_ );

    measureView_ = new MeasureItemView( -1 );


    prepareAnalysis( analysisId );



    //    data.items.append( QVector<double>() << 1.0 << 2.0 );



    ui->dataTableView->setModel( measureView_ );


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


