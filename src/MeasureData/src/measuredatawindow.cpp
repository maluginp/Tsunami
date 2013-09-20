#include "measuredatawindow.h"
#include "ui_measuredatawindow.h"



MeasureDataWindow::MeasureDataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MeasureDataWindow)
{
    ui->setupUi(this);

    ui->plotItemSplitter->widget( 0 )->setMaximumWidth( 300 );
    ui->parameterSplitter->widget(0)->setMaximumWidth( 250 );


    parameterView_ = new ParameterItemView( 0 );

    ui->parameterTableView->setModel( parameterView_ );

}

MeasureDataWindow::~MeasureDataWindow()
{
    delete ui;
}
