#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
