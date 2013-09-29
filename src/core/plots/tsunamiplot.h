#ifndef TSUNAMIPLOT_H
#define TSUNAMIPLOT_H

#include "../components/qcustomplot.h"

#include "graphicitem.h"

class TsunamiPlot : public QCustomPlot
{
public:
    TsunamiPlot();

    void addGraphic( const QString& key, QVector<double> keys, QVector<double> measured,
                     QVector<double> simulated);
    void addGraphic( const QString& key, QVector<double> keys, QVector<double> measured);

    void hideGraphic(const QString& key);
    void showGraphic(const QString& key);

    void hideGraphics();
    void showGraphics();

    void setCurrentGraph(const QString& key);



private:

    GraphicItem* findGraphicItem( const QString& key);
    QList<GraphicItem*> graphics_;

    GraphicItem* currentGraphic_;
};

#endif // TSUNAMIPLOT_H
