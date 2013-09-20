#ifndef ADDMEASUREFORM_H
#define ADDMEASUREFORM_H

#include <QWidget>

namespace Ui {
class addMeasureForm;
}

class addMeasureForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit addMeasureForm(QWidget *parent = 0);
    ~addMeasureForm();
    
private:
    Ui::addMeasureForm *ui;
};

#endif // ADDMEASUREFORM_H
