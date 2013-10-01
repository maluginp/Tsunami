#include "plotdatawindow.h"
#include "ui_plotdatawindow.h"
#include <views/parameterplotview.h>

PlotDataWindow::PlotDataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotDataWindow)
{
    ui->setupUi(this);

    ui->splitter_3->widget(0)->setMaximumWidth(250);
    ui->splitter_2->widget(0)->setMaximumWidth(10000);
    ui->splitter_2->widget(1)->setMaximumWidth(300);

    QVariantMap params;
    params.insert("aaa",1e-16);
    params.insert("bbb",5.0);
    params.insert("ccc",10.0);
    params.insert("ddd",11.0);

    paramPlotView_ = new ParameterPlotView( params );

    ui->parameterTableView->setModel( paramPlotView_ );
}

PlotDataWindow::~PlotDataWindow()
{
    delete ui;
}
