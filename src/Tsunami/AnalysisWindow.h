#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>

namespace Ui {
class AnalysisWindow;
}

class AnalysisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisWindow(QWidget *parent = 0);
    ~AnalysisWindow();

private:
    Ui::AnalysisWindow *ui;
};

#endif // ANALYSISWINDOW_H
