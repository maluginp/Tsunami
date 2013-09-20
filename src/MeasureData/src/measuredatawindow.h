#ifndef MEASUREDATAWINDOW_H
#define MEASUREDATAWINDOW_H

#include <QMainWindow>
#include <views/parameteritemview.h>
#include <views/measureitemview.h>


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


    ParameterItemView* parameterView_;

};

#endif // MEASUREDATAWINDOW_H
