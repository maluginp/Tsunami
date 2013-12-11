#include "extractorwindow.h"
#include "ui_extractorwindow.h"
#include "extractor/extractor.h"
#include "views/parametersextractionview.h"
#include "dbstorage/settingstorage.h"
#include "dbstorage/parameterstorage.h"
#include <QWidget>
#include "delegates/delegatereadonly.h"
#include "PlotExtractionDialog.h"
namespace tsunami{
ExtractorWindow::ExtractorWindow(DeviceType type, int libraryId, QList<int> measures, const QString& optimize,
                                 QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExtractorWindow) {
    ui->setupUi(this);

    type_ = type;
    optimize_ = optimize;
    libraryId_ = libraryId;
    measures_ = measures;


    library_ = db::ParameterStorage::instance()->openLibrary( libraryId_ );
    parameters_ = new gui::ParametersExtractionView(library_);
    ui->parametersTableView->setModel( parameters_ );
    ui->parametersTableView->setUpdatesEnabled( true );
//    ui->parametersTableView->setItemDelegateForColumn(0, new gui::DelegateReadOnly());

    settings_ = db::SettingStorage::instance();

    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    connect(ui->runButton,SIGNAL(clicked()),this,SLOT(runExtraction()));
    connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(clickedStop()));
    connect(ui->showPlotButton,SIGNAL(clicked()),this,SLOT(clickedShowPlotButton()));
}


ExtractorWindow::~ExtractorWindow() {
    delete ui;
}

void ExtractorWindow::finishExtraction() {
    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->saveParametersButton->setEnabled(true);


}

void ExtractorWindow::clickedStop() {
    emit stop();
}


void ExtractorWindow::runExtraction() {

    ui->runButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->saveParametersButton->setEnabled(false);
    QString simulator = settings_->value("spice/simulator").toString();
    QString pathSimulator = settings_->value(QString("spice/%1/path").
                                             arg(simulator)).toString();

    core::Extractor* extractor = core::Extractor::createExtractor(optimize_,type_,library_,measures_);
    extractor->simulator(simulator,pathSimulator);
    QThread* thread = new QThread;


    extractor->moveToThread(thread);
    connect(thread,SIGNAL(started()),extractor,SLOT(process()));
    connect(extractor,SIGNAL(finished()),thread,SLOT(quit()));
    connect(this,SIGNAL(stop()),extractor,SLOT(stop()),Qt::DirectConnection);
    connect(extractor,SIGNAL(finished()),extractor,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
//    connect(extractor,SIGNAL(updateFunctionError()),this,SLOT(updateParametersView()),Qt::DirectConnection);
    connect(extractor,SIGNAL(finished()),this,SLOT(finishExtraction()));
    connect(extractor,SIGNAL(log(QString)),this,SLOT(log(QString)),Qt::QueuedConnection);

    thread->start();


    return;

}

void ExtractorWindow::log(const QString &log) {
    ui->extractionLogText->append( QString("%1").arg(log) );
}

void ExtractorWindow::clickedCancel() {

    emit stop();
}

void ExtractorWindow::clickedShowPlotButton(){
    PlotExtractionDialog dialog( library_->deviceId(), library_, measures_ );
    dialog.exec();

}

}
