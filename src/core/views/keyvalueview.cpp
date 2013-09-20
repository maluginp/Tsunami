#include "keyvalueview.h"

KeyValueView::KeyValueView(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QModelIndex KeyValueView::index(int row, int column, const QModelIndex &parent) const {
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex KeyValueView::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int KeyValueView::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)

    return keyValues_.size();
}

int KeyValueView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

QVariant KeyValueView::data(const QModelIndex &index, int role) const {
    KeyValuePair pair;

    if(role == Qt::EditRole || role == Qt::DisplayRole){
        pair = getPair( index.row() );
        if(index.column() == 0){
            return QVariant( pair.key );
        }else if(index.column() == 1){
            return pair.value;
        }
    }
    return QVariant();
}

Qt::ItemFlags KeyValueView::flags(const QModelIndex &index) const {
    if(index.column()==0){
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool KeyValueView::setData(const QModelIndex &index, const QVariant &value, int role) {
    if( role == Qt::EditRole && index.column() == 1 ){
        keyValues_[index.row()].value = value;
        return true;
    }

    return false;
}

QVariant KeyValueView::headerData(int section, Qt::Orientation orientation, int role) const {
    if( role == Qt::DisplayRole && orientation == Qt::Horizontal ){
        if(section == 0){
            return QVariant("Ключ");
        }else if(section == 1){
            return QVariant("Значение");
        }
    }
    return QVariant();
}


const KeyValuePair &KeyValueView::getPair(const int &index) const {
    return keyValues_.at(index);
}
