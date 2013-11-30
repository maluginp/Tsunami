#include "parameteritemview.h"
#include "dbstorage/parameterstorage.h"
#include <QItemSelectionModel>
namespace tsunami {
namespace gui{

ParameterItemView::ParameterItemView(db::LibraryModel* library, QObject *parent) :
    QAbstractItemModel(parent) {

    library_ = library;

    // enable, name, initital, fitted, minimum, maximum, fixed
    columns_ = 7;

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
    return library_->countParameters();
}

int ParameterItemView::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return columns_;
}

QVariant ParameterItemView::data(const QModelIndex &index, int role) const {
    if(role != Qt::EditRole && role != Qt::DisplayRole){
        return QVariant();
    }

    int row = index.row();
    int column = index.column();

    switch(column){
    case 0: return library_->at(row).enable();
    case 1: return library_->at(row).name();
    case 2: return QVariant(library_->at(row).initial());
    case 3: return library_->at(row).fitted();
    case 4: return library_->at(row).minimum();
    case 5: return library_->at(row).maximum();
    case 6: return library_->at(row).fixed();
    }


    return QVariant();
}

Qt::ItemFlags ParameterItemView::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}


bool ParameterItemView::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(role != Qt::EditRole){
        return false;
    }

    int row = index.row();
    int column = index.column();

//    double digit;
//    if(column >= 2 && column <= 5){

//    }

    switch(column){
    case 0: library_->at(row).enable(  value.toBool() ); break;
    case 1: library_->at(row).name(    value.toString() ); break;
    case 2: library_->at(row).initial( value.toDouble()); break;
    case 3: library_->at(row).fitted(  value.toDouble() ); break;
    case 4: library_->at(row).minimum( value.toDouble()); break;
    case 5: library_->at(row).maximum( value.toDouble()); break;
    case 6: library_->at(row).fixed(   value.toBool()); break;
    default:
        return false;
    }
    return true;
}

QVariant ParameterItemView::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Horizontal){
        switch(section){
        case 0: return QString("");
        case 1: return QString("Name");
        case 2: return QString("Initial");
        case 3: return QString("Fitted");
        case 4: return QString("Minimum");
        case 5: return QString("Maximum");
        case 6: return QString("Fixed");
        }
    }

    if(orientation == Qt::Vertical){
        return QVariant();
    }

    return QVariant();
}

void ParameterItemView::addEmptyParameter() {
    beginResetModel();
    library_->addEmptyParameter();
    endResetModel();
}

void ParameterItemView::removeSelectedParameters(const QList<int>& indexes) {
    beginResetModel();
    library_->removeParameters( indexes );
    endResetModel();
}

void ParameterItemView::removeSelectedParameter(const QModelIndex &index) {
    int row = index.row();

    beginResetModel();
    library_->removeParameters( QList<int>() << row );
    endResetModel();
}

}
}
