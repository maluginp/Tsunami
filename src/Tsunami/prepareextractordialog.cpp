#include "prepareextractordialog.h"
#include "ui_prepareextractordialog.h"

PrepareExtractorDialog::PrepareExtractorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareExtractorDialog)
{
    ui->setupUi(this);
}

PrepareExtractorDialog::~PrepareExtractorDialog()
{
    delete ui;
}
