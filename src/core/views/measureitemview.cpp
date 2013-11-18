#include "measureitemview.h"
#include "../dbstorage/measurestorage.h"
#include "../models/measuremodel.h"
namespace tsunami {
namespace gui{

MeasureItemView::MeasureItemView(const int &measureId, QObject *parent) :
    QAbstractItemModel(parent),rows_(0),columns_(0) {

    storage_ = db::MeasureStorage::instance();

    openMeasure(measureId);


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
    Q_ASSERT(false);
//    return measure_.dataRows();
}

int MeasureItemView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    Q_ASSERT(false);
//    return measure_.dataColumns();
}

QVariant MeasureItemView::data(const QModelIndex &index, int role) const  {
    if(role != Qt::EditRole && role != Qt::DisplayRole){
        return QVariant();
    }

    Q_ASSERT(false);
//    return QVariant(measure_.item( index.row(), index.column() ));

}

Qt::ItemFlags MeasureItemView::flags(const QModelIndex &index) const {
    Q_UNUSED(index)
    return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

bool MeasureItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool ok;
//    if(role == Qt::EditRole){
//        measure_.setItem( index.row(), index.column(), value.toDouble(&ok));
//    }

    Q_ASSERT(false);
    return false;
}

QVariant MeasureItemView::headerData(int section, Qt::Orientation orientation, int role) const {
//    if(role == Qt::DisplayRole){
//        if(orientation == Qt::Horizontal){
//            return measure_.getColumnName( section );
//        }
//    }
    Q_ASSERT(false);

    return QVariant();
}

bool MeasureItemView::saveMeasure() {
    return storage_->saveMeasure( measure_ );
}


void MeasureItemView::openMeasure(int measureId) {

    Q_ASSERT(false);
//    if(measureId == -1){
//        return;
//    }

//    measure_ = storage_->openMeasure( measureId );
//    storeMeasure_ = measure_;

//    rows_ = measure_.dataRows();
//    columns_ = measure_.dataColumns();
}

void MeasureItemView::restoreMeasure() {
    measure_ = storeMeasure_;
}

}
}
