#include "measureitemview.h"
#include "../dbstorage/measurestorage.h"
#include "../models/measuremodel.h"
namespace tsunami {
namespace gui{

MeasureItemView::MeasureItemView(db::MeasureModel* model, QObject *parent) :
    QAbstractItemModel(parent),rows_(0),columns_(0) {

    measure_ = model;

}

QModelIndex MeasureItemView::index(int row, int column, const QModelIndex &parent) const {
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex MeasureItemView::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int MeasureItemView::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return measure_->dataRows();
//    return measure_.dataRows();
}

int MeasureItemView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return measure_->dataColumns();
//    return measure_.dataColumns();
}

QVariant MeasureItemView::data(const QModelIndex &index, int role) const  {
    if(role != Qt::EditRole && role != Qt::DisplayRole){
        return QVariant();
    }

    return QVariant( measure_->at( index.row(), index.column() ) );

//    Q_ASSERT(false);
//    return QVariant(measure_.item( index.row(), index.column() ));

}

Qt::ItemFlags MeasureItemView::flags(const QModelIndex &index) const {
    Q_UNUSED(index)
    return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

bool MeasureItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool ok;
    if(role == Qt::EditRole){
        measure_->at(index.row(),index.column())  = value.toDouble();
        return true;
    }

    return false;
}

QVariant MeasureItemView::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            return measure_->column(section);
        }
    }


    return QVariant();
}




}
}
