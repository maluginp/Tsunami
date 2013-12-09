#include "CreateLibraryDialog.h"
#include "ui_CreateLibraryDialog.h"
#include <QComboBox>
namespace tsunami{
CreateLibraryDialog::CreateLibraryDialog(int deviceId,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateLibraryDialog),
    deviceId_(deviceId)
{
    ui->setupUi(this);
    storage_ = db::ParameterStorage::instance();

    QMap<int,QString> libraries = storage_->listLibraries(deviceId_);
    ui->libraryCopiedNameComboBox->clear();
    foreach( QString name,libraries.values() ){
        ui->libraryCopiedNameComboBox->addItem( name, libraries.key(name) );
    }

    ui->libraryCopiedNameComboBox->setEnabled( false );

    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->createButton,SIGNAL(clicked()),this,SLOT(clickedCreateButton()));
    connect(ui->libraryCopiedCheckBox,SIGNAL(stateChanged(int)),
            this,SLOT(changedLibraryCopied(int)));
}

CreateLibraryDialog::~CreateLibraryDialog() {
    delete ui;
}

void CreateLibraryDialog::clickedCreateButton(){
    QString libraryName = ui->libraryNameLineEdit->text();

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
        bool ok;
        int libraryId = ui->libraryCopiedNameComboBox->itemData(
                    ui->libraryCopiedNameComboBox->currentIndex()).toInt(&ok);
        if(libraryId == -1 || !ok){
            return;
        }
        library_ = storage_->openLibrary( libraryId  );
        library_->id(-1);
    }else{
        library_ = new db::LibraryModel(libraryName,deviceId_);
    }

    accept();
}

void CreateLibraryDialog::changedLibraryCopied(int copied) {
    if(copied == Qt::Checked){
        ui->libraryCopiedNameComboBox->setEnabled(true);
    }else{
        ui->libraryCopiedNameComboBox->setEnabled(false);
    }
}

}
