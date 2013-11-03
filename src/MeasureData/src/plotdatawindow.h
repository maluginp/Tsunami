#ifndef PLOTDATAWINDOW_H
#define PLOTDATAWINDOW_H

#include <QMainWindow>
class MeasureItemView;
class ParameterPlotView;

namespace Ui {
class PlotDataWindow;
}

class PlotDataWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlotDataWindow(QWidget *parent = 0);
    ~PlotDataWindow();

private:
    Ui::PlotDataWindow *ui;
    ParameterPlotView* paramPlotView_;
    MeasureItemView* measureView_;
private slots:
    void findParameter( const QString& parameter );
};

#endif // PLOTDATAWINDOW_H
