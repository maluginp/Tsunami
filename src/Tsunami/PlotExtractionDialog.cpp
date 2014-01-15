#include "PlotExtractionDialog.h"
#include "ui_PlotExtractionDialog.h"
#include <QComboBox>
#include "spice/Circuit.h"
#include "spice/NgSpiceSimulator.h"
#include "spice/SpiceModel.h"
#include "Log.h"

namespace tsunami{

PlotExtractionDialog::PlotExtractionDialog(int deviceId,
                                           db::LibraryModel* library,
                                           const QList<int>& measures,
                                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotExtractionDialog),
    deviceId_(deviceId),
    library_(library),
    simulator_(NULL)
{
    ui->setupUi(this);

    ui->typeParameterComboBox->addItem( tr("Initial"), "initial" );
    ui->typeParameterComboBox->addItem( tr("Fitted"),  "fitted" );
    db::SettingStorage* settings = db::SettingStorage::instance();
    db::MeasureStorage* storage =  db::MeasureStorage::instance();
    measures_ = storage->getMeasures( measures );

    foreach(db::MeasureModel* measure, measures_){
        ui->measureComboBox->addItem( measure->name(), measure->id() );
    }
    changedMeasure(0);
    QString simulator = settings->value("spice/simulator").toString();
    QString pathSimulator = settings->value(QString("spice/%1/path").
                                             arg(simulator)).toString();
//    if(simulator.compare("ngspice") == 0){
    simulator_ = new spice::NgSpiceSimulator( pathSimulator );
//    }


    connect(ui->buildButton,SIGNAL(clicked()),this,SLOT(clickedBuildButton()));
}

PlotExtractionDialog::~PlotExtractionDialog()
{
    delete ui;
}

db::MeasureModel* PlotExtractionDialog::simulate(db::MeasureModel* measure) {
    log::logTrace() << "Simulating";
    db::DeviceStorage* storage = db::DeviceStorage::instance();
    db::DeviceModel* device = storage->openDevice( deviceId_ );

    spice::Circuit *circuit = spice::Circuit::createCircuitDevice( device->type(),
                                                                   measure->sources() );

    spice::SpiceModel* model = new spice::SpiceModel( "simulate" ,device->type());
    model->setLibrary( library_ );


    circuit->setSpiceModel( device->type(), model );
    simulator_->setCircuit( circuit );

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

void PlotExtractionDialog::clickedBuildButton() {
    log::logTrace() << "Building plot";

    if(!checkInputValues()){
        QMessageBox::warning(this,windowTitle(),tr("Incorrect input values"));
        return;
    }

    int index = ui->measureComboBox->currentIndex();
    db::MeasureModel* measureModel = measures_[index];
    db::MeasureModel* simulateModel = simulate(measureModel);
    if(!simulateModel){
        log::logError() << "Can not simulate model";
        return;
    }
    int rows = measureModel->rows();

    QString keyName = ui->axisKeyComboBox->currentText();
    QString valueName = ui->axisValueComboxBox->currentText();
    QString constName = ui->constComboBox->currentText();

    QVector<double> constants;

    for(int i=0; i < rows; ++i){
        QMap<QString, double> measured  = measureModel->get(i);
        QMap<QString, double> simulated = simulateModel->find(measured);
        if(simulated.size() == 0){
            continue;
        }

        PlotItem* plot;
        if(constants.contains( measured[constName] )){
            plot = ui->plotter->plot( QString::number( measured[constName]) );
        }else{
            constants.append( measured[constName] );
            plot = ui->plotter->addPlot( QString::number( measured[constName]) );
        }

        plot->addRow( measured[keyName], simulated[valueName], measured[valueName] );

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

    QList<Source> sources = measure->sources();
    foreach(Source source,sources){
        if(source.direction() == SOURCE_DIRECTION_OUTPUT){
            ui->axisValueComboxBox->addItem( source.name() );
        }else if(source.mode() != SOURCE_MODE_GND) {
            ui->axisKeyComboBox->addItem( source.name() );
            ui->constComboBox->addItem(source.name());
        }
    }

}

}
