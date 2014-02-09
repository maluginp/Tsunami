#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "views/ListItemView.h"
#include "views/KeyValueView.h"
#include "webkit/APIObject.h"

#include "sources/AnalysisVoltagePulse.h"

#include "spice/devices/IndependentSourceDevice.h"

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
    void showSource(const QString& node,const QString& type,const QString& mode);

    Ui::AnalysisWindow *ui;
    gui::ListItemView* listAnalysis_;
    db::AnalysisStorage* storage_;
    int deviceId_;
    int analysisId_;
    db::AnalysisModel* currentAnalysis_;

    gui::KeyValueView* sourceConfigurationView_;

    APIObject* api_;
    QStringList nodes_;

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
    void changedSourceNode(int index);
    void checkedSourceSecondEnable(bool checked);
    void changedSourceType(int index);
    void changedSourceMode(int index);
signals:
    void pageLoadFinished();
    void updatedDataBase();
};
}

#endif // ANALYSISWINDOW_H
