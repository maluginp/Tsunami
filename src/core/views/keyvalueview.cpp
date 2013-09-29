#include "keyvalueview.h"
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QAbstractItemView>
#else
#include <QtWidgets/QAbstractItemView>
#endif
#include "../delegates/keyvaluedelegate.h"

#include <QDebug>
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

    return pairs_.size();
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
            return QVariant( pair.title );
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
        pairs_[index.row()].value = value;
        return true;
    }
    if( role == Qt::DisplayRole && index.column() == 1){
        pairs_[index.row()].value = value ;
    }
    emit dataChanged( index, index );

    return false;
}

QVariant KeyValueView::headerData(int section, Qt::Orientation orientation, int role) const {
    if( role == Qt::DisplayRole && orientation == Qt::Horizontal ){
        if(section == 0){
            return QVariant(tr("Key"));
        }else if(section == 1){
            return QVariant(tr("Value"));
        }
    }
    return QVariant();
}

void KeyValueView::setPairs(const KeyValuePair *pairs, const int &num) {
    beginResetModel();

    pairs_.clear();
    for(int i=0; i < num; ++i){
        addPair( pairs[i] );
    }
    endResetModel();
//    reset()
}

void KeyValueView::addPair(const QString &key, const QVariant &value, const KeyValuePair::ValueType &type, const QString &title) {
    KeyValuePair pair(key,value,type,title);
    addPair(pair);
}

void KeyValueView::addPair(const KeyValuePair &pair) {
    pairs_.append( pair );
}

void KeyValueView::setValue(const QString &key, const QVariant &value) {
    int nPairs = pairs_.size();
    for(int i=0; i < nPairs; ++i){
        if( pairs_[i].key.compare(key,Qt::CaseInsensitive) == 0){
            pairs_[i].value = value;
            break;
        }
    }
    return;
}

void KeyValueView::setPairData(const QString &key, const QVariant &data){
    int nPairs = pairs_.size();
    for(int i=0; i < nPairs; ++i){
        if( pairs_[i].key.compare(key,Qt::CaseInsensitive) == 0){
            pairs_[i].data = data;
        }
    }
    return;
}

void KeyValueView::setPair(const QString &key, const KeyValuePair &pair) {
    int nPairs = pairs_.size();
    for(int i=0; i < nPairs; ++i){
        if( pairs_[i].key.compare(key,Qt::CaseInsensitive) == 0){
            pairs_[i] = pair;
            break;
        }
    }
    return;
}


const KeyValuePair &KeyValueView::getPair(const int &index) const {
    Q_ASSERT( index <= pairs_.size() );
    return pairs_.at(index);
}

const KeyValuePair &KeyValueView::getPair(const QString &key) const {
     int nPairs = pairs_.size();
     for(int i=0; i < nPairs; ++i){
         if(pairs_[i].key.compare(key) == 0){
             return pairs_[i];
         }
     }
     return KeyValuePair();
}

void KeyValueView::fillDelegates(QAbstractItemView *view) {

    view->itemDelegate()->deleteLater();
    view->setItemDelegate( NULL );

    view->setItemDelegate( new KeyValueDelegate( pairs_, view ) );

}
