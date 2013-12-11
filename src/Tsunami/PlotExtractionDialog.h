#ifndef PLOTEXTRACTIONDIALOG_H
#define PLOTEXTRACTIONDIALOG_H

#include <QDialog>
#include "dbstorage/DbStorages.h"
#include "spice/simulator.h"

namespace Ui {
class PlotExtractionDialog;
}
namespace tsunami{

class PlotExtractionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotExtractionDialog(int deviceId, db::LibraryModel* library, const QList<int>& measures, QWidget *parent = 0);
    ~PlotExtractionDialog();

    db::MeasureModel *simulate();
private:
    int deviceId_;
    Ui::PlotExtractionDialog *ui;
    QList<db::MeasureModel*> measures_;
    db::LibraryModel* library_;
    spice::Simulator* simulator_;
private slots:
    void clickedBuildButton();
    void changedMeasure(int index);
};
}
#endif // PLOTEXTRACTIONDIALOG_H
