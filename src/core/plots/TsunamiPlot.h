#ifndef TSUNAMIPLOT_H
#define TSUNAMIPLOT_H

#include "../components/qcustomplot.h"

#include "PlotItem.h"

class TSUNAMI_EXPORT TsunamiPlot : public QCustomPlot {
public:
    explicit TsunamiPlot( QWidget *parent = 0 );

    PlotItem* addPlot(const QString &name , PlotItem::PlotType type = PlotItem::PLOT_ALL);
    PlotItem* plot(const QString& name , PlotItem::PlotType type = PlotItem::PLOT_ALL);

    void clearPlots();
    void setLabelAxisX(const QString& key);
    void setLabelAxisY(const QString& value);

public slots:
    void build();
private:
    QList<PlotItem*> plots_;
    PlotItem* currentGraphic_;
};

#endif // TSUNAMIPLOT_H
