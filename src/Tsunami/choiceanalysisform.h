#ifndef CHOICEANALYSISFORM_H
#define CHOICEANALYSISFORM_H

#include <QDialog>
#include "views/listitemview.h"

namespace Ui {
class ChoiceAnalysisForm;
}

namespace tsunami{
namespace db{
class AnalysisStorage;
class AnalysisModel;
}


class ChoiceAnalysisForm : public QDialog {
    Q_OBJECT

public:
    explicit ChoiceAnalysisForm(int deviceId,QWidget *parent = 0);
    ~ChoiceAnalysisForm();

    static int getAnalysisId( int deviceId  );

private slots:
    void clickedAnalysis( const QModelIndex& index );
    void clickedAddButton();
    void clickedOpenButton();
    void clickedEditButton();

private:
    void updateList();
    void showAnalysisDescription(  db::AnalysisModel* model );

    db::AnalysisModel* currentModel_;
    Ui::ChoiceAnalysisForm *ui;
    db::AnalysisStorage* storage_;
    gui::ListItemView *listModel_;
    int deviceId_;
};

}


#endif // CHOICEANALYSISFORM_H
