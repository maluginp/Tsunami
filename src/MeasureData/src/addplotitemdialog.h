#ifndef ADDPLOTITEMDIALOG_H
#define ADDPLOTITEMDIALOG_H

#include <QDialog>

namespace Ui {
class AddPlotItemDialog;
}

class AddPlotItemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddPlotItemDialog(QWidget *parent = 0);
    ~AddPlotItemDialog();
    
private:
    Ui::AddPlotItemDialog *ui;
};

#endif // ADDPLOTITEMDIALOG_H
