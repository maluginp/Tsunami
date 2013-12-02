#include "librarywindow.h"
#include "ui_librarywindow.h"

#include "openlibrarydialog.h"
#include "dbstorage/parameterstorage.h"
#include "views/parameteritemview.h"
#include "delegates/delegatecheckbox.h"
#include "delegates/delegatedoubleitem.h"
#include "ShowTextDialog.h"
#include "dbstorage/devicestorage.h"
#include "models/devicemodel.h"

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
    }
}

}
