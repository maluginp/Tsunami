#include "listitemview.h"

ListItemView::ListItemView(QObject *parent) :
QAbstractItemModel(parent) {

}

ListItemView::ListItemView(const QVariantMap &items, QObject *parent) :
    QAbstractItemModel(parent) {
    setItems( items );
}

QModelIndex ListItemView::index(int row, int column, const QModelIndex &parent) const {
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex ListItemView::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int ListItemView::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return items_.size();
}

int ListItemView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

QVariant ListItemView::data(const QModelIndex &index, int role) const {
    if(index.column() != 0){
        return QVariant();
    }

    if( editable_ && role == Qt::EditRole ){
        return itemAt( index.row() );
    }

    if( role == Qt::DisplayRole ){
        return keyAt( index.row() );
    }

    return QVariant();
}

Qt::ItemFlags ListItemView::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags;
    flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if( editable_ ){
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}


bool ListItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!editable_){
        return false;
    }

    return true;
}

QVariant ListItemView::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

void ListItemView::setItems(const QVariantMap &items) {
    items_ = items;
}

QVariant ListItemView::itemAt(const int &index) const {
    int row=0;
    foreach(QVariant item, items_.values()){
        if(row == index){
            return item;
        }
        ++row;
    }
    return QVariant();
}

QString ListItemView::keyAt(const int &index) const {
    int row=0;
    foreach(QString item, items_.keys()){
        if(row == index){
            return item;
        }
        ++row;
    }
    return QString();
}
