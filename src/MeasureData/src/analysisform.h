#ifndef ANALYSISFORM_H
#define ANALYSISFORM_H

#include <QWidget>
#include <views/listitemview.h>
#include <views/keyvalueview.h>
#include <dbstorage/analysisstorage.h>

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
    void prepareItemsInput();
    void prepareItemsOutput();
    void openAnalysis(const int& analysisId);


    Ui::AnalysisForm *ui;
    ListItemView* itemsInputView_;
    ListItemView* itemsOutputView_;

    KeyValueView* itemInputView_;
    KeyValueView* itemOutputView_;

    AnalysisModel model_;

    AnalysisStorage* storage_;
};

#endif // ANALYSISFORM_H
