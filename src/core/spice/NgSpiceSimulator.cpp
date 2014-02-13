#include "NgSpiceSimulator.h"
#include <QFile>
#include "Circuit.h"
#include "SpiceModel.h"
#include "Device.h"
#include "Terminal.h"
#include "models/MeasureModel.h"

#include "Log.h"

namespace tsunami{
namespace spice{

NgSpiceSimulator::NgSpiceSimulator(const QString &path) :
    Simulator(path) {

}

bool NgSpiceSimulator::simulate() {
//    changeSigns_.clear();
    log::logTrace() << "Simulating";
    // Append models;
    QString fileName = randomName(8);

    QFile file( QString("%1.net").arg(fileName) );
    log::logTrace() << QString("Create netlist file %1")
                       .arg(file.fileName());

    if(!file.open(QIODevice::WriteOnly)){
        log::logError() << QString("Can not open file for write %1")
                           .arg(file.fileName());
        return false;
    }

    QByteArray generatedNetlist = netlist();

    if(file.write( generatedNetlist ) == -1){
//        file.remove();
        return false;
    }

    file.close();

    // Execute
    QStringList arguments;
    arguments << "-b"  //<< QString("-o%1.out").arg(fileName)
                 << QString("%1.net").arg(fileName);

    QByteArray output;
    if(!exec( output, arguments )){
        return false;
    }

    // No. of Data Rows :

    log::logTrace() << QString("Result simulated\n%1").arg(QString(output));


    int positionStartIndex = output.indexOf("Index");
    Q_ASSERT(positionStartIndex != -1);
    output = output.mid(positionStartIndex).replace("\r","");

    parseSimulatedData(output);

    return true;
}

bool NgSpiceSimulator::isCorrectForParse() {
    return true;
}

void NgSpiceSimulator::parseSimulatedData(const QByteArray &outputData) {
    QByteArray output = outputData;
    QList<QByteArray> list =  output.split( '\n' );
    list.removeAt(0);
    list.removeAt(0);
//    // Remove null items
    QList<QByteArray>::iterator it = list.begin();
    while(it!=list.end()){
        if((*it).isEmpty()){
            it = list.erase(it);
        }else{
            it++;
        }
    }

    delete simulated_;
    simulated_ = new db::MeasureModel();
    simulated_->sources( measure_->sources() );
    simulated_->type( measure_->type() );

//    int numberAnalyses = analyses_.count();


//    simulated_->attrs(QVariantMap());
//    simulated_->columns(columns_);

    QVector< QVector<double> > data;
//    int dc1=0, dc2=0;
    foreach(QByteArray str,list){

        QList<QByteArray> vals = str.split('\t');
        if(vals.last().isEmpty()){
            vals.removeLast();
        }

        if( columns_.size() != vals.size() ){
            log::logDebug() << "Can not parsing "
                            << str;
            continue;
        }else{

            if(measure_->type() == ANALYSIS_DC){

            }
        }

        QVector<double> row;
        int nColumn = 0;
        foreach(QByteArray val, vals){
            double value = val.toDouble();
//            if(isChangeSign(nColumn)){
//                value *= -1.0;
//            }
            row.append(  value );
            nColumn++;
        }
        data.append(row);
    }

    QVector< QVector<double> > tempData;
    if(measure_->type() == ANALYSIS_DC){
        int numberAnalyses = measure_->numberAnalyses();

        if(numberAnalyses == 2){
            QList<double> sourceFirstValues = measure_->analysisValues(0);
            QList<double> sourceSecondValues = measure_->analysisValues(1);
            int dataRows = sourceFirstValues.count()*sourceSecondValues.count();
            log::logTrace() << QString("Parse simulated data\n"
                                   "Count generated rows: %1\n"
                                  "Simulated rows: %2")
                               .arg(dataRows).arg(data.count());
            Q_ASSERT( dataRows == data.count());

            columns_.append( measure_->analysis(0).value("name").toString() );
            columns_.append( measure_->analysis(1).value("name").toString() );

            int index = 0;
            foreach(double dc2, sourceSecondValues){
                foreach(double dc1,sourceFirstValues){
                    QVector<double> row = data.at(index);
                    row.append(dc1);
                    row.append(dc2);

                    tempData.append(row);
                    index++;
                }
            }
        }else{
            QList<double> sourceFirstValues = measure_->analysisValues(0);
            Q_ASSERT(sourceFirstValues.count() == data.count());
            int index=0;
            columns_.append( measure_->analysis(0).value("name").toString() );
            foreach(double dc1,sourceFirstValues){
                QVector<double> row = data.at(index);
                row.append(dc1);
//                row.append(dc2);

                tempData.append(row);
                index++;
            }
        }
        data = tempData;
    }

    log::logTrace() << "Columns after parse: " << columns_;
    simulated_->columns(columns_);
    simulated_->data(data);

}




}
}
