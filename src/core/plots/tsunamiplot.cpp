#include "tsunamiplot.h"

TsunamiPlot::TsunamiPlot()
{
}

void TsunamiPlot::addGraphic(const QString &key, QVector<double> keys,
                             QVector<double> measured, QVector<double> simulated) {
    GraphicItem* graph = new GraphicItem(key,this, keys, measured,simulated);
    graphics_.append( graph );
}

void TsunamiPlot::addGraphic(const QString &key, QVector<double> keys,
                             QVector<double> measured) {
    GraphicItem* graph = new GraphicItem( key, this, keys, measured, true );
    graphics_.append( graph );
}

void TsunamiPlot::hideGraphic(const QString &key) {
    GraphicItem* graph = findGraphicItem(key);
    if(graph){
        graph->hideGraphics();
    }
}

void TsunamiPlot::showGraphic(const QString &key) {
    GraphicItem* graph = findGraphicItem(key);
    if(graph){
        graph->showGraphics();
    }
}

void TsunamiPlot::hideGraphics() {
    foreach(GraphicItem* graph, graphics_){
        graph->hideGraphics();
    }
}

void TsunamiPlot::showGraphics() {
    foreach(GraphicItem* graph, graphics_){
        graph->showGraphics();
    }
}

void TsunamiPlot::setCurrentGraph(const QString &key) {
    currentGraphic_ = findGraphicItem( key );
}

GraphicItem *TsunamiPlot::findGraphicItem(const QString &key) {
    foreach(GraphicItem* item, graphics_){
        if(item->key().compare( key, Qt::CaseInsensitive ) == 0){
            return item;
        }
    }
    return NULL;
}

