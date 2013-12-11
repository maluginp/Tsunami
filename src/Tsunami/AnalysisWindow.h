#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "views/listitemview.h"
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

    Ui::AnalysisWindow *ui;
    gui::ListItemView* listAnalysis_;
    db::AnalysisStorage* storage_;
    int deviceId_;
    int analysisId_;

    APIObject* api_;
private slots:
    void clickedOpenAnalysis();
    void clickedSaveAnalysis(QList<Source> sources);

    void selectedAnalysisItem( const QModelIndex& index );
    void loadStarted();
    void loadFinished(bool);
signals:
    void pageLoadFinished();

};
}

#endif // ANALYSISWINDOW_H
