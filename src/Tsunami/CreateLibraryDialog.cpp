#include "CreateLibraryDialog.h"
#include "ui_CreateLibraryDialog.h"
#include <QComboBox>
namespace tsunami{
CreateLibraryDialog::CreateLibraryDialog(int deviceId,QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CreateLibraryDialog),
        deviceId_(deviceId) {

    ui->setupUi(this);

    ui->errorLibraryNameText->setText("");
    ui->errorLibraryText->setText("");

    // Get Device Type from database
    db::DeviceStorage* deviceStorage = db::DeviceStorage::instance();
    db::DeviceModel* device = deviceStorage->openDevice(deviceId_);

    if(device){
        deviceType_ = device->type();
        delete device;
    }

    QVariantMap libraryType;
    libraryType.insert(tr("Device libraries"),"library");
    libraryType.insert(tr("Templates"),"template");
    libraryTree_ = new gui::ListTreeView(tr("Libraries"),libraryType);

    ui->libraryTreeView->setModel( libraryTree_ );

    storage_ = db::ParameterStorage::instance();   

    loadLibrariesTreeView();
    changedLibraryCopied(Qt::Unchecked);

    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(clickedCreateButton()));
    connect(ui->libraryCopiedCheckBox,SIGNAL(stateChanged(int)),
            this,SLOT(changedLibraryCopied(int)));
}

CreateLibraryDialog::~CreateLibraryDialog() {
    delete ui;
}

void CreateLibraryDialog::loadLibrariesTreeView() {
    libraryTree_->clear();
    QList<db::LibraryModel*> libraries;
    // Device libraries
    libraries = storage_->getLibrariesByDeviceId( deviceId_ );
    foreach(db::LibraryModel* library,libraries){
        libraryTree_->addChildByValue("library", library->name(), library->id() );
    }
    qDeleteAll(libraries);

    libraries = storage_->getTemplateLibrariesByDeviceType( deviceType_ );
    foreach(db::LibraryModel* library,libraries ){
        libraryTree_->addChildByValue( "template", library->name(), -1 );
    }
    qDeleteAll(libraries);


    ui->libraryTreeView->expandAll();

}

void CreateLibraryDialog::clickedCreateButton(){
    QString libraryName = ui->libraryNameLineEdit->text();

    ui->errorLibraryText->setText("");
    ui->errorLibraryNameText->setText("");

    if(libraryName.isEmpty()){
        ui->errorLibraryNameText->setText( tr("Library name is empty." ) );
        return;
    }

    if(storage_->exists(libraryName)){
        ui->errorLibraryNameText->setText( tr("Library is existed.") );
        return;
    }

    library_ = 0;
    if( ui->libraryCopiedCheckBox->checkState() == Qt::Checked ){
        // Check root item
        bool ok;
        int libraryId = ui->libraryTreeView->currentIndex().data(Qt::UserRole).toInt(&ok);

        if (ok) {
            if (libraryId == -1) {
                QString name = ui->libraryTreeView->currentIndex().data(Qt::DisplayRole).toString();
                library_ = storage_->openTemplateLibrary(name);
                library_->deviceId( deviceId_ );
            } else {
                library_ = storage_->openLibrary( libraryId  );
                library_->id(-1);
            }

            if(library_){
                library_->name( libraryName );
            }
        }else{
            ui->errorLibraryText->setText( tr("Not choiced library") );
            return;
        }
    }else{
        library_ = new db::LibraryModel(libraryName,deviceId_);
    }

    accept();
}

void CreateLibraryDialog::changedLibraryCopied(int copied) {
    if(copied == Qt::Checked){
        ui->libraryTreeView->setEnabled(true);
     }else{
        ui->libraryTreeView->setEnabled(false);
    }
}

}
