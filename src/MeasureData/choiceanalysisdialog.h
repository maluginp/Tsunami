#ifndef CHOICEANALYSISDIALOG_H
#define CHOICEANALYSISDIALOG_H

#include <QDialog>

namespace Ui {
class choiceAnalysisDialog;
}

class choiceAnalysisDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit choiceAnalysisDialog(QWidget *parent = 0);
    ~choiceAnalysisDialog();
    
    int getAnalysisId();
    static int open();
private: 
    Ui::choiceAnalysisDialog *ui;

};

#endif // CHOICEANALYSISDIALOG_H
