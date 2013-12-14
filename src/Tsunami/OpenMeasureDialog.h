#ifndef OPENMEASUREDIALOG_H
#define OPENMEASUREDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include "dbstorage/MeasureStorage.h"
#include "defines.h"

namespace Ui {
class OpenMeasureDialog;
}

namespace tsunami{
namespace gui{
    class ListItemView;
}
class OpenMeasureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenMeasureDialog(int deviceId, QWidget *parent = 0);
    ~OpenMeasureDialog();
    static int getMeasureId(int deviceId, QWidget *parent);
private:
    Ui::OpenMeasureDialog *ui;
    gui::ListItemView* measuresView_;
    int measureId_,
        deviceId_;
    db::MeasureStorage* storage_;
private slots:
    void clickedMeasureItem( const QModelIndex& index );
    void clickedDoubleMeasureItem( const QModelIndex& index);
    void changeMeasureName(const QString& name);
    void changeMeasureAnalysis(int index);
};

}

#endif // OPENMEASUREDIALOG_H
