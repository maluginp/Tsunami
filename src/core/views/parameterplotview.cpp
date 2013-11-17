#include "parameterplotview.h"
#include <QPair>

namespace tsunami {
namespace gui{

ParameterPlotView::ParameterPlotView(QVariantMap parameters, QObject *parent) :
    QAbstractItemModel(parent) {
    items_ = parameters;
    changes_ = parameters;
}

QModelIndex ParameterPlotView::index(int row, int column, const QModelIndex &parent) const {
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }
    if(!parent.isValid()){
        return createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex ParameterPlotView::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

int ParameterPlotView::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return items_.count();
}

int ParameterPlotView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant ParameterPlotView::data(const QModelIndex &index, int role) const {
    if(role != Qt::DisplayRole && role != Qt::EditRole){
        return QVariant();
    }
    if( index.column() == 0){
        return itemAt(index.row()).second;
    }else if(index.column() == 1){
        return changeAt(index.row()).second.toString();
    }
    return QVariant();
}

Qt::ItemFlags ParameterPlotView::flags(const QModelIndex &index) const {
    if(index.column()==0){
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool ParameterPlotView::setData(const QModelIndex &index, const QVariant &value, int role) {
    Q_UNUSED(role);
    QString key;
    if( index.column() == 1 ){
        key = changeAt(index.row()).first;
        if(key.isEmpty()){ return false; }
        changes_[key] = value;
        return true;
    }

    return false;
}

QVariant ParameterPlotView::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    QString key;
    if(orientation == Qt::Vertical){
        key = itemAt(section).first;
        if(!key.isEmpty()){
            return QVariant(key);
        }
    }else if(orientation == Qt::Horizontal){
        if(section == 0){
            return QVariant("Initial");
        }else if(section == 1){
            return QVariant("Changed");
        }
    }

    return QVariant();
}

QPair<QString, QVariant> ParameterPlotView::itemAt(int index) const {
    int row = 0;
    foreach( QString key, items_.keys() ){
        if(row == index){
            return QPair<QString,QVariant>(key,items_.value(key));
        }
        ++row;
    }
    return QPair<QString,QVariant>();
}

QPair<QString, QVariant> ParameterPlotView::changeAt(int index) const {
    int row = 0;
    foreach( QString key, changes_.keys() ){
        if(row == index){
            return QPair<QString,QVariant>(key,changes_.value(key));
        }
        ++row;
    }
    return QPair<QString,QVariant>();
}

}
}
