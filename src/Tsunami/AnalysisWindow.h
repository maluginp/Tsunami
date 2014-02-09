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
    void updateAnalysisList();
    void showSource(const QString& node);

    QPushButton *getNodeButton(int index);
    QComboBox* getNodeComboBox(int index);
    Ui::AnalysisWindow *ui;
    gui::ListItemView* listAnalysis_;
    db::AnalysisStorage* storage_;
    int deviceId_;
    int analysisId_;
    db::AnalysisModel* currentAnalysis_;

    gui::KeyValueView* sourceConfigurationView_;

    APIObject* api_;
//    QStringList nodes_;

    QMap<int,QString> nodes_;
    QMap<QString, Source*> sources_;
private slots:
    void clickedOpenAnalysis();
    void clickedSaveAnalysis(const QList<tsunami::Source>& sources);
    void clickedCreateAnalysis();

    void selectedAnalysisItem( const QModelIndex& index );
    void loadStarted();
    void loadFinished(bool);
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
