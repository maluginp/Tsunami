#include "TsunamiPlot.h"

TsunamiPlot::TsunamiPlot(QWidget *parent) : QCustomPlot(parent) {
//    setupFullAxesBox();

//    setRangeDrag(Qt::Horizontal|Qt::Vertical);
//    setRangeZoom(Qt::Horizontal|Qt::Vertical);
    setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
//    setInteraction( QCP::iSelectAxes | QCP::iZo QCustomPlot::iSelectPlottables );
}

PlotItem *TsunamiPlot::addPlot(const QString &name, PlotItem::PlotType type) {
    PlotItem* plot = new PlotItem(this,name,type);
    plots_.append( plot );

    return plot;
}

PlotItem *TsunamiPlot::plot(const QString &name, PlotItem::PlotType type) {
    foreach(PlotItem* plot, plots_){
        if(plot->name().compare(name,Qt::CaseInsensitive) == 0){
            plot->type( type );
            return plot;
        }
    }

    return addPlot(name,type);
}

void TsunamiPlot::clearPlots() {
    clearGraphs();
    qDeleteAll(plots_);
    plots_.clear();
}

void TsunamiPlot::setLabelAxisX(const QString &key) {
    xAxis->setLabel( key );
}

void TsunamiPlot::setLabelAxisY(const QString &value) {
    yAxis->setLabel( value );
}

void TsunamiPlot::build() {
//    int nGraphs = graphCount();
//    for(int i=0; i < nGraphs; ++i){
//        removeGraph(i);
//    }


    foreach(PlotItem* plot, plots_ ){
        plot->build();
    }
    rescaleAxes(true);
    replot();

}




