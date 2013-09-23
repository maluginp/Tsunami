#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <QtCore>
#include <dbstorage/analysisstorage.h>

const int addMeasureForm::nPairs_ = 0;
KeyValuePair addMeasureForm::headerPairs_[] = {
    KeyValuePair("test",  QVariant(),  KeyValuePair::TYPE_TEXT, tr("Test")),
};


addMeasureForm::addMeasureForm(const int &analysisId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm) {

    AnalysisModel analysis = AnalysisStorage::instance()->openAnalysis( analysisId );

    ui->setupUi(this);

    headerView_ = new KeyValueView();
    headerView_->setPairs( headerPairs_, nPairs_ );

    ui->headerTableView->setModel( headerView_ );

    measureView_ = new MeasureItemView( -1 );

    MeasureData data = measureView_->model().measureData();

    foreach(IAnalysisItem* item, analysis.inputs()){
        data.columns.append( item->name() );
    }
    foreach(IAnalysisItem* item, analysis.outputs()){
        data.columns.append( item->name() );
    }


//    data.items.append( QVector<double>() << 1.0 << 2.0 );

    measureView_->model().setMeasureData( data );

    ui->dataTableView->setModel( measureView_ );


}

addMeasureForm::~addMeasureForm()
{
    delete ui;
}
