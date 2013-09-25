#ifndef PLOTDATAWINDOW_H
#define PLOTDATAWINDOW_H

#include <QMainWindow>

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
};

#endif // PLOTDATAWINDOW_H
