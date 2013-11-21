#include "analysismodel.h"
#include "../components/json.h"
#include <QDebug>
//AnalysisModel::AnalysisModel() :
//    analysisId_(-1), deviceId_(-1),name_(QString()), type_(ANALYSIS_UNKNOWN),
//    createAt_(QDateTime::currentDateTime()), changeAt_(QDateTime::currentDateTime()),
//    enable_(false) {
//}

//void AnalysisModel::removeItemInput(const QString &nameItem) {
//    foreach(IAnalysisItem* item,inputs_){
//        if(item->name().compare(nameItem,Qt::CaseInsensitive) == 0){
//            inputs_.removeOne( item );
//        }
//    }
//}

//void AnalysisModel::removeItemOutput(const QString &nameItem) {
//    foreach(IAnalysisItem* item,outputs_){
//        if(item->name().compare(nameItem,Qt::CaseInsensitive) == 0){
//            outputs_.removeOne( item );
//        }
//    }
//}

//AnalysisModel &AnalysisModel::setId(const int &analysisId) {
//    analysisId_ = analysisId;
//    return *this;
//}

//AnalysisModel &AnalysisModel::setDeviceId(const int &deviceId) {
//    deviceId_ = deviceId;
//    return *this;
//}

//AnalysisModel &AnalysisModel::setName(const QString &name) {
//    name_ = name;
//    return *this;
//}

//AnalysisModel &AnalysisModel::setType(const AnalysisModel::AnalysisType &type) {
//    type_ = type;
//    return *this;
//}

//AnalysisModel &AnalysisModel::setType(const QString &type) {
//    if(type.compare("dc") == 0){
//        type_ = ANALYSIS_DC;
//    }else if(type.compare("ac") == 0){
//        type_ = ANALYSIS_AC;
//    }else if(type.compare("tran") == 0){
//        type_ = ANALYSIS_TRAN;
//    }else{
//        type_ = ANALYSIS_UNKNOWN;
//    }
//    return *this;
//}

//AnalysisModel &AnalysisModel::setInputs(const QList<IAnalysisItem *> &inputs) {
//    inputs_ = inputs;
//    return *this;
//}

//AnalysisModel &AnalysisModel::addInput(IAnalysisItem *input) {
//    if( !inputExists( input->name() ) ){
//        inputs_.append( input );
//    }else{
//        int nInputItems = inputs_.size();
//        for(int i=0; i < nInputItems; ++i){
//            if(inputs_[i]->name() == input->name()){
//                delete inputs_[i];
//                inputs_[i] = input;
//            }
//        }
//    }


//    return *this;
//}

//AnalysisModel &AnalysisModel::setOutputs(const QList<IAnalysisItem *> &outputs) {
//    outputs_ = outputs;
//    return *this;
//}

//AnalysisModel &AnalysisModel::addOutput(IAnalysisItem *output) {
//    outputs_.append( output );
//    return *this;
//}

//AnalysisModel &AnalysisModel::setCreateAt(const QDateTime &createAt) {
//    createAt_ = createAt;
//    return *this;
//}

//AnalysisModel &AnalysisModel::setChangeAt(const QDateTime &changeAt) {
//    changeAt_ = changeAt;
//    return *this;
//}

//AnalysisModel &AnalysisModel::setEnable(const bool &enable) {
//    enable_ = enable;
//    return *this;
//}

//void AnalysisModel::parseJsonInput(const QString &json) {
//    QVariantList inputItems = QtJson::parse(json).toList();
//    inputs_.clear();
//    foreach(QVariant inputItem, inputItems){
//        IAnalysisItem* item = NULL;
//        QVariantMap inputItemAttrs = inputItem.toMap();
//        QString type = inputItemAttrs.value( "type" ).toString();

//        if(type.compare("sweep") == 0){
//            item = new AnalysisItemSweep( inputItemAttrs.value( "node" ).toString(),
//                                          inputItemAttrs.value( "mode" ).toString(),
//                                          inputItemAttrs.value( "number" ).toInt(),
//                                          inputItemAttrs.value("method").toString(),
//                                          inputItemAttrs.value("start").toDouble(),
//                                          inputItemAttrs.value("stop").toDouble(),
//                                          inputItemAttrs.value("step").toDouble());


//        }else if(type.compare("const") == 0){
//            item = new AnalysisItemConst(inputItemAttrs.value( "node" ).toString(),
//                                         inputItemAttrs.value( "mode" ).toString(),
//                                         inputItemAttrs.value( "const" ).toDouble());
//        }

//        item->parseJson( inputItem.toString() );

//        if(item != NULL){
//            inputs_.append( item );
//        }

//    }


//}

//void AnalysisModel::parseJsonOutput(const QString &json) {
//    QVariantList outputItems = QtJson::parse(json).toList();
//    outputs_.clear();
//    foreach(QVariant outputItem, outputItems){
//        IAnalysisItem* item = NULL;
//        QVariantMap outputItemAttrs = outputItem.toMap();
//        QString type = outputItemAttrs.value( "type" ).toString();

//        if(type.compare("output") == 0){
//            item = new AnalysisItemOutput(outputItemAttrs.value( "node" ).toString(),
//                                          outputItemAttrs.value( "mode" ).toString());
//        }
//        //        else if(type.compare("func") == 0){
//        //            item = new AnalysisItem
//        //        }
//        item->parseJson( outputItem.toString() );
//        if(item != NULL){
//            outputs_.append( item );
//        }

//    }
//}

//QString AnalysisModel::jsonInput() {
//    QVariantList inputs;

//    foreach( IAnalysisItem* item, inputs_ ){
//        inputs.append( item->json() );
//    }
//    QString serialized = QtJson::serializeStr( inputs );
//    qDebug() << serialized;
//    return serialized;
//}

//QString AnalysisModel::jsonOutput() {
//    QVariantList outputs;

//    foreach( IAnalysisItem* item, outputs_ ){
//        outputs.append( item->json() );
//    }

//    return QtJson::serializeStr( outputs );

//}

//bool AnalysisModel::inputExists(const QString &name) {
//    foreach(IAnalysisItem* item,inputs_){
//        if(item->name() == name){
//            return true;
//        }
//    }
//    return false;
//}

//bool AnalysisModel::outputExists(const QString &name) {
//    foreach(IAnalysisItem* item,outputs_){
//        if(item->name() == name){
//            return true;
//        }
//    }
//    return false;
//}
