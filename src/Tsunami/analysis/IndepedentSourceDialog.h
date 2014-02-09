#ifndef INDEPEDENTSOURCEDIALOG_H
#define INDEPEDENTSOURCEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include "defines.h"

namespace Ui {
class IndepedentSourceDialog;
}

namespace tsunami{
class IndepedentSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IndepedentSourceDialog(Source* source, QWidget *parent = 0);
    ~IndepedentSourceDialog();

private:
    void openSource();

    void sourceVoltage();
    void sourceCurrent();

    void showConst();
    void showPulse();
    void showExp();
    void showSin();

    void showAcSource();

    void hiddenAllParameters(bool hide = true);
    void hiddenParameter( int index, bool hide = true );

    QLabel* parameterLabel(int index);
    QLineEdit* parameterLineEdit(int index);
    //    void hideLabels(int count);

    SourceType sourceType();

    Ui::IndepedentSourceDialog *ui;
    QString type_;
    Source* source_;
private slots:
    void clickedSaveButton();
    void toggledSourceType(bool toggled);
};
}
#endif // INDEPEDENTSOURCEDIALOG_H
