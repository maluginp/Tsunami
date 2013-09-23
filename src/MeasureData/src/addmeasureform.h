#ifndef ADDMEASUREFORM_H
#define ADDMEASUREFORM_H

#include <QWidget>
#include <views/keyvalueview.h>
#include <views/measureitemview.h>
class AnalysisModel;
namespace Ui {
class addMeasureForm;
}

class addMeasureForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit addMeasureForm(const int& analysisId, QWidget *parent = 0);
    ~addMeasureForm();
    
private:
    Ui::addMeasureForm *ui;

    KeyValueView* headerView_;
    MeasureItemView* measureView_;

    static KeyValuePair headerPairs_[];
    static const int nPairs_;

};

#endif // ADDMEASUREFORM_H
