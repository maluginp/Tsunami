#include "graphicitem.h"

#include "tsunamiplot.h"


GraphicItem::GraphicItem(TsunamiPlot *plotter)
    : plotter_(plotter) {



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
            }else if(type.compare("measure",Qt::CaseInsensitive) == 0 &&
                     plotter_->graph(i)->property("type").toString().compare("measure") == 0){
                position = i;
                return plotter_->graph(i);
            }
        }
    }

    return NULL;

}

