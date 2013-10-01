#include "addplotitemdialog.h"
#include "ui_addplotitemdialog.h"

AddPlotItemDialog::AddPlotItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPlotItemDialog)
{
    ui->setupUi(this);
}

AddPlotItemDialog::~AddPlotItemDialog()
{
    delete ui;
}
