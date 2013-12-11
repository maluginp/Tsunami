#ifndef TSUNAMIPLOT_H
#define TSUNAMIPLOT_H

#include "../components/qcustomplot.h"

#include "PlotItem.h"

class TsunamiPlot : public QCustomPlot {
public:
    explicit TsunamiPlot( QWidget *parent = 0 );

    PlotItem* addPlot(const QString &name );
    PlotItem* plot( const QString& name );


public slots:
    void build();
private:
    QList<PlotItem*> plots_;
    PlotItem* currentGraphic_;
};

#endif // TSUNAMIPLOT_H
