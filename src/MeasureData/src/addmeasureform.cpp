#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <QtCore>

const int addMeasureForm::nPairs_ = 3;
KeyValuePair addMeasureForm::headerPairs_[] = {
    KeyValuePair("test",  QVariant(""),  KeyValuePair::TYPE_TEXT),
    KeyValuePair("date",  QDate::currentDate(), KeyValuePair::TYPE_DATE),
    KeyValuePair("test1", true, KeyValuePair::TYPE_CHECKBOX )
};


addMeasureForm::addMeasureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm)
{
    ui->setupUi(this);


    headerView_ = new KeyValueView();
    headerView_->addPairs( headerPairs_, nPairs_ );

    ui->headerTableView->setModel( headerView_ );

    measureView_ = new MeasureItemView( -1 );

    MeasureData data = measureView_->model().measureData();

    data.columns.append( "aa" );
    data.columns.append( "bb" );

    data.items.append( QVector<double>() << 1.0 << 2.0 );

    measureView_->model().setMeasureData( data );

    ui->dataTableView->setModel( measureView_ );

}

addMeasureForm::~addMeasureForm()
{
    delete ui;
}
