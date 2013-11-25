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
    explicit LibraryWindow(QWidget *parent = 0);
    ~LibraryWindow();

private:
    Ui::LibraryWindow *ui;
    db::ParameterStorage* storage_;
    db::LibraryModel* library_;
    gui::ParameterItemView* parameters_;
private slots:
    void clickedOpenLibraryAction();
};

}
#endif // LIBRARYWINDOW_H
