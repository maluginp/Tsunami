#ifndef EXTRACTORWINDOW_H
#define EXTRACTORWINDOW_H

#include <QMainWindow>

namespace Ui {
class ExtractorWindow;
}

class ExtractorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExtractorWindow(QWidget *parent = 0);
    ~ExtractorWindow();

private:
    Ui::ExtractorWindow *ui;
};

#endif // EXTRACTORWINDOW_H
