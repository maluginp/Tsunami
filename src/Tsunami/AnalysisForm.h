#ifndef ANALYSISFORM_H
#define ANALYSISFORM_H

#include <QWidget>
#include "views/Views.h"
#include "dbstorage/DbStorages.h"

namespace Ui {
    class AnalysisForm;
}


namespace tsunami{

class AnalysisForm : public QWidget {
    Q_OBJECT

public:
    explicit AnalysisForm(int analysisId, QWidget *parent = 0);
    void openAnalysis(int analysisId);
    ~AnalysisForm();


private:
    Source defaultSource( SourceDirection direction );

    Ui::AnalysisForm *ui;
    gui::ListItemView* itemsInputView_;
    gui::ListItemView* itemsOutputView_;

    gui::KeyValueView* itemValueInputView_;
    gui::KeyValueView* itemValueOutputView_;

    db::AnalysisModel *model_;
    db::AnalysisStorage* storage_;

    static gui::KeyValuePair ITEM_TYPE_LINEAR[];
    static int nTypeLinear_;
    static gui::KeyValuePair ITEM_TYPE_LIST[];
    static int nTypeList_;
    static gui::KeyValuePair ITEM_TYPE_CONST[];
    static int nTypeConst_;
    static gui::KeyValuePair ITEM_TYPE_OUTPUT[];
    static int nTypeOutput_;
//    static KeyValuePair ITEM_TYPE_FUNC[];


private slots:
    void selectedItemInput( const QModelIndex& index );
    void selectedItemOutput( const QModelIndex& index );
    void showSource( const Source& source );



    void changeTypeItemInput( int index );
    void changeTypeItemOutput( int index );
    void insertInputItemClick();
    void removeInputItemRemove();


    void changeNameAnalysis(const QString& name);
    void changeTypeAnalysis(int index);

};

}


#endif // ANALYSISFORM_H
