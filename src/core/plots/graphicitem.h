#ifndef GRAPICHITEM_H
#define GRAPICHITEM_H

#include <QVector>
#include <QString>

class QCPGraph;
class QCPAxis;
class TsunamiPlot;

//class GraphicItem {
//public:
//    GraphicItem(const QString& key,TsunamiPlot* plotter);
//    GraphicItem(const QString& key,TsunamiPlot* plotter,QVector<double> keys,
//                QVector<double> measured, bool build = false );
//    GraphicItem(const QString& key,TsunamiPlot* plotter,QVector<double> keys,
//                QVector<double> measured, QVector<double> simulated, bool build = false );
//    GraphicItem(const QString& key,TsunamiPlot *plotter, QVector<double> keys,
//                QVector<double> measured, QVector<double> simulated,
//                QCPAxis* axisX, QCPAxis* axisY, bool build = false);


//    void setData( QVector<double> keys, QVector<double> measured,
//                  QVector<double> simulated );
//    void setData( QVector<double> keys, QVector<double> measured );

//    GraphicItem& addData(double key, double measured);
//    GraphicItem& addData(double key, double measured,double simulated);

//    void showGraphic(const QString& type);
//    void showGraphics();
//    void hideGraphic(const QString& type);
//    void hideGraphics();

//    void setAxis( QCPAxis* axisX, QCPAxis* axisY );
//    void setAxisX(QCPAxis* axis);
//    void setAxisY(QCPAxis* axis);


//    const QString& key();

//private:
//    void enableGraphicImpl( const QString& type, bool isShow );
//    bool buildGraphicImpl( const QString& type);
//    void removeGraphicImpl( const QString& type);
//    void setAxisImpl( QCPAxis* axisX, QCPAxis* axisY );

//    QCPGraph* findGraphic( const QString& type );
//    QCPGraph* findGraphic( const QString& type, int& position );

//    QString key_;
//    QCPGraph* graphMeasure_;
//    QCPGraph* graphSimulation_;

//    QCPAxis* axisX_;
//    QCPAxis* axisY_;

//    bool enableMeasure_;
//    bool enableSimulation_;

//    TsunamiPlot* plotter_;

//    //TODO style
//    QVector<double> keys_;
//    QVector<double> measured_;
//    QVector<double> simulated_;

//    double error;
//};

#endif // GRAPICHITEM_H
