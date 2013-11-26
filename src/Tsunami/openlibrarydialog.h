#ifndef OPENLIBRARYDIALOG_H
#define OPENLIBRARYDIALOG_H

#include <QDialog>
#include <QModelIndex>
namespace Ui {
class OpenLibraryDialog;
}
namespace tsunami{
namespace db{
    class ParameterStorage;
    class LibraryModel;
}
namespace gui{
    class ListItemView;
}
class OpenLibraryDialog : public QDialog {
    Q_OBJECT

public:
    explicit OpenLibraryDialog(int deviceId, QWidget *parent = 0);
    ~OpenLibraryDialog();
    static int getLibraryId(int deviceId);
private:
    void updateList();
    void showDescriptionLibrary(int libraryId);

    gui::ListItemView* libraries_;
    db::ParameterStorage* storage_;
    Ui::OpenLibraryDialog *ui;

    int deviceId_;
    int libraryId_;
private slots:
    void selectedLibrary(const QModelIndex& index);
};
}

#endif // OPENLIBRARYDIALOG_H
