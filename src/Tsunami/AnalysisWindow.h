#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "views/ListItemView.h"
#include "views/KeyValueView.h"
#include "webkit/APIObject.h"
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
    void showSourceNode(const QString& node);
    void showSourceGround(const QString& node);
    void showSourceConst(const QString& node);
    void showSourcePulse(const QString& node);
    void showSourceExp(const QString& node);

    Ui::AnalysisWindow *ui;
    gui::ListItemView* listAnalysis_;
    db::AnalysisStorage* storage_;
    int deviceId_;
    int analysisId_;
    db::AnalysisModel* currentAnalysis_;

    gui::KeyValueView* sourceConfigurationView_;

    APIObject* api_;

    QStringList nodes_;
private slots:
    void clickedOpenAnalysis();
    void clickedSaveAnalysis(const QList<tsunami::Source>& sources);
    void clickedCreateAnalysis();

    void selectedAnalysisItem( const QModelIndex& index );
    void loadStarted();
    void loadFinished(bool);
    // -- new
    void changedAnalysisType(int index);
    void changedSourceNode(int index);
    void checkedSourceSecondEnable(bool checked);
    void changedSourceType(int index);

signals:
    void pageLoadFinished();
    void updatedDataBase();
};
}

#endif // ANALYSISWINDOW_H
