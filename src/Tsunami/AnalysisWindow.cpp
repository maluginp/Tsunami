#include "AnalysisWindow.h"
#include "ui_AnalysisWindow.h"
#include "webkit/WebKit.h"
#include <QComboBox>
#include <QLineEdit>
#include "dbstorage/analysisstorage.h"
#include "models/analysismodel.h"
#include "webkit/APIObject.h"
namespace tsunami{
AnalysisWindow::AnalysisWindow(int deviceId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow),
    listAnalysis_(0),api_(0),
    deviceId_(deviceId)
{
    ui->setupUi(this);


    api_ = new APIObject();
    storage_ = db::AnalysisStorage::instance();
    listAnalysis_ = new gui::ListItemView();
    ui->analysisItemsListView->setModel( listAnalysis_ );

    ui->analysisTypeComboBox->addItem( "AC", "dc" );
    ui->analysisTypeComboBox->addItem( "DC", "dc" );
    ui->analysisTypeComboBox->addItem( "TRAN", "tran" );
//    connect(ui->webView,SIGNAL(loadStarted()),this,SLOT(loadStarted()));
//    loadStarted();
    connect(ui->webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(loadStarted()) );
    ui->webView->load( QUrl("html/test.html") );
    ui->webView->setFocus();

    updateAnalysisList();

    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openAnalysis()));
    //    ui->analysisWebView->setUrl();
//    ui->analysisWebView->setPage(  );
}

AnalysisWindow::~AnalysisWindow() {
    delete ui;
}

void AnalysisWindow::updateAnalysisList() {
    QMap<int,QString> list = storage_->listAnalysis(deviceId_);

    listAnalysis_->clear();

    foreach(int analysisId,list.keys()){
        listAnalysis_->addItem( list.value(analysisId), QVariant(analysisId) );
    }
}

void AnalysisWindow::openAnalysis() {
    analysisId_ = 1;

    db::AnalysisModel* analysis = storage_->openAnalysis( analysisId_ );

    int typeIndex = ui->analysisTypeComboBox->findData( analysis->typeJson() );
    ui->analysisTypeComboBox->setCurrentIndex(typeIndex);

    ui->analysisNameLineEdit->setText( analysis->name() );

    if(analysis->enable()){
        ui->analysisEnableCheckBox->setCheckState(Qt::Checked);
    }else{
        ui->analysisEnableCheckBox->setCheckState(Qt::Unchecked);
    }

    api_->openAnalysis( analysis );
}

void AnalysisWindow::loadStarted() {
//    delete api_;
    api_->disconnect( );
//    disconnect( api_, SIGNAL(analysisOpened(QVariantList)) );
     ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("Api",api_);
}

}
