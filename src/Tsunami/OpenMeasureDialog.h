#ifndef OPENMEASUREDIALOG_H
#define OPENMEASUREDIALOG_H

#include <QDialog>
#include <QModelIndex>

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
    static int getMeasureId(int deviceId);
private:
    Ui::OpenMeasureDialog *ui;
    gui::ListItemView* measuresView_;
    int measureId_;
private slots:
    void clickedMeasureItem( const QModelIndex& index );
};

}

#endif // OPENMEASUREDIALOG_H
