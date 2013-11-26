#include "extractorwindow.h"
#include "ui_extractorwindow.h"

ExtractorWindow::ExtractorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExtractorWindow)
{
    ui->setupUi(this);
}

ExtractorWindow::~ExtractorWindow()
{
    delete ui;
}
