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
    changeSigns_.clear();
//    log::logTrace() << "Simulating";
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

//    QByteArray netlist = netlist();

//    if(file.write( netlist ) == -1){
//        file.close();
////        file.remove();
//        return false;
//    }

//    file.close();

    // Execute
    QStringList arguments;
    arguments << "-b"  //<< QString("-o%1.out").arg(fileName)
                 << QString("%1.net").arg(fileName);

    QByteArray output;
    if(!exec( output, arguments )){
        return false;
    }

    // No. of Data Rows :

//    log::logTrace() << QString("Result simulated\n%1").arg(QString(output));


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

//    QByteArray output = outputData;
//    QList<QByteArray> list =  output.split( '\n' );
//    list.removeAt(0);
//    list.removeAt(0);
//    // Remove null items
//    QList<QByteArray>::iterator it = list.begin();
//    while(it!=list.end()){
//        if((*it).isEmpty()){
//            it = list.erase(it);
//        }else{
//            it++;
//        }
//    }

//    delete simulated_;
//    simulated_ = new db::MeasureModel();
//    simulated_->sources( circuit()->sources() );
//    simulated_->type( circuit()->typeAnalysis() );
//    simulated_->attrs(QVariantMap());
//    simulated_->columns(columns_);

//    QVector< QVector<double> > data;

//    foreach(QByteArray str,list){

//        QList<QByteArray> vals = str.split('\t');
//        if(vals.last().isEmpty()){
//            vals.removeLast();
//        }

//        if( columns_.size() != vals.size() ){
////            log::logDebug() << "Can not parsing "
////                            << str;
//            continue;
//        }

//        QVector<double> row;
//        int nColumn = 0;
//        foreach(QByteArray val, vals){
//            double value = val.toDouble();
//            if(isChangeSign(nColumn)){
//                value *= -1.0;
//            }
//            row.append(  value );
//            nColumn++;
//        }
//        data.append(row);
//    }
////    log::logDebug() << "Number of parsed row:"<<data.size();

//    simulated_->data(data);

}

bool NgSpiceSimulator::isChangeSign(int numberColumn) {

    QString column = columns_[numberColumn];

    return changeSigns_.contains(column);

}



}
}
