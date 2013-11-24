#ifndef ADDMEASUREFORM_H
#define ADDMEASUREFORM_H

#include <QWidget>


namespace Ui {
class addMeasureForm;
}

namespace tsunami{

namespace db{
    class AnalysisModel;
}
namespace gui{
    class KeyValueView;
    class MeasureItemView;
    class KeyValuePair;
}

class addMeasureForm : public QWidget {
    Q_OBJECT
public:
    addMeasureForm(int analysisId, QWidget *parent = 0);
    ~addMeasureForm();
private:
    int analysisId_;
    void openAnalysis(int analysisId);
    Ui::addMeasureForm *ui;

    gui::KeyValueView* headerView_;
    gui::KeyValueView* attributesView_;
    gui::MeasureItemView* measureView_;


    static gui::KeyValuePair headerPairs_[];
    static const int nPairs_;
private slots:
//    void addButtonClick();
};

}



#endif // ADDMEASUREFORM_H
