#include "PrepareExtractorDialog.h"
#include "ui_PrepareExtractorDialog.h"
#include "views/MeasureEnableView.h"
#include "delegates/DelegateCheckBox.h"
#include "Log.h"

namespace tsunami{
PrepareExtractorDialog::PrepareExtractorDialog(int deviceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareExtractorDialog),measures_(0),deviceId_(deviceId) {
    ui->setupUi(this);

    ui->measuresTableView->setItemDelegateForColumn(0,new DelegateCheckBox(ui->measuresTableView));

    db::MeasureStorage* storage =  db::MeasureStorage::instance();

    analysisTypeView_ = new gui::ListItemView("Analysis");
    if(storage->numberMeasuresByAnalysis(deviceId_,"ac") > 0){
        analysisTypeView_->addItem( "AC","ac");
    }
    if(storage->numberMeasuresByAnalysis(deviceId_,"dc") > 0){
        analysisTypeView_->addItem( "DC", "dc" );
    }
    if(storage->numberMeasuresByAnalysis(deviceId_,"tran") > 0){
        analysisTypeView_->addItem( "TRAN", "tran");
    }

     ui->analysisTreeView->setModel( analysisTypeView_ );

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


    if(analysisTypeView_->rowCount() == 0){
        reject();
    }else{
        clickedAnalysisType( analysisTypeView_->first() );
    }

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
    currentAnalysis_ = analysis;
    delete measures_;

    measures_ = new gui::MeasureEnableView(deviceId_,analysis);
    ui->measuresTableView->setModel( measures_ );
    ui->measuresTableView->setColumnWidth(0,30);
    ui->measuresTableView->setColumnWidth(1,400);
    ui->measuresTableView->setColumnWidth(2,100);

}

void PrepareExtractorDialog::clickedRunButton() {
    log::logTrace() << "Clicking";
    QList<int> numberMeasures = measures();
    if(numberMeasures.count() > 0 && libraryId_ != -1) {
        log::logTrace() << "Number measures " << numberMeasures;
        accept();
    }else{
        QMessageBox::warning(this,windowTitle(),tr("Measures are not choice"));
    }
}

void PrepareExtractorDialog::changedLibraryComboBox(int index){
    libraryId_ = ui->libraryComboBox->itemData(index).toInt();
}

void PrepareExtractorDialog::clickedAnalysisType(const QModelIndex &index) {
    QString type = index.data( Qt::UserRole ).toString();
    if(currentAnalysis_ != type ){
        loadMeasures(type);
    }
}
}
