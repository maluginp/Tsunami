#ifndef ANALYSISFORM_H
#define ANALYSISFORM_H

#include <QWidget>
#include <views/listitemview.h>
#include <views/keyvalueview.h>
#include <dbstorage/analysisstorage.h>

namespace Ui {
class AnalysisForm;
}

namespace tsunami{
namespace gui{


class AnalysisForm : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisForm(QWidget *parent = 0);
    void openAnalysis(const int& analysisId);
    ~AnalysisForm();
private:
    void prepareItemsInput();
    void prepareItemsOutput();
    void openAnalysisImpl(const AnalysisModel& model);


    Ui::AnalysisForm *ui;
    ListItemView* itemsInputView_;
    ListItemView* itemsOutputView_;

    KeyValueView* itemValueInputView_;
    KeyValueView* itemValueOutputView_;

    AnalysisModel model_;
    AnalysisModel storedModel_;

    AnalysisStorage* storage_;

    static KeyValuePair ITEM_TYPE_SWEEP[];
    static KeyValuePair ITEM_TYPE_CONST[];
    static KeyValuePair ITEM_TYPE_OUTPUT[];
//    static KeyValuePair ITEM_TYPE_FUNC[];


private slots:

    void changeTypeItemInput( const int& index );
    void changeTypeItemOutput( const int& index );
    void insertInputItemClick();
    void removeInputItemRemove();
    void selectedItemInput( const QModelIndex& index );
    void selectedItemOutput( const QModelIndex& index );

    void changeNameAnalysis(const QString& name);
    void changeTypeAnalysis(const int& index);

    void insertOutputItemClick();
    void removeOutputItemClick();

    void saveAnalysis();
    void restoreAnalysis();
};

}
}


#endif // ANALYSISFORM_H
