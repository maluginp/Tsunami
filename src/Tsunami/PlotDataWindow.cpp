#include "PlotDataWindow.h"
#include "ui_PlotDataWindow.h"
#include "views/ParameterPlotView.h"
#include "views/MeasureItemView.h"
#include <QDebug>

namespace tsunami{

PlotDataWindow::PlotDataWindow(QWidget *parent) :
    QMainWindow(parent) //,
//    ui(new Ui::PlotDataWindow)
{
//    ui->setupUi(this);

//    ui->splitter_3->widget(0)->setMaximumWidth(250);
//    ui->splitter_2->widget(0)->setMaximumWidth(10000);
//    ui->splitter_2->widget(1)->setMaximumWidth(300);

//    QVariantMap params;
//    params.insert("aaa",1e-16);
//    params.insert("bbb",5.0);
//    params.insert("ccc",10.0);
//    params.insert("ddd",11.0);

//    paramPlotView_ = new gui::ParameterPlotView( params );

//    measureView_   = new gui::MeasureItemView( -1 );


//    ui->measureTableView->setModel( measureView_ );
//    ui->parameterTableView->setModel( paramPlotView_ );

//    connect(ui->parameterFilterText,SIGNAL(textChanged(QString)),this,SLOT(findParameter(QString)));

}

PlotDataWindow::~PlotDataWindow() {
//    delete ui;
}

void PlotDataWindow::findParameter(const QString &parameter) {
//    int nParameters = paramPlotView_->rowCount();
//    qDebug() << parameter;

//    for( int i=0; i < nParameters; ++i ){
//        if(paramPlotView_->itemAt(i).first.contains(parameter, Qt::CaseInsensitive)){
//            ui->parameterTableView->setCurrentIndex( paramPlotView_->index(i,2) );
//        }
//    }
}

}
