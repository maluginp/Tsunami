#ifndef PREPAREEXTRACTORDIALOG_H
#define PREPAREEXTRACTORDIALOG_H

#include <QDialog>
#include "views/measureenableview.h"
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
    const QString& methodExtraction() const { return methodExtraction_;}
    QList<int> measures();
private:
    void loadListLibraries();
    void loadListExtractionMethods();
    Ui::PrepareExtractorDialog *ui;
    gui::MeasureEnableView *measures_;
    int libraryId_;
    int deviceId_;
    QString methodExtraction_;
private slots:
    void clickedRunButton();
    void changedMethodExtractionComboBox(int index);
    void changedLibraryComboBox(int index);
};
}
#endif // PREPAREEXTRACTORDIALOG_H
