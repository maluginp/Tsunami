#include "ExtractorWindow.h"
#include "ui_ExtractorWindow.h"
#include "extractor/Extractor.h"
#include "views/ParametersExtractionView.h"
#include "dbstorage/SettingStorage.h"
#include "dbstorage/ParameterStorage.h"
#include "delegates/DelegateReadOnly.h"
#include "PlotExtractionDialog.h"

namespace tsunami{
using namespace core;

ExtractorWindow::ExtractorWindow(DeviceType type, int libraryId, QList<int> measures, const QString& optimize,
                                 QWidget *parent) :
    QMainWindow(parent),
    running_(false),
    ui(new Ui::ExtractorWindow)
{
    ui->setupUi(this);

    type_ = type;
    optimize_ = optimize;
    libraryId_ = libraryId;
    measures_ = measures;


    library_ = db::ParameterStorage::instance()->openLibrary( libraryId_ );
    parameters_ = new gui::ParametersExtractionView(library_);
    ui->parametersTableView->setModel( parameters_ );
    ui->parametersTableView->setUpdatesEnabled( true );

    int nParameters = library_->countParameters();
    for(int i=0; i < nParameters; ++i){
        if(!library_->at(i).enable()){
            ui->parametersTableView->hideRow(i);
        }
    }
//    ui->parametersTableView->setItemDelegateForColumn(0, new gui::DelegateReadOnly());

    settings_ = db::SettingStorage::instance();

    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    connect(ui->saveParametersButton, SIGNAL(clicked()),
                                this, SLOT(clickedSave()));
    connect(ui->runButton,SIGNAL(clicked()),this,SLOT(runExtraction()));
    connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(clickedStop()));
    connect(ui->showPlotButton,SIGNAL(clicked()),this,SLOT(clickedShowPlotButton()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(clickedCloseButton()));
}


ExtractorWindow::~ExtractorWindow() {
    delete ui;
}

void ExtractorWindow::finishExtraction() {
    running_ = false;
    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->saveParametersButton->setEnabled(true);


}

void ExtractorWindow::clickedSave() {
    db::ParameterStorage *parameterStorage = db::ParameterStorage::instance();

    bool saved = parameterStorage->saveLibrary( library_ );

    if(saved){
        MESSAGE_INFO(tr("Library is saved"));
        return;
    }
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

    Extractor* extractor = Extractor::createExtractor(optimize_,type_,library_,measures_);
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

    running_ = true;
    thread->start();


    return;

}

void ExtractorWindow::log(const QString &log) {
//    log::logDebug() << log;
    ui->extractionLogText->append( log );
}

void ExtractorWindow::clickedCancel() {

    emit stop();
}

void ExtractorWindow::clickedShowPlotButton(){
    PlotExtractionDialog dialog( library_->deviceId(), library_, measures_ );
    dialog.exec();

}
// FIXME: undefined behaviour
void ExtractorWindow::clickedCloseButton() {
    if(running_){
        emit stop();
    }
    close();
}

}
