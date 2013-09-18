#include "measureitemview.h"
#include "../dbstorage/measurestorage.h"
#include "../models/measuremodel.h"

MeasureItemView::MeasureItemView(const int &measureId, QObject *parent) :
    QAbstractItemModel(parent),rows_(0),columns_(0) {

    storage_ = MeasureStorage::instance();

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
    return QModelIndex();
}

int MeasureItemView::rowCount(const QModelIndex &parent) const {
    return rows_;
}

int MeasureItemView::columnCount(const QModelIndex &parent) const {
    return columns_;
}

QVariant MeasureItemView::data(const QModelIndex &index, int role) const {
    if(role != Qt::EditRole && role != Qt::DisplayRole){
        return QVariant();
    }

    return QVariant(measure_.item( index.row(), index.column() ));

}

Qt::ItemFlags MeasureItemView::flags(const QModelIndex &index) const {
    return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

bool MeasureItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool ok;
    if(role == Qt::EditRole){
        measure_.item( index.row(), index.column() ) = value.toDouble(&ok);
    }
    return false;
}

QVariant MeasureItemView::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            return measure_.getColumnName( section );
        }
    }

    return QVariant();
}

bool MeasureItemView::saveMeasure() {
    return storage_->saveMeasure( measure_ );
}


void MeasureItemView::openMeasure(const int &measureId) {
    measure_ = storage_->openMeasure( measureId );
    storeMeasure_ = measure_;

    rows_ = measure_.dataRows();
    columns_ = measure_.dataColumns();
}

void MeasureItemView::restoreMeasure() {
    measure_ = storeMeasure_;
}
