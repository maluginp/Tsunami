#include "measureenableview.h"
#include "dbstorage/measurestorage.h"
#include "models/measuremodel.h"

namespace tsunami{
namespace gui{

MeasureEnableView::MeasureEnableView(int deviceId, const QString &analysis, QObject *parent)
    : QAbstractItemModel(parent),deviceId_(deviceId)  {

    db::MeasureStorage* storage = db::MeasureStorage::instance();

    items_.clear();

    QVariantMap criteria;
    criteria.insert( "device",   deviceId );
    criteria.insert( "analysis", analysis );

    QList<db::MeasureModel *> measures = storage->findMeasures( criteria );
    foreach(db::MeasureModel* measure, measures){
        MeasureEnableViewItem item;

        item.enable = true;
        item.id = measure->id();
        item.name = measure->name();
        item.type = measure->typeJson().toUpper();

        items_.append( item );
    }

    qDeleteAll(measures);
    measures.clear();
}

QModelIndex MeasureEnableView::index(int row, int column, const QModelIndex &parent) const {
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex MeasureEnableView::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int MeasureEnableView::rowCount(const QModelIndex &parent) const {
    return items_.count();
}

int MeasureEnableView::columnCount(const QModelIndex &parent) const {
    return 3;
}

QVariant MeasureEnableView::data(const QModelIndex &index, int role) const {

    if(role != Qt::EditRole && role != Qt::DisplayRole){
        return QVariant();
    }
    int row = index.row();
    int column = index.column();
    switch(column){
    case 0: return items_[row].enable;
    case 1: return items_[row].name;
    case 2: return items_[row].type;
    }

    return QVariant();

}

Qt::ItemFlags MeasureEnableView::flags(const QModelIndex &index) const {
    if(index.column() == 0){
        return (Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

bool MeasureEnableView::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(role != Qt::EditRole){
        return false;
    }

    int row = index.row();
    int column = index.column();

    if(column != 0){
        return false;
    }

    items_[row].enable = value.toBool();

    return true;
}

QVariant MeasureEnableView::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal){
        return QVariant();
    }

    switch( section ){
    case 0: return QVariant("");
    case 1: return QVariant("Name");
    case 2: return QVariant("Type");
    default:
        break;
    }

    return QVariant();
}

QList<int> MeasureEnableView::listEnabledMeasures() {
    QList<int> measures;

    int nItems = items_.count();
    for(int i=0; i < nItems; ++i){
        if(items_[i].enable){
            measures.append( items_[i].id );
        }
    }

    return measures;
}


}
}

