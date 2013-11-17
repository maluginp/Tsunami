#include "parameteritemview.h"

namespace tsunami {
namespace gui{

ParameterItemView::ParameterItemView(const int& libraryId,QObject *parent) :
    QAbstractItemModel(parent) {

    storage_ = ParameterStorage::instance();
    openParameters( libraryId );
}

QModelIndex ParameterItemView::index(int row, int column, const QModelIndex &parent) const{
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex ParameterItemView::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

int ParameterItemView::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return rows_;
}

int ParameterItemView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return columns_.size();
}

QVariant ParameterItemView::data(const QModelIndex &index, int role) const {
    if(role != Qt::EditRole && role != Qt::DisplayRole){
        return QVariant();
    }

    return library_.parameterValue( index );
}

Qt::ItemFlags ParameterItemView::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}


void ParameterItemView::openParameters(const int &libraryId) {
    library_ = storage_->openLibrary( libraryId );
    storeLibrary_ = library_;
    rows_ = library_.parameters().size();
    columns_ = ParameterModel::getColumns();
}

bool ParameterItemView::saveParameters() {
    return storage_->saveLibrary( library_ );
}


void ParameterItemView::restoreParameters() {
    library_ = storeLibrary_;
}


bool ParameterItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(role != Qt::EditRole){
        return false;
    }
    return library_.setParameterValue( index, value );
}

QVariant ParameterItemView::headerData(int section, Qt::Orientation orientation, int role) {
    if( role != Qt::DisplayRole ){
        return QVariant();
    }

    if(orientation == Qt::Horizontal){
        qDebug() << "section:" << section;
        return columns_.at(section);
    }

    if(orientation == Qt::Vertical){
        return QVariant(library_.parameter( section ).name());
    }

    return QVariant();
}

}
}
