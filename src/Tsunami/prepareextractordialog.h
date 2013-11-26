#ifndef PREPAREEXTRACTORDIALOG_H
#define PREPAREEXTRACTORDIALOG_H

#include <QDialog>

namespace Ui {
class PrepareExtractorDialog;
}

class PrepareExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrepareExtractorDialog(QWidget *parent = 0);
    ~PrepareExtractorDialog();

private:
    Ui::PrepareExtractorDialog *ui;
};

#endif // PREPAREEXTRACTORDIALOG_H
