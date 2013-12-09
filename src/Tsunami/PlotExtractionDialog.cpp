#include "PlotExtractionDialog.h"
#include "ui_PlotExtractionDialog.h"

PlotExtractionDialog::PlotExtractionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotExtractionDialog)
{
    ui->setupUi(this);
}

PlotExtractionDialog::~PlotExtractionDialog()
{
    delete ui;
}
