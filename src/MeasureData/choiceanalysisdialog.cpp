#include "choiceanalysisdialog.h"
#include "ui_choiceanalysisdialog.h"
#include <dbstorage/analysisstorage.h>

choiceAnalysisDialog::choiceAnalysisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choiceAnalysisDialog)
{
    ui->setupUi(this);


    QMap<int,QString> analyses = AnalysisStorage::instance()->listAnalysis();

    QList<QString> values = analyses.values();
    qSort( values.begin(),values.end() );

    foreach( QString value, values ){
        ui->choiceComboxBox->addItem( value, analyses.key( value ) );
    }


    connect(ui->choiceButton,SIGNAL(clicked()),this,SLOT(accept()));
}

choiceAnalysisDialog::~choiceAnalysisDialog()
{
    delete ui;
}

int choiceAnalysisDialog::getAnalysisId() {
    int analysisId = -1;
    if(ui->choiceComboxBox->currentIndex() != -1){

        analysisId = ui->choiceComboxBox->model()->data( ui->choiceComboxBox->model()->index(
                     ui->choiceComboxBox->currentIndex(),0), Qt::UserRole  ).toInt();


    }

    return analysisId;

}

int choiceAnalysisDialog::openDialog() {
    choiceAnalysisDialog dialog(0);
    if(dialog.exec() == QDialog::Accepted){
        return dialog.getAnalysisId();
    }
    return -1;
}
