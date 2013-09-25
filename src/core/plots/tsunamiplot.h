#ifndef TSUNAMIPLOT_H
#define TSUNAMIPLOT_H

#include "../components/qcustomplot.h"

#include "graphicitem.h"

class TsunamiPlot : public QCustomPlot
{
public:
    TsunamiPlot();

    void addGraphic( QVector<double> keys, QVector<double> measured,
                     QVector<double> simulated);
    void addGraphic( QVector<double> keys, QVector<double> measured);

    void hideGraphic(const QString& key);
    void showGraphic(const QString& key);

    void hideGraphics( );





private:
    void hideGraphicImpl( const QString& key, bool hide );
    QList<GraphicItem*> graphics_;

    GraphicItem* currentGraphic_;
};

#endif // TSUNAMIPLOT_H
