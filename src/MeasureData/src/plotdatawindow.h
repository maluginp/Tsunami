#ifndef PLOTDATAWINDOW_H
#define PLOTDATAWINDOW_H

#include <QMainWindow>
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

};

#endif // PLOTDATAWINDOW_H
