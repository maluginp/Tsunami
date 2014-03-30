#ifndef EXTRACTORWINDOW_H
#define EXTRACTORWINDOW_H

#include <QMainWindow>
#include "defines.h"

namespace Ui {
class ExtractorWindow;
}
namespace tsunami {
namespace gui{
    class  ParametersExtractionView;
}
namespace db{
    class SettingStorage;
    class LibraryModel;
}
namespace core{
    class Extractor;
}
class ExtractorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExtractorWindow(DeviceType type, int libraryId, QList<int> measures, const QString &optimize, QWidget *parent = 0);
    ~ExtractorWindow();

private:
    bool running_;
    int libraryId_;
    QList<int> measures_;
    QString optimize_;
    DeviceType type_;
    Ui::ExtractorWindow *ui;
    db::LibraryModel* library_;
    gui::ParametersExtractionView* parameters_;
    db::SettingStorage* settings_;
private slots:
    void finishExtraction();
    void clickedSave();

    void clickedStop();
    void runExtraction();
    void log( const QString& log );
    void clickedCancel();
    void clickedShowPlotButton();
    void clickedCloseButton();
signals:
    void stop();
};

}


#endif // EXTRACTORWINDOW_H
