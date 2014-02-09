#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "views/ListItemView.h"
#include "views/KeyValueView.h"
#include "webkit/APIObject.h"

#include "analysis/IndepedentSourceDialog.h"

namespace Ui {
class AnalysisWindow;
}
namespace tsunami{
namespace db{
class AnalysisStorage;
class AnalysisModel;
}
class AnalysisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisWindow(int deviceId,QWidget *parent = 0);
    ~AnalysisWindow();
    void openAnalysis(int analysisId);
private:
    void prepareDevice();
    void showSource(const QString& node);
    void hideSource(int index,bool hide = true);
    QPushButton *getNodeButton(int index);
    QComboBox* getNodeComboBox(int index);

    Ui::AnalysisWindow *ui;

    db::AnalysisStorage* storage_;
    db::AnalysisModel* currentAnalysis_;

    int deviceId_;
    int analysisId_;

    QMap<int,QString> nodes_;
    QMap<QString, Source*> sources_;
private slots:
    void clickedOpenAnalysis();
    void clickedSaveAnalysis(const QList<tsunami::Source>& sources);
//    void clickedCreateAnalysis();

//    void selectedAnalysisItem( const QModelIndex& index );

    // -- new
    void changedAnalysisType(int index);
    void checkedSourceSecondEnable(bool checked);

    void changedSourceTypeNode1(int index);
    void changedSourceTypeNode2(int index);
    void changedSourceTypeNode3(int index);
    void changedSourceTypeNode4(int index);

    void changeSourceType(int nodeId, const QString& type);


    void clickedSourceParamsNode1();
    void clickedSourceParamsNode2();
    void clickedSourceParamsNode3();
    void clickedSourceParamsNode4();
    void clickedSourceParams(int nodeId, const QString& type);
signals:
    void pageLoadFinished();
    void updatedDataBase();
};
}

#endif // ANALYSISWINDOW_H
