#include "parametersextractionview.h"
#include "models/librarymodel.h"
#include "models/parametermodel.h"
#include <QColor>
#include <math.h>

namespace tsunami{
namespace gui{
ParametersExtractionView::ParametersExtractionView(db::LibraryModel *library,
                                                   QObject *parent)
    : library_(library),QAbstractItemModel(parent){

}

QModelIndex ParametersExtractionView::index(int row, int column,
                                            const QModelIndex &parent) const {
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex ParametersExtractionView::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int ParametersExtractionView::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return library_->countParameters();
}

int ParametersExtractionView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

QVariant ParametersExtractionView::data(const QModelIndex &index, int role) const {
//    if( role != Qt::DisplayRole ){
//        return QVariant();
//    }

    int row = index.row();
    int column = index.column();

    if( role == Qt::BackgroundColorRole){
        if( library_->at(row).fixed() ){
            return QVariant(QColor(0xDA,0xDA,0xDA));
        }else{
            if(  fabs(library_->at(row).initial() - library_->at(row).fitted()) > 1e-16  ){
                return QVariant(QColor("pink"));
            }
        }
        return QVariant(QColor(0xFF,0xFF,0xFF));
    }

    if( role == Qt::DisplayRole || role == Qt::EditRole ){
        switch(column){
        case 0: return library_->at(row).initial();
        case 1: return library_->at(row).fitted();
        }
    }
    return QVariant();
}

Qt::ItemFlags ParametersExtractionView::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

bool ParametersExtractionView::setData(const QModelIndex &index, const QVariant &value, int role){
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

QVariant ParametersExtractionView::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole ){
        return QVariant();
    }

    if(orientation == Qt::Vertical){
        return library_->at(section).name();
    }else if(orientation == Qt::Horizontal){
        if(section == 0){
            return tr("Initial");
        }else if(section == 1){
            return tr("Fitted");
        }
    }

    return QVariant();
}

}
}
