#include "TsunamiPlot.h"

TsunamiPlot::TsunamiPlot(QWidget *parent) : QCustomPlot(parent) {
//    setupFullAxesBox();

//    setRangeDrag(Qt::Horizontal|Qt::Vertical);
//    setRangeZoom(Qt::Horizontal|Qt::Vertical);
    setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
//    setInteraction( QCP::iSelectAxes | QCP::iZo QCustomPlot::iSelectPlottables );
}

PlotItem *TsunamiPlot::addPlot(const QString &name) {
    PlotItem* plot = new PlotItem(this,name);
    plots_.append( plot );

    return plot;
}

PlotItem *TsunamiPlot::plot(const QString &name) {
    foreach(PlotItem* plot, plots_){
        if(plot->name().compare(name,Qt::CaseInsensitive) == 0){
            return plot;
        }
    }

    return addPlot(name);
}

void TsunamiPlot::clearPlots() {
    clearGraphs();
    qDeleteAll(plots_);
    plots_.clear();
}

void TsunamiPlot::clearPlots() {
    clearGraphs();
    plots_.clear();

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




