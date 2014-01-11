#include "LibraryWindow.h"
#include "ui_LibraryWindow.h"

#include "OpenLibraryDialog.h"
#include "dbstorage/ParameterStorage.h"
#include "views/ParameterItemView.h"
#include "delegates/DelegateCheckBox.h"
#include "delegates/DelegateDoubleItem.h"
#include "ShowTextDialog.h"
#include "dbstorage/DeviceStorage.h"
#include "models/DeviceModel.h"
#include "CreateLibraryDialog.h"
namespace tsunami{

LibraryWindow::LibraryWindow(int deviceId,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LibraryWindow), parameters_(NULL) {
    ui->setupUi(this);
    storage_ = db::ParameterStorage::instance();

    deviceId_ = deviceId;



    connect(ui->actionExportNetList,SIGNAL(triggered()),this,SLOT(clickedExportNetList()));
    connect(ui->actionOpenLibrary,SIGNAL(triggered()),this,SLOT(clickedOpenLibraryAction()));
    connect(ui->actionNewLibrary,SIGNAL(triggered()),this, SLOT(clickedNewLibraryAction()));
    connect(ui->actionAddParameter,SIGNAL(triggered()),this,SLOT(clickedAddParameterAction()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(clickedAddParameterAction()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(clickedRemoveParameterAction()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(clickedSaveButton()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(clickedSearchButton()));
    connect(ui->actionRemoveLibrary,SIGNAL(triggered()),this,SLOT(clickedRemoveLibrary()));
    connect(ui->actionCloseLibrary,SIGNAL(triggered()),this,SLOT(clickedCloseLibrary()));

    clickedCloseLibrary();
}

LibraryWindow::~LibraryWindow() {
    delete ui;
}

void LibraryWindow::openLibrary(int libraryId) {
    if(libraryId == -1){
        return;
    }

    library_ = storage_->openLibrary(libraryId);

    showParameters( library_ );
    enableButtons(true);

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

void LibraryWindow::enableButtons(bool enable) {
    ui->libraryNameLineEdit->setEnabled( enable );
    ui->searchButton->setEnabled(enable);
    ui->addButton->setEnabled(enable);
    ui->removeButton->setEnabled(enable);
    ui->saveButton->setEnabled(enable);

}


void LibraryWindow::clickedOpenLibraryAction() {

    int libraryId =  OpenLibraryDialog::getLibraryId(deviceId_);

    openLibrary( libraryId );

}

void LibraryWindow::clickedNewLibraryAction() {

    CreateLibraryDialog dialog(deviceId_);
    if(dialog.exec() == QDialog::Accepted){
        library_ = dialog.library();
        showParameters(library_);

        enableButtons(true);

        emit updatedDataBase();

    }
    return;
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

void LibraryWindow::clickedExportNetList() {
    QString libraryName;
    QString netlist;

    db::DeviceModel* device = db::DeviceStorage::instance()->openDevice(deviceId_);

    libraryName = library_->name();
    libraryName.replace(" ","_");

     netlist.append( QString(".model %1").arg(libraryName));

    DeviceType type = device->type();
    switch(type){
    case DEVICE_CAPACITOR:
    case DEVICE_RESISTOR:
    case DEVICE_DIODE:
        netlist.append( "" );
        break;
    case DEVICE_NBJT:
        netlist.append(" NPN");
        break;
    case DEVICE_PBJT:
        netlist.append(" PNP");
        break;
    case DEVICE_NMOS:
        netlist.append(" NMOS");
        break;
    case DEVICE_PMOS:
        netlist.append(" PMOS");
        break;
    case DEVICE_UNKNOWN:
    default:
        Q_ASSERT(false);
//            break;
    }

    netlist.append("(");

    QStringList parameters;
    foreach(db::ParameterModel parameter, library_->parameters()){
        if(parameter.enable()){
            parameters << QString("%1=%2").arg(parameter.name()).arg(parameter.fitted());
        }
    }

    netlist.append( parameters.join(", ") );
//    netlist.chop(1);
    netlist.append(")");

    ShowTextDialog dialog( tr("Export NetList"), netlist );
    dialog.exec();

    dialog.close();
}

void LibraryWindow::clickedSaveButton() {
    if(!storage_->saveLibrary( library_ )){
        qDebug() << "Can not save library";
    }else{
        emit updatedDataBase();
    }
}

void LibraryWindow::clickedSearchButton() {
    QString name = ui->libraryNameLineEdit->text();
    if(parameters_ == 0 || name.isEmpty()){
        return;
    }
    ui->parametersTableView->setCurrentIndex( parameters_->findByParameterName(name) );
}

void LibraryWindow::clickedRemoveLibrary() {
    if(library_ != 0){
        if(library_->id() == -1){
            return;
        }

        int button =  QMessageBox::question(this,
                                            tr("Remove library"),
                                            tr("Do you want remove %1").arg(library_->name()),
                                            QMessageBox::No | QMessageBox::Yes,
                                            QMessageBox::No);
        if(button == QMessageBox::Yes){
            if(storage_->removeLibrary( library_->id() )){
                clickedCloseLibrary();
                delete library_;
                library_ = 0;
            }
    }
}




}

void LibraryWindow::clickedCloseLibrary() {
    ui->libraryNameText->setText(tr("Choice library"));
    delete parameters_;
    ui->parametersTableView->setModel(0);
    parameters_ = 0;
    library_ = 0;

    enableButtons(false);

}

}
