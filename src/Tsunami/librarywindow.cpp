#include "librarywindow.h"
#include "ui_librarywindow.h"

#include "openlibrarydialog.h"
#include "dbstorage/parameterstorage.h"
#include "views/parameteritemview.h"
#include "delegates/delegatecheckbox.h"
#include "delegates/delegatedoubleitem.h"
namespace tsunami{

LibraryWindow::LibraryWindow(int deviceId,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LibraryWindow), parameters_(NULL) {
    ui->setupUi(this);
    storage_ = db::ParameterStorage::instance();

    deviceId_ = deviceId;




    connect(ui->actionOpenLibrary,SIGNAL(triggered()),this,SLOT(clickedOpenLibraryAction()));
    connect(ui->actionNewLibrary,SIGNAL(triggered()),this, SLOT(clickedNewLibraryAction()));
    connect(ui->actionAddParameter,SIGNAL(triggered()),this,SLOT(clickedAddParameterAction()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(clickedAddParameterAction()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(clickedRemoveParameterAction()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(clickedSaveButton()));
}

LibraryWindow::~LibraryWindow() {
    delete ui;
}

void LibraryWindow::showParameters(db::LibraryModel *library) {
    ui->libraryNameText->setText( library->name() );

    delete parameters_;
    parameters_ = new gui::ParameterItemView(library);
    ui->parametersTableView->setModel( parameters_ );
    ui->parametersTableView->setItemDelegateForColumn(2, new DelegateDoubleItem(ui->parametersTableView));
    ui->parametersTableView->setItemDelegateForColumn(3, new DelegateDoubleItem(ui->parametersTableView));
    ui->parametersTableView->setItemDelegateForColumn(4, new DelegateDoubleItem(ui->parametersTableView));
    ui->parametersTableView->setItemDelegateForColumn(5, new DelegateDoubleItem(ui->parametersTableView));

    ui->parametersTableView->setItemDelegateForColumn(0, new DelegateCheckBox(ui->parametersTableView));
    ui->parametersTableView->setItemDelegateForColumn(6, new DelegateCheckBox(ui->parametersTableView));
    ui->parametersTableView->setColumnWidth(0,30);

}


void LibraryWindow::clickedOpenLibraryAction() {

    int libraryId =  OpenLibraryDialog::getLibraryId(deviceId_);

    if(libraryId == -1){
        Q_ASSERT(false);
    }else{
        library_ = storage_->openLibrary(libraryId);

        showParameters( library_ );
    }

}

void LibraryWindow::clickedNewLibraryAction() {

    QString libraryName = QInputDialog::getText(this,tr("New library"), tr("Name"));

    library_ = new db::LibraryModel(libraryName,deviceId_);
    showParameters(library_);

}

void LibraryWindow::clickedAddParameterAction() {
    parameters_->addEmptyParameter();

}

void LibraryWindow::clickedRemoveParameterAction() {
//    QItemSelectionModel* selectedItems = ui->parametersTableView->selectionModel();

//    QModelIndexList list = selectedItems->selectedRows();
//    QList<int> indexes;
//    foreach(QModelIndex index,list){
//        if(!indexes.contains(index.row())){
//            indexes.append( index.row() );
//        }
//    }

    parameters_->removeSelectedParameter( ui->parametersTableView->currentIndex() );
}

void LibraryWindow::clickedSaveButton() {
    if(!storage_->saveLibrary( library_ )){
        qDebug() << "Can not save library";
    }
}

}
