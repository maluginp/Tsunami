#include "ChoiceAnalysisForm.h"
#include "ui_ChoiceAnalysisForm.h"
#include "dbstorage/AnalysisStorage.h"
#include "models/AnalysisModel.h"

namespace tsunami{

ChoiceAnalysisForm::ChoiceAnalysisForm(int deviceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceAnalysisForm),
    currentModel_(NULL) {
    ui->setupUi(this);

    deviceId_ = deviceId;
    storage_ = db::AnalysisStorage::instance();


    listModel_ = new gui::ListItemView();
    ui->analysisList->setModel( listModel_ );


    updateList();

    connect(ui->analysisList,SIGNAL(clicked(QModelIndex)),
                        this,SLOT(clickedAnalysis(QModelIndex)));

    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(clickedAddButton()));
    connect(ui->editButton,SIGNAL(clicked()),this,SLOT(clickedEditButton()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));

}

ChoiceAnalysisForm::~ChoiceAnalysisForm()
{
    delete ui;
}

int ChoiceAnalysisForm::getAnalysisId(int deviceId, QWidget* parent) {
    ChoiceAnalysisForm form(deviceId,parent);
    int analysisId = -1;
    if(form.exec() == QDialog::Accepted){
        if(form.currentModel_ != NULL){
             analysisId = form.currentModel_->id();
        }else{
            return -1;
        }
    }
    form.close();
    return analysisId;
}

void ChoiceAnalysisForm::clickedAnalysis(const QModelIndex &index) {
    int analysisId = listModel_->data( index, Qt::EditRole ).toInt();

    currentModel_ = storage_->openAnalysis( analysisId );

    showAnalysisDescription( currentModel_ );



}

void ChoiceAnalysisForm::clickedAddButton() {
    Q_ASSERT(false);
}

void ChoiceAnalysisForm::clickedEditButton() {
    Q_ASSERT(false);
}

void ChoiceAnalysisForm::updateList() {
    QMap<int,QString> list = storage_->listAnalysis(deviceId_);

    listModel_->clear();

    foreach(int analysisId,list.keys()){
        listModel_->addItem( list.value(analysisId), QVariant(analysisId) );
    }

}

void ChoiceAnalysisForm::showAnalysisDescription(db::AnalysisModel *model) {

    QString desc;

    desc.append( QString("<b>Name:</b> %1<br/>").arg(model->name()) );

    QString typeAnalysis;
    switch(model->type()){
    case ANALYSIS_DC:
        typeAnalysis = "DC"; break;
    case ANALYSIS_AC:
        typeAnalysis = "AC"; break;
    case ANALYSIS_TRAN:
        typeAnalysis = "TRAN"; break;
    }

    desc.append( QString("<b>Analysis type:</b> %1<br/>").arg(typeAnalysis) );

    ui->descriptionLabel->setText( desc );


}

}
