#ifndef ADDMEASUREFORM_H
#define ADDMEASUREFORM_H

#include <QWidget>
#include <views/keyvalueview.h>
#include <views/measureitemview.h>
#include <models/analysisitems.h>

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

    void prepareAnalysis( const int& analysisId );
    int countAnalysisItem(const QList<IAnalysisItem*>& items, const AnalysisItemType& type);

//    bool sortItems(IAnalysisItem *item1, IAnalysisItem *item2);

    Ui::addMeasureForm *ui;

    KeyValueView* headerView_;
    MeasureItemView* measureView_;

    static KeyValuePair headerPairs_[];
    static const int nPairs_;

};

#endif // ADDMEASUREFORM_H
