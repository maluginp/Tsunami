#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
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

private:
    void updateAnalysisList();

    Ui::AnalysisWindow *ui;
    gui::ListItemView* listAnalysis_;
    db::AnalysisStorage* storage_;
    int deviceId_;
    int analysisId_;

    APIObject* api_;
private slots:
    void openAnalysis();
    void loadStarted();
};
}

#endif // ANALYSISWINDOW_H
