#include "plotdatawindow.h"
#include "ui_plotdatawindow.h"

PlotDataWindow::PlotDataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotDataWindow)
{
    ui->setupUi(this);

    ui->splitter_3->widget(0)->setMaximumWidth(250);
    ui->splitter_2->widget(0)->setMaximumWidth(10000);
    ui->splitter_2->widget(1)->setMaximumWidth(300);

}

PlotDataWindow::~PlotDataWindow()
{
    delete ui;
}
