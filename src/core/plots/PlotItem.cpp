#include "PlotItem.h"
#include "TsunamiPlot.h"

PlotItem::PlotItem(TsunamiPlot *plotter, const QString &name, PlotItem::PlotType type) :
    plotter_(plotter),name_(name),type_(type)
{
    axes_.insert(AXIS_X, "");
    axes_.insert(AXIS_Y, "");
}

void PlotItem::setKeys(const QVector<double> &keys) {
    keys_ = keys;

    int numberKeys = keys.count();
    measured_.clear(); simulated_.clear();
    measured_.reserve( numberKeys );
    simulated_.reserve( numberKeys );

    for(int i=0; i < numberKeys; ++i){
        measured_[i] = DBL_MAX;
        simulated_[i] = DBL_MAX;
    }

}

int PlotItem::numberKeys() {
    return keys_.count();
}

const double &PlotItem::key(int index) const {
    return keys_[index];
}

void PlotItem::setRow(double key, double simulate, double measure) {
    int index = keys_.indexOf(key);
    Q_ASSERT(index != -1);

    measured_[index] = measure;
    simulated_[index] = simulate;

}

void PlotItem::addRow(double key, double simulate, double measure) {
    keys_.append( key );
    simulated_.append( simulate );
    measured_.append( measure );
}

void PlotItem::hide( PlotItem::PlotType type ) {
    if( type == PLOT_ALL ){
        hide(PLOT_MEASURE);
        hide(PLOT_SIMULATE);
    }

    if( isType(type) ){
        QCPGraph* graph =  findGraph( type );
        if(graph != 0){
            plotter_->removeGraph( graph );
        }
    }

}

void PlotItem::build(PlotItem::PlotType type) {
    if( type == PLOT_ALL ){
        build(PLOT_MEASURE);
        build(PLOT_SIMULATE);
    }

    if( isType(type) ){
        QCPGraph* graph = findGraph(type);
        if(graph != 0){
            plotter_->removeGraph( graph );
        }

        graph = plotter_->addGraph( axis( AXIS_X ), axis( AXIS_Y ) );
        graph->setProperty( "name", name_ );
        graph->setProperty( "type", type );

        setStyleGraph( type, graph );

        QVector<double> keys = preparedVector( AXIS_X, type ),
                values = preparedVector( AXIS_Y, type );

        graph->addData( keys, values );
    }

}

void PlotItem::setAxis(PlotItem::PlotAxis axis, const QString &title) {
    axes_[axis] = title;
}

QPoint PlotItem::rangeAxis(PlotItem::PlotAxis axis){
    return QPoint();
}

bool PlotItem::isType(PlotItem::PlotType type) {
    return ((type_ & type) == type);
}

QCPAxis *PlotItem::axis(PlotItem::PlotAxis axis) {
    QCPAxis* plotAxis = 0;
//    if(axis == AXIS_X){
//        plotAxis = new QCPAxis(0,QCPAxis::atBottom);
//    }else if(axis == AXIS_Y){
//        plotAxis = new QCPAxis(0,QCPAxis::atLeft);
//    }

    return plotAxis;
}

QCPGraph* PlotItem::findGraph(PlotItem::PlotType type) {
    int nGraphs = plotter_->graphCount();
    for( int i=0; i < nGraphs; ++i ){
        if( plotter_->graph(i)->property("name").toString().compare(name_) == 0 &&
            plotter_->graph(i)->property("type").toInt() == type ){

            return plotter_->graph(i);
        }
    }
    return 0;
}

QVector<double> PlotItem::preparedVector(PlotItem::PlotAxis axis, PlotItem::PlotType type){

    if(axis == AXIS_X && type == PLOT_ALL){
        QVector<double> keys = keys_;
        for(int i=0; i < keys_.count(); ++i){
            if( fabs( keys_[i] ) != DBL_MAX ){
                keys << keys_[i];
            }
        }

        return keys;
    }

    if( type == PLOT_MEASURE || type == PLOT_SIMULATE ){

        QVector<double> keys, values;
        for(int i=0; i < keys_.count(); ++i){
            if( fabs( keys_[i] ) != DBL_MAX){

                if( type == PLOT_MEASURE && fabs(measured_[i]) != DBL_MAX){
                    keys << keys_[i];
                    values << measured_[i];
                }else if(type == PLOT_SIMULATE && fabs(simulated_[i]) != DBL_MAX){
                    keys << keys_[i];
                    values << simulated_[i];
                }

            }
        }

        if( axis == AXIS_X ){
            return keys;
        }else{
            return values;
        }

    }


    return QVector<double>();
}

void PlotItem::setStyleGraph(PlotItem::PlotType type, QCPGraph *graph) {
    if(type == PLOT_MEASURE){
        graph->setLineStyle(QCPGraph::lsNone);
        graph->setScatterStyle( QCPScatterStyle(QCPScatterStyle::ssCross,3) );
    }else if(type == PLOT_SIMULATE){
        graph->setLineStyle(QCPGraph::lsLine);
    }
}
