#include "analysisform.h"
#include "ui_analysisform.h"

AnalysisForm::AnalysisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisForm)
{
    ui->setupUi(this);
}

AnalysisForm::~AnalysisForm()
{
    delete ui;
}
