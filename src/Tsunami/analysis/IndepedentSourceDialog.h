#ifndef INDEPEDENTSOURCEDIALOG_H
#define INDEPEDENTSOURCEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

namespace Ui {
class IndepedentSourceDialog;
}

class IndepedentSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IndepedentSourceDialog(QWidget *parent = 0);
    ~IndepedentSourceDialog();

private:
    void sourceVoltage();

    void showConst();
    void showPulse();
    void showExp();
    void showSin();

    void hiddenAllParameters(bool hide = true);
    void hiddenParameter( int index, bool hide = true );

    QLabel* parameterLabel(int index);
    QLineEdit* parameterLineEdit(int index);
    //    void hideLabels(int count);

    QString sourceType();

    Ui::IndepedentSourceDialog *ui;
    QString type_;
private slots:
    void clickedSaveButton();
    void toggledSourceType(bool toggled);
};

#endif // INDEPEDENTSOURCEDIALOG_H
