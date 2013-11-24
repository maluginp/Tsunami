#ifndef PLOTDATAWINDOW_H
#define PLOTDATAWINDOW_H

#include <QMainWindow>


namespace Ui {
class PlotDataWindow;
}

namespace tsunami{
namespace gui{
    class MeasureItemView;
    class ParameterPlotView;
}



class PlotDataWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlotDataWindow(QWidget *parent = 0);
    ~PlotDataWindow();

private:
//    Ui::PlotDataWindow *ui;
    gui::ParameterPlotView* paramPlotView_;
    gui::MeasureItemView* measureView_;
private slots:
    void findParameter( const QString& parameter );
};

}
#endif // PLOTDATAWINDOW_H
