#ifndef PLOTITEM_H
#define PLOTITEM_H
#include "defines.h"
#include <float.h>
class TsunamiPlot;
class QCPGraph;
class QCPAxis;

class TSUNAMI_EXPORT PlotItem{
public:
    enum PlotType{
        PLOT_MEASURE  = 1 << 0,
        PLOT_SIMULATE = 1 << 1,
        PLOT_ALL = 3
    };
    enum PlotAxis{
        AXIS_X,
        AXIS_Y
    };

    PlotItem( TsunamiPlot* plotter, const QString& name, PlotType type = PLOT_ALL );
    ~PlotItem();
    void type( PlotType type ) { type_ = type; }
    void setKeys(const QVector<double>& keys);
    int numberKeys();
    const double& key(int index) const;

    void setRow( double key, double simulate = DBL_MAX, double measure = DBL_MAX );
    void addRow( double key, double simulate = DBL_MAX, double measure = DBL_MAX );
    void hide(PlotType type = PLOT_ALL);
    void build();


    void setAxis( PlotAxis axis, const QString& title );
    QPoint rangeAxis( PlotAxis axis );

    const QString& name() const { return name_; }

    void setProperty( const QString& key, const QVariant& property );

private:
    bool isType(PlotType type);
    QCPAxis* axis( PlotAxis axis );
    QCPGraph* findGraph( PlotType type );
    QVector<double> preparedVector( PlotAxis axis, PlotType type = PLOT_ALL );
    void build(PlotType type);
    void setStyleGraph( PlotType type, QCPGraph* graph );

    TsunamiPlot* plotter_;
    QString name_;
    PlotType type_;

    QMap<PlotAxis, QString> axes_;
    QVector<double> keys_;
    QVector<double> measured_,simulated_;

    QVariantMap properties_;
};

#endif // PLOTITEM_H
