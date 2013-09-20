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
        pairs_[index.row()].value = value;
        return true;
    }

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

void KeyValueView::addPairs(const KeyValuePair *pairs, const int &num) {

    for(int i=0; i < num; ++i){
        addPair( pairs[i] );
    }

}

void KeyValueView::addPair(const QString &key, const QVariant &value, const KeyValuePair::ValueType &type) {
    KeyValuePair pair(key,value,type);
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
    return pairs_.at(index);
}
