#ifndef PLOTEXTRACTIONDIALOG_H
#define PLOTEXTRACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class PlotExtractionDialog;
}

class PlotExtractionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotExtractionDialog(QWidget *parent = 0);
    ~PlotExtractionDialog();

private:
    Ui::PlotExtractionDialog *ui;
};

#endif // PLOTEXTRACTIONDIALOG_H
