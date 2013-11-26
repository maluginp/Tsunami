#include "openlibrarydialog.h"
#include "ui_openlibrarydialog.h"
#include "dbstorage/parameterstorage.h"
#include "views/listitemview.h"

namespace tsunami{
OpenLibraryDialog::OpenLibraryDialog(int deviceId,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenLibraryDialog),
    deviceId_(deviceId)

{
    ui->setupUi(this);

    storage_ = db::ParameterStorage::instance();
    libraries_ = new gui::ListItemView();

    ui->listLibrarys->setModel( libraries_ );

    updateList();

    connect(ui->listLibrarys,SIGNAL(clicked(QModelIndex)),
            this,SLOT(selectedLibrary(QModelIndex)));
    connect(ui->choiceButton,SIGNAL(clicked()),this,SLOT(accept()));
}

OpenLibraryDialog::~OpenLibraryDialog()
{
    delete ui;
}

int OpenLibraryDialog::getLibraryId(int deviceId) {
    OpenLibraryDialog dialog(deviceId);
    int libraryId = -1;
    if(dialog.exec() == QDialog::Accepted){
        libraryId = dialog.libraryId_;
    }

    dialog.close();
    return libraryId;

}

void OpenLibraryDialog::updateList() {

    libraries_->clear();

    QMap<int, QString> list =storage_->listLibraries(deviceId_);
    foreach(QString name,list.values()){
        libraries_->addItem( name, list.key(name) );
    }


}

void OpenLibraryDialog::showDescriptionLibrary(int libraryId) {
    db::LibraryModel* library = storage_->openLibrary(libraryId);

    ui->descriptionLibraryText->setText( QString("<b>Library name:</b> %1").arg(library->name()) );
    delete library;
}

void OpenLibraryDialog::selectedLibrary(const QModelIndex &index) {
    int libraryId = index.data(Qt::EditRole).toInt();

    libraryId_ = libraryId;
    showDescriptionLibrary(libraryId_);

}

}
