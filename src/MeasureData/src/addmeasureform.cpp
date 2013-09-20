#include "addmeasureform.h"
#include "ui_addmeasureform.h"
#include <QtCore>

const int addMeasureForm::nPairs_ = 1;
KeyValuePair addMeasureForm::headerPairs_[] = {
    KeyValuePair("test", QVariant(""),  KeyValuePair::TYPE_TEXT)
//    {"date", QDate::currentDate(), KeyValueView::TYPE_DATE}
};


addMeasureForm::addMeasureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm)
{
    ui->setupUi(this);


    headerView_ = new KeyValueView();

//    for(int i=0; i < nPairs_; ++i){
//        headerView_->addPair( headerPairs_[i] );
//    }

    headerView_->addPairs( headerPairs_, nPairs_ );

    ui->headerTableView->setModel( headerView_ );


}

addMeasureForm::~addMeasureForm()
{
    delete ui;
}
