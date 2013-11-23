#ifndef CHOICEANALYSISFORM_H
#define CHOICEANALYSISFORM_H

#include <QDialog>

namespace Ui {
class ChoiceAnalysisForm;
}

class ChoiceAnalysisForm : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceAnalysisForm(QWidget *parent = 0);
    ~ChoiceAnalysisForm();

private:
    Ui::ChoiceAnalysisForm *ui;
};

#endif // CHOICEANALYSISFORM_H
