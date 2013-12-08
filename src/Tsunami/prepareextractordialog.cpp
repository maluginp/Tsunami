#include "prepareextractordialog.h"
#include "ui_prepareextractordialog.h"
#include "views/measureenableview.h"
#include "delegates/delegatecheckbox.h"

namespace tsunami{
PrepareExtractorDialog::PrepareExtractorDialog(int deviceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareExtractorDialog),deviceId_(deviceId),measures_(0) {
    ui->setupUi(this);

    ui->measuresTableView->setItemDelegateForColumn(0,new DelegateCheckBox(ui->measuresTableView));


    analysisTypeView_ = new gui::ListItemView("Analysis");
    analysisTypeView_->addItem( "AC","ac");
    analysisTypeView_->addItem( "DC", "dc" );
    analysisTypeView_->addItem( "TRAN", "tran");
    ui->analysisTreeView->setModel( analysisTypeView_ );
    loadMeasures( "ac" );

    loadListLibraries();


    db::SettingStorage* settings = db::SettingStorage::instance();
    ui->methodText->setText( settings->value("optimize/method").toString() );
//    optimize/method"

    connect(ui->analysisTreeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(clickedAnalysisType(QModelIndex)));
    connect(ui->libraryComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changedLibraryComboBox(int)));
    connect(ui->runButton,SIGNAL(clicked()),this,SLOT(clickedRunButton()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
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

void PrepareExtractorDialog::loadMeasures(const QString &analysis) {
    delete measures_;

    measures_ = new gui::MeasureEnableView(deviceId_,analysis);
    ui->measuresTableView->setModel( measures_ );
    ui->measuresTableView->setColumnWidth(0,30);
    ui->measuresTableView->setColumnWidth(1,400);
    ui->measuresTableView->setColumnWidth(2,100);

}

void PrepareExtractorDialog::clickedRunButton()
{
    accept();
}

void PrepareExtractorDialog::changedLibraryComboBox(int index){
    libraryId_ = ui->libraryComboBox->itemData(index).toInt();
}

void PrepareExtractorDialog::clickedAnalysisType(const QModelIndex &index) {
    QString type = index.data( Qt::UserRole ).toString();
    loadMeasures(type);
}
}
