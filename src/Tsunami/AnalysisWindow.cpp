#include "AnalysisWindow.h"
#include "ui_AnalysisWindow.h"
#include "webkit/WebKit.h"

AnalysisWindow::AnalysisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow)
{
    ui->setupUi(this);
//    ui->analysisWebWidget
//    ui->webView = new WebView();


    ui->webView->load( QUrl("html/index.html") );
    //    ui->analysisWebView->setUrl();
//    ui->analysisWebView->setPage(  );
}

AnalysisWindow::~AnalysisWindow()
{
    delete ui;
}
