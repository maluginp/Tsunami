#include "prepareextractordialog.h"
#include "ui_prepareextractordialog.h"
#include "dbstorage/parameterstorage.h"
#include "views/measureenableview.h"
#include "delegates/delegatecheckbox.h"

namespace tsunami{
PrepareExtractorDialog::PrepareExtractorDialog(int deviceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareExtractorDialog),deviceId_(deviceId) {
    ui->setupUi(this);



    measures_ = new gui::MeasureEnableView(deviceId_);
    ui->measuresTableView->setModel( measures_ );
//    int width = ui->measuresTableView->width();
    ui->measuresTableView->setColumnWidth(0,30);
    ui->measuresTableView->setColumnWidth(1,400);
    ui->measuresTableView->setColumnWidth(2,100);



    ui->measuresTableView->setItemDelegateForColumn(0,new DelegateCheckBox(ui->measuresTableView));

    loadListLibraries();
    loadListExtractionMethods();

    connect(ui->methodExtractionComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedMethodExtractionComboBox(int)));
    connect(ui->libraryComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changedLibraryComboBox(int)));

    connect(ui->runButton,SIGNAL(clicked()),this,SLOT(clickedRunButton()));
}

PrepareExtractorDialog::~PrepareExtractorDialog() {
    delete ui;
}

QList<int> PrepareExtractorDialog::measures() {
    return measures_->listEnabledMeasures();
}

void PrepareExtractorDialog::loadListLibraries() {
    db::ParameterStorage* storage =  db::ParameterStorage::instance();
    QMap<int,QString> libraries = storage->listLibraries(deviceId_);

    ui->libraryComboBox->clear();
    foreach(QString name,libraries.values()){
        ui->libraryComboBox->addItem( name, libraries.key(name) );
    }

//    ui->libraryComboBox->setCurrentIndex(0);
    libraryId_ = ui->libraryComboBox->itemData(0).toInt();
}

void PrepareExtractorDialog::loadListExtractionMethods() {
    ui->methodExtractionComboBox->clear();
    ui->methodExtractionComboBox->addItem("Hooke-Jeevees", QVariant("hookejeeves") );
    methodExtraction_  = ui->methodExtractionComboBox->itemData(0,Qt::UserRole).toString();
}

void PrepareExtractorDialog::clickedRunButton()
{
    accept();
}

void PrepareExtractorDialog::changedMethodExtractionComboBox(int index){
    methodExtraction_ = ui->methodExtractionComboBox->itemData(index).toString();
}

void PrepareExtractorDialog::changedLibraryComboBox(int index){
    libraryId_ = ui->libraryComboBox->itemData(index).toInt();
}
}
