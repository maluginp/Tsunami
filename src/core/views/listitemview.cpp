#include "listitemview.h"

namespace tsunami {
namespace gui{


ListItemView::ListItemView(QObject *parent) :
    QAbstractItemModel(parent), editable_(false) {

}

ListItemView::ListItemView(const QList<QPair<QString, QVariant> > &items, QObject *parent) :
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

    if( editable_ && (role == Qt::EditRole || role == Qt::UserRole) ){
        return itemAt( index.row() );
    }

    if( role == Qt::DisplayRole ){
        return keyAt( index.row() );
    }

    return QVariant();
}

Qt::ItemFlags ListItemView::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
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
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

ListItemView &ListItemView::setItems(const QList<QPair<QString, QVariant> > &items) {
    beginResetModel();
    items_ = items;
    endResetModel();
    return *this;
}

ListItemView& ListItemView::addItem(const QPair<QString, QVariant> &item) {
    beginResetModel();
    items_.append( item );
    endResetModel();
    return *this;
}

ListItemView &ListItemView::addItem(const QString &key, const QVariant &value) {
    QPair<QString,QVariant> pair(key,value);
    return addItem(pair);
}

QVariant ListItemView::itemAt(const int &index) const {
    Q_ASSERT( index <= items_.size() );
    return items_.at(index).second;
}

QString ListItemView::keyAt(const int &index) const {
    Q_ASSERT( index <= items_.size() );
    return items_.at(index).first;
}

}
}
