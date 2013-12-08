#ifndef PREPAREEXTRACTORDIALOG_H
#define PREPAREEXTRACTORDIALOG_H

#include <QDialog>
#include "views/measureenableview.h"
#include <dbstorage/DbStorages.h>
#include "views/listitemview.h"

namespace Ui {
class PrepareExtractorDialog;
}
namespace tsunami{
namespace gui{
    class MeasureEnableView;
}
class PrepareExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrepareExtractorDialog(int deviceId,QWidget *parent = 0);
    ~PrepareExtractorDialog();


    const int& libraryId() const { return libraryId_; }
    QList<int> measures();
private:
    void loadListLibraries();
    void loadMeasures( const QString& analysis );


    Ui::PrepareExtractorDialog *ui;
    gui::MeasureEnableView *measures_;
    gui::ListItemView* analysisTypeView_;

    int libraryId_;
    int deviceId_;

private slots:
    void clickedRunButton();
    void changedLibraryComboBox(int index);
    void clickedAnalysisType(const QModelIndex& index);
};
}
#endif // PREPAREEXTRACTORDIALOG_H
