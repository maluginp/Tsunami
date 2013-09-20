#include "listitemview.h"

ListItemView::ListItemView(QObject *parent) :
    QAbstractItemModel(parent)
{
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
    return 0;
}

int ListItemView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 0;
}

QVariant ListItemView::data(const QModelIndex &index, int role) const {
    return QVariant();
}

Qt::ItemFlags ListItemView::flags(const QModelIndex &index) const {
    return Qt::NoItemFlags;
}


bool ListItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    return false;
}

QVariant ListItemView::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}
