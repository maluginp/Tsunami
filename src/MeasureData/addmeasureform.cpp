#include "addmeasureform.h"
#include "ui_addmeasureform.h"

addMeasureForm::addMeasureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMeasureForm)
{
    ui->setupUi(this);
}

addMeasureForm::~addMeasureForm()
{
    delete ui;
}
