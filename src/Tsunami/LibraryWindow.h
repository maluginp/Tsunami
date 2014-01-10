#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

#include <QMainWindow>

namespace Ui {
class LibraryWindow;
}
namespace tsunami {
namespace db{
    class ParameterStorage;
    class LibraryModel;
}
namespace gui{
    class ParameterItemView;
}
class LibraryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LibraryWindow(int deviceId, QWidget *parent = 0);
    ~LibraryWindow();
    void openLibrary(int libraryId);

public slots:
    void clickedNewLibraryAction();

private:
    void showParameters(db::LibraryModel* library);
    void enableButtons(bool enable);

    Ui::LibraryWindow *ui;
    db::ParameterStorage* storage_;
    db::LibraryModel* library_;
    gui::ParameterItemView* parameters_;

    int deviceId_;

private slots:
    void clickedOpenLibraryAction();
    void clickedAddParameterAction();
    void clickedRemoveParameterAction();
    void clickedExportNetList();
    void clickedSaveButton();
    void clickedSearchButton();
    void clickedRemoveLibrary();
    void clickedCloseLibrary();
};

}
#endif // LIBRARYWINDOW_H
