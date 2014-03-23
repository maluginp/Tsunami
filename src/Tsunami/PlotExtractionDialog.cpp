#include "PlotExtractionDialog.h"
#include "ui_PlotExtractionDialog.h"
#include <QComboBox>
#include "spice/Circuit.h"
#include "spice/NgSpiceSimulator.h"
#include "spice/SpiceModel.h"
#include "Log.h"

namespace tsunami{

using namespace spice;

PlotExtractionDialog::PlotExtractionDialog(int deviceId,
                                           db::LibraryModel* library,
                                           const QList<int>& measures,
                                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotExtractionDialog),
    deviceId_(deviceId),
    simulator_(NULL)
{
    ui->setupUi(this);

    ui->typeParameterComboBox->addItem( tr("Initial"), "initial" );
    ui->typeParameterComboBox->addItem( tr("Fitted"),  "fitted" );
    db::SettingStorage* settings = db::SettingStorage::instance();
    db::MeasureStorage* measureStorage =  db::MeasureStorage::instance();

    if(measures.count() > 0) {
        measures_ = measureStorage->getMeasures( measures );
    }else{
        measures_ = measureStorage->getMeasuresByDeviceId( deviceId_ );
    }


    foreach(db::MeasureModel* measure, measures_){
        ui->measureComboBox->addItem( measure->name(), measure->id() );
    }
    changedMeasure(0);
    QString simulator = settings->value("spice/simulator").toString();
    QString pathSimulator = settings->value(QString("spice/%1/path").
                                             arg(simulator)).toString();

    simulator_ = new spice::NgSpiceSimulator( pathSimulator );


    if(!library) { // нулевая библиотека

        db::ParameterStorage* parameterStorage = db::ParameterStorage::instance();
        libraries_ = parameterStorage->getLibrariesByDeviceId( deviceId_ );
        foreach(db::LibraryModel* lib,libraries_){
            ui->libraryComboBox->addItem( lib->name(), lib->id() );
        }
        ui->libraryComboBox->setEnabled(true);
    }else{
        ui->libraryComboBox->addItem( library->name(), library->id() );
        libraries_.append( library );
        ui->libraryComboBox->setEnabled(false);
    }

    checkedSimulateEnable(false);

    ui->labelConstant->setText("");

    connect(ui->plotter,SIGNAL(plottableDoubleClick(QCPAbstractPlottable*,QMouseEvent*)),
            this,SLOT(clickedPlot(QCPAbstractPlottable*,QMouseEvent*)));
    connect(ui->buildButton,SIGNAL(clicked()),this,SLOT(clickedBuildButton()));
    connect(ui->enableSimulateCheckBox,SIGNAL(toggled(bool)),this,SLOT(checkedSimulateEnable(bool)));
}

PlotExtractionDialog::~PlotExtractionDialog()
{
    delete ui;
}

db::MeasureModel* PlotExtractionDialog::simulate(db::MeasureModel* measure) {
    log::logTrace() << "Simulating";
    db::DeviceStorage* storage = db::DeviceStorage::instance();
    db::DeviceModel* device = storage->openDevice( deviceId_ );
    SpiceModel* model = new SpiceModel("SPICEMODEL",device->type());
    Circuit* circuit = new Circuit("Plot");
    circuit->create( device->type(), measure->sources(), model);

    model->setLibrary( getLibrary() );

    // Prepare simulator
    simulator_->setCircuit( circuit );
    simulator_->analyses( measure->analyses() );
    simulator_->typeAnalysis( measure->type() );
    simulator_->measure( measure );

    simulator_->simulate();
    db::MeasureModel* simulate = simulator_->simulatedData();
    delete device;

    return simulate;
}

bool PlotExtractionDialog::checkInputValues() {
    QString keyName = ui->axisKeyComboBox->currentText();
    QString valueName = ui->axisValueComboxBox->currentText();
    QString constName = ui->constComboBox->currentText();


    if(keyName == valueName || keyName == constName || valueName == constName){
        log::logError() << QString("Incorrect key=%1,value=%2,const=%3")
                           .arg(keyName).arg(valueName).arg(constName);
        return false;
    }

    return true;
}

bool PlotExtractionDialog::simulateEnabled() {
    return ui->enableSimulateCheckBox->isChecked();
}

db::LibraryModel *PlotExtractionDialog::getLibrary() {
    int index = ui->libraryComboBox->currentIndex();

    if(index >= libraries_.count()){
        return NULL;
    }

    return libraries_[index];

}

void PlotExtractionDialog::clickedBuildButton() {
    log::logTrace() << "Building plot";

//    ui->plotter->clearPlots();

    if(!checkInputValues()){
        MESSAGE_WARNING(tr("Incorrect input values"));
        return;
    }

    int index = ui->measureComboBox->currentIndex();
    db::MeasureModel* measureModel = measures_[index];
    db::MeasureModel* simulateModel = NULL;
    if(simulateEnabled()){
        simulateModel = simulate(measureModel);
        if(!simulateModel){
            log::logError() << "Can not simulate model";
            return;
        }
    }
    int rows = measureModel->rows();

    QString keyName = ui->axisKeyComboBox->currentText();
    QString valueName = ui->axisValueComboxBox->currentText();
    QString constName = ui->constComboBox->currentText();

    ui->plotter->clearPlots();

    ui->plotter->setLabelAxisX( keyName );
    ui->plotter->setLabelAxisY( valueName );

    for(int i=0; i < rows; ++i){
        QMap<QString, double> measured  = measureModel->get(i);

        PlotItem* plot;
        QMap<QString, double> simulated;
        if(simulateEnabled()){
            simulated = simulateModel->find(measured);
            if(simulated.size() == 0){
                continue;
            }
        }

        if(!simulateEnabled()){
            plot = ui->plotter->plot( QString::number( measured[constName]), PlotItem::PLOT_MEASURE );
            plot->addRow( measured[keyName], .0, measured[valueName] );
        }else{
            plot = ui->plotter->plot( QString::number( measured[constName]));
            plot->addRow( measured[keyName], simulated[valueName], measured[valueName] );
        }

        plot->setProperty("constant",constName);
        plot->setProperty("value",measured[constName]);


    }


    ui->plotter->build();
    return;
}

void PlotExtractionDialog::changedMeasure(int index) {

    if( index >= measures_.count() ){
        return;
    }

    db::MeasureModel* measure = measures_[index];

    ui->axisKeyComboBox->clear();
    ui->axisValueComboxBox->clear();;
    ui->constComboBox->clear();

    QList<Source*> sources = measure->sources();
    foreach(Source* source,sources){
        if(source->direction() == SOURCE_DIRECTION_OUTPUT){
            ui->axisValueComboxBox->addItem(source->name());
        }else if(source->mode() != SOURCE_MODE_GND) {
            ui->axisKeyComboBox->addItem(source->name());
            ui->constComboBox->addItem(source->name());
        }
    }

}

void PlotExtractionDialog::checkedSimulateEnable(bool enable) {
    ui->simulateGroup->setEnabled( enable );
}

void PlotExtractionDialog::clickedPlot(QCPAbstractPlottable *plot, QMouseEvent *mouseEvent) {
    log::logTrace() << "Clicked by plot " << plot->property("constant").toString()
                    << "=" << plot->property("value").toString();

    ui->labelConstant->setText( QString("%1=%2").arg(plot->property("constant").toString(),plot->property("value").toString()) );

}

}
