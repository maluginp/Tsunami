#ifndef ANALYSISFORM_H
#define ANALYSISFORM_H

#include <QWidget>

namespace Ui {
class AnalysisForm;
}

class AnalysisForm : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisForm(QWidget *parent = 0);
    ~AnalysisForm();

private:
    Ui::AnalysisForm *ui;
};

#endif // ANALYSISFORM_H
