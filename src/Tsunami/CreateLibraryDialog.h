#ifndef CREATELIBRARYDIALOG_H
#define CREATELIBRARYDIALOG_H

#include <QDialog>
#include <dbstorage/parameterstorage.h>
namespace Ui {
class CreateLibraryDialog;
}

namespace tsunami{
class CreateLibraryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateLibraryDialog(int deviceId, QWidget *parent = 0);
    ~CreateLibraryDialog();
    db::LibraryModel* library(){ return library_; }
private:
    Ui::CreateLibraryDialog *ui;
    db::ParameterStorage* storage_;
    db::LibraryModel* library_;
    int deviceId_;
private slots:
    void clickedCreateButton();
    void changedLibraryCopied(int copied);
};
}
#endif // CREATELIBRARYDIALOG_H
