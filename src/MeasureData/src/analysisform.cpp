#include "analysisform.h"
#include "ui_analysisform.h"

AnalysisForm::AnalysisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisForm) {
    ui->setupUi(this);

    storage_ = AnalysisStorage::instance();

    itemsInputView_ = new ListItemView();
    itemsOutputView_ = new ListItemView();

    openAnalysis( 1 );

}



AnalysisForm::~AnalysisForm()
{
    delete ui;
}

void AnalysisForm::prepareItemsInput() {
    IAnalysisItem* item = NULL;
    QVariantMap items;
    int noItem=0;

    foreach(item, model_.inputs()){
        items.insert(item->name(),noItem);
        noItem++;
    }

    itemsInputView_->setItems( items );
}

void AnalysisForm::prepareItemsOutput() {
    IAnalysisItem* item = NULL;
    QVariantMap items;
    int noItem=0;

    foreach(item, model_.outputs()){
        items.insert(item->name(),noItem);
        noItem++;
    }

    itemsOutputView_->setItems( items );
}

void AnalysisForm::openAnalysis(const int &analysisId) {
    model_ =  storage_->openAnalysis( analysisId );

    ui->nameAnalysisText->setText( model_.name() );

    prepareItemsInput();
    prepareItemsOutput();

}
