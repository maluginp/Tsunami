#ifndef ADDMEASUREFORM_H
#define ADDMEASUREFORM_H

#include <QWidget>


namespace Ui {
class addMeasureForm;
}

namespace tsunami{

namespace db{
    class AnalysisModel;
    class MeasureModel;
    class MeasureStorage;
}
namespace gui{
    class KeyValueView;
    class MeasureItemView;
    class KeyValuePair;
}

class addMeasureForm : public QWidget {
    Q_OBJECT
public:
    enum Action{
        NEW,
        EDIT
    };

    addMeasureForm(Action action, int id, QWidget *parent = 0);
    ~addMeasureForm();
private:
    int analysisId_;
    bool isReadOnlyColumn( const QString& column );
    void openAnalysis(int analysisId);
    void openMeasure(int measureId);

    Ui::addMeasureForm *ui;

    Action action_;

    gui::KeyValueView* headerView_;
    gui::KeyValueView* attributesView_;
    gui::MeasureItemView* measureView_;

    db::MeasureModel* measure_;
    db::MeasureStorage* measureStorage_;

    static gui::KeyValuePair headerPairs_[];
    static const int nPairs_;
private slots:
    void addButtonClick();
};

}



#endif // ADDMEASUREFORM_H
