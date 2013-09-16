#ifndef MEASUREDATAWINDOW_H
#define MEASUREDATAWINDOW_H

#include <QMainWindow>

namespace Ui {
class MeasureDataWindow;
}

class MeasureDataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MeasureDataWindow(QWidget *parent = 0);
    ~MeasureDataWindow();

private:
    Ui::MeasureDataWindow *ui;
};

#endif // MEASUREDATAWINDOW_H
