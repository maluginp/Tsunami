#include "choiceanalysisform.h"
#include "ui_choiceanalysisform.h"

ChoiceAnalysisForm::ChoiceAnalysisForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceAnalysisForm)
{
    ui->setupUi(this);
}

ChoiceAnalysisForm::~ChoiceAnalysisForm()
{
    delete ui;
}
