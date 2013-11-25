#include "librarywindow.h"
#include "ui_librarywindow.h"
#include "dbstorage/parameterstorage.h"
#include "views/parameteritemview.h"
#include "delegates/delegatecheckbox.h"
namespace tsunami{

LibraryWindow::LibraryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LibraryWindow) {
    ui->setupUi(this);
    storage_ = db::ParameterStorage::instance();


    connect(ui->actionOpenLibrary,SIGNAL(triggered()),
            this,SLOT(clickedOpenLibraryAction()));
}

LibraryWindow::~LibraryWindow() {
    delete ui;
}


void LibraryWindow::clickedOpenLibraryAction() {
    int libraryId = 1;
    library_ = storage_->openLibrary(libraryId);

    ui->libraryNameText->setText( library_->name() );

    parameters_ = new gui::ParameterItemView(library_);
    ui->parametersTableView->setModel( parameters_ );
    ui->parametersTableView->setItemDelegateForColumn(0, new DelegateCheckBox(ui->parametersTableView));
    ui->parametersTableView->setItemDelegateForColumn(6, new DelegateCheckBox(ui->parametersTableView));
    ui->parametersTableView->setColumnWidth(0,30);

}

}
