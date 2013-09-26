#include "tsunamiplot.h"

TsunamiPlot::TsunamiPlot()
{
}

GraphicItem *TsunamiPlot::findGraphicItem(const QString &key) {
    foreach(GraphicItem* item, graphics_){
        if(item->key().compare( key, Qt::CaseInsensitive ) == 0){
            return item;
        }
    }
    return NULL;
}
