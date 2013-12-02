#ifndef SHOWTEXTDIALOG_H
#define SHOWTEXTDIALOG_H

#include <QDialog>

namespace Ui {
class ShowTextDialog;
}
namespace tsunami{
class ShowTextDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShowTextDialog(const QString& title, const QString& message,QWidget *parent = 0);
    ~ShowTextDialog();

private:
    Ui::ShowTextDialog *ui;
private slots:
    void clippedToClipboard();
    void clickedSaveFile();
};
}

#endif // SHOWTEXTDIALOG_H
