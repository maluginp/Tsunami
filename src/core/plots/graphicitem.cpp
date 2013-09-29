#include "graphicitem.h"

#include "tsunamiplot.h"


GraphicItem::GraphicItem(const QString &key, TsunamiPlot *plotter) :
    plotter_(plotter), key_(key),enableMeasure_(false),enableSimulation_(false),
    axisX_(NULL),axisY_(NULL),graphMeasure_(NULL),graphSimulation_(NULL) {

}

GraphicItem::GraphicItem(const QString &key, TsunamiPlot *plotter, QVector<double> keys,
                         QVector<double> measured, QVector<double> simulated,
                         QCPAxis *axisX, QCPAxis *axisY, bool build) :
    plotter_(plotter), key_(key),keys_(keys), measured_(measured),
    enableMeasure_(true),enableSimulation_(false), axisX_(axisX),axisY_(axisY),
    graphMeasure_(NULL),graphSimulation_(NULL) {

    if(build) {
        buildGraphicImpl("measure" );
        buildGraphicImpl("simulation" );
    }

}

GraphicItem::GraphicItem(const QString &key, TsunamiPlot *plotter, QVector<double> keys,
                         QVector<double> measured, bool build) :
    plotter_(plotter), key_(key),keys_(keys), measured_(measured),
    enableMeasure_(true),enableSimulation_(false), axisX_(NULL),axisY_(NULL),
    graphMeasure_(NULL),graphSimulation_(NULL) {

    if(build){
        buildGraphicImpl( "measure" );
    }

}

GraphicItem::GraphicItem(const QString &key, TsunamiPlot *plotter, QVector<double> keys,
                         QVector<double> measured, QVector<double> simulated, bool build) :
    plotter_(plotter), key_(key),keys_(keys), measured_(measured), simulated_(simulated),
    enableMeasure_(true), enableSimulation_(true), axisX_(NULL),axisY_(NULL),
    graphMeasure_(NULL), graphSimulation_(NULL) {

    if(build) {
        buildGraphicImpl("measure" );
        buildGraphicImpl("simulation" );
    }

}


void GraphicItem::setData(QVector<double> keys, QVector<double> measured, QVector<double> simulated) {
    keys_      = keys;
    measured_  = measured;
    simulated_ = simulated;
}

void GraphicItem::setData(QVector<double> keys, QVector<double> measured) {
    keys_      = keys;
    measured_  = measured;
    // FIXME temporary solution
    simulated_ = QVector<double>(keys.size(),0.0);
}

void GraphicItem::setAxis(QCPAxis *axisX, QCPAxis *axisY) {
    setAxisImpl(axisX,axisY);
}

void GraphicItem::setAxisX(QCPAxis *axis) {
    setAxisImpl( axis, NULL );
}

void GraphicItem::setAxisY(QCPAxis *axis) {
    setAxisImpl( NULL, axis );
}

const QString &GraphicItem::key() {
    return key_;
}

void GraphicItem::enableGraphicImpl(const QString &type, bool isShow)
{
    if(type.compare("measure",Qt::CaseInsensitive) == 0){
        if( isShow == enableMeasure_ ){ return; }
        enableMeasure_ = isShow;
    }else if(type.compare("simulation",Qt::CaseInsensitive) == 0){
        if( isShow == enableSimulation_ ){ return; }
        enableSimulation_ = isShow;
    }else{
        return;
    }

    if(isShow){
        buildGraphicImpl( type );
    }else{
        removeGraphicImpl( type );
    }
}

bool GraphicItem::buildGraphicImpl(const QString &type) {

    if(type.compare("measure",Qt::CaseInsensitive) == 0){
        if( measured_.size() <= 2 ){
            return false;
        }
    }else if(type.compare("simulation",Qt::CaseInsensitive) == 0){
        if( simulated_.size() <= 2){
            return false;
        }
    }

    removeGraphicImpl( type );
    if(type.compare("measure",Qt::CaseInsensitive) == 0){
        graphMeasure_    = plotter_->addGraph( axisX_, axisY_ );
        graphMeasure_->addData( keys_, measured_ );
        graphMeasure_->setProperty( "key",  key_ );
        graphMeasure_->setProperty( "type", type);

    }else if(type.compare("simulation",Qt::CaseInsensitive) == 0){
        graphSimulation_ = plotter_->addGraph( axisX_, axisY_ );
        graphSimulation_->addData( keys_, simulated_ );
        graphSimulation_->setProperty("key",key_);
        graphSimulation_->setProperty("type",type);
    }else{
        return false;
    }
    return true;
}

void GraphicItem::removeGraphicImpl(const QString &type) {
    int position = -1;
    if(!findGraphic(type,position)){
        plotter_->removeGraph( position );
    }
}

void GraphicItem::setAxisImpl(QCPAxis *axisX, QCPAxis *axisY) {
    if(!axisX){
        axisX_ = axisY;
    }
    if(!axisY){
        axisY_ = axisY;
    }

}


QCPGraph *GraphicItem::findGraphic(const QString &type) {
    int position = -1;
    return findGraphic(type,position);
}

QCPGraph *GraphicItem::findGraphic(const QString &type, int &position) {
    int nGraphics = plotter_->graphCount();
    position = -1;
    for( int i=0; i < nGraphics; ++i ){
        if(plotter_->graph(i)->property("key").toString().compare(key_) == 0){
            if(type.compare("measure",Qt::CaseInsensitive) == 0 &&
                    plotter_->graph(i)->property("type").toString().compare("measure") == 0){
                position = i;
                return plotter_->graph(i);
            }else if(type.compare("simulation",Qt::CaseInsensitive) == 0 &&
                     plotter_->graph(i)->property("type").toString().compare("simulation") == 0){
                position = i;
                return plotter_->graph(i);
            }
        }
    }

    return NULL;

}


GraphicItem &GraphicItem::addData(double key, double measured) {
    keys_.append( key );
    measured_.append( measured );
    //FIXME temporary solution
    simulated_.append( 0.0 );
    return *this;
}

GraphicItem &GraphicItem::addData(double key, double measured, double simulated) {
    keys_.append( key );
    measured_.append( measured );
    simulated_.append( simulated );

    return *this;
}

void GraphicItem::showGraphic(const QString &type) {
    enableGraphicImpl( type, true );
}

void GraphicItem::showGraphics() {
    enableGraphicImpl( "measure", true );
    enableGraphicImpl( "simulation", true );
}

void GraphicItem::hideGraphic(const QString &type) {
    enableGraphicImpl( type, false );
}

void GraphicItem::hideGraphics() {
    enableGraphicImpl( "measure",    false );
    enableGraphicImpl( "simulation", false );
}
