#ifndef GRAPICHITEM_H
#define GRAPICHITEM_H

#include <QVector>
class QCPGraph;
class QCPAxis;
class TsunamiPlot;

class GraphicItem {
public:
    GraphicItem(TsunamiPlot* plotter);

    void setData( QVector<double> keys, QVector<double> measured,
                  QVector<double> simulated );
    void setData( QVector<double> keys, QVector<double> measured );

    GraphicItem& addData(double key, double measured);
    GraphicItem& addData(double key, double measured,double simulated);

    void showGraphic(const QString& type);
    void showGraphicMeasure();
    void showGraphicSimulation();


    void setAxis( QCPAxis* axisX, QCPAxis* axisY );
    void setAxisX(QCPAxis* axis);
    void setAxisY(QCPAxis* axis);


    const QString& key();

private:
    void enableGraphicImpl( const QString& type, bool isShow );
    bool buildGraphicImpl( const QString& type);
    void removeGraphicImpl( const QString& type);
    void setAxisImpl( QCPAxis* axisX, QCPAxis* axisY );

    QCPGraph* findGraphic( const QString& type );
    QCPGraph* findGraphic( const QString& type, int& position );

    QString key_;
    QCPGraph* graphMeasure_;
    QCPGraph* graphSimulation_;

    QCPAxis* axisX_;
    QCPAxis* axisY_;

    bool enableMeasure_;
    bool enableSimulation_;

    TsunamiPlot* plotter_;

    //TODO style
    QVector<double> keys_;
    QVector<double> measured_;
    QVector<double> simulated_;

    double error;
};

#endif // GRAPICHITEM_H
