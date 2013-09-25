#ifndef GRAPICHITEM_H
#define GRAPICHITEM_H

#include <QVector>
class QCPGraph;
class TsunamiPlot;

class GraphicItem {
public:
    GraphicItem(TsunamiPlot* plotter);



private:
    QString name_;
    QCPGraph* graphMeasure_;
    QCPGraph* graphSimulation_;

    TsunamiPlot* plotter_;

    //TODO style
    QVector<double> keys_;
    QVector<double> measured_;
    QVector<double> simulated_;

    double error;
};

#endif // GRAPICHITEM_H
