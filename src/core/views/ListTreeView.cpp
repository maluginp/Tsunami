#include "ListTreeView.h"
namespace tsunami{
namespace gui {


ListTreeView::ListTreeView(const QString &title, const QVariantMap &rootMap, QObject *parent) :
    QAbstractItemModel(parent) {
    rootItem_ = new ListTreeViewItem( "ROOT", "root", 0 );
    foreach(QString key, rootMap.keys()){
          rootItem_->addChild( new ListTreeViewItem( key, rootMap.value(key), rootItem_ ) );
    }
    title_ = title;
}

ListTreeView::~ListTreeView() {
    delete rootItem_;
}

QVariant ListTreeView::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ListTreeViewItem *item = static_cast<ListTreeViewItem*>(index.internalPointer());
    if(item == 0){
        return QVariant();
    }

    if(role == Qt::DisplayRole){
        return item->name();
    }else if(role == Qt::UserRole){
        return item->value();
    }
    return QVariant();
}

Qt::ItemFlags ListTreeView::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ListTreeView::headerData(int section, Qt::Orientation orientation, int role) const {
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 ){
        return QVariant(title_);
    }
    return QVariant();

}

QModelIndex ListTreeView::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ListTreeViewItem *parentItem;

    if (!parent.isValid()){
        parentItem = rootItem_;
    }else{
        parentItem = static_cast<ListTreeViewItem *>(parent.internalPointer());
    }
    ListTreeViewItem *childItem = parentItem->child(row);
    if (childItem){
        if(childItem->parent() == rootItem_){
            if(childItem->numberChilds() > 0){
               return createIndex(row, column, childItem);
            }
        } else {
            return createIndex(row, column, childItem);
        }
    }

    return QModelIndex();
}

QModelIndex ListTreeView::parent(const QModelIndex &index) const{
    if (!index.isValid())
        return QModelIndex();

    ListTreeViewItem *childItem = static_cast<ListTreeViewItem*>(index.internalPointer());

    ListTreeViewItem *parentItem = childItem->parent();
    if(parentItem == rootItem_ || parentItem == 0){
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ListTreeView::rowCount(const QModelIndex &parent) const {
    ListTreeViewItem *parentItem;
    if (parent.column() > 0)
         return 0;

    if( !parent.isValid() ){
        parentItem = rootItem_;
    }else{
       parentItem = static_cast<ListTreeViewItem *>(parent.internalPointer());
    }

    if(parentItem == 0){
        return 0;
    }

    return parentItem->numberChilds();
}

int ListTreeView::columnCount(const QModelIndex &parent) const {
    return 1;
}

void ListTreeView::addChild(const QString &rootName, const QString &name, const QVariant &value) {
    beginResetModel();

    int nItems = rootItem_->numberChilds();
    for(int i=0; i < nItems; ++i){
        ListTreeViewItem* item = rootItem_->child(i);
        if(item->name().compare(rootName,Qt::CaseInsensitive) == 0){
            item->addChild( new ListTreeViewItem(name,value,item) );
        }
    }
    endResetModel();
}

void ListTreeView::clear() {
   beginResetModel();
   int nItems = rootItem_->numberChilds();
   for(int i=0; i < nItems; ++i ){
       rootItem_->child(i)->removeChilds();
   }
   endResetModel();
}

// ListTreeViewItem IMPLEMENTATIONS
ListTreeViewItem::ListTreeViewItem(QString name, QVariant value, ListTreeViewItem *parent) {
    name_ = name;
    value_ = value;
    parent_ = parent;
}

ListTreeViewItem::~ListTreeViewItem() {
    parent_ = 0;
    qDeleteAll( childs_ );
}

int ListTreeViewItem::numberChilds() {
    return childs_.count();
}

const QVariant &ListTreeViewItem::value() const{
    return value_;
}

const QString &ListTreeViewItem::name() const{
    return name_;
}

int ListTreeViewItem::row() const {
    if (parent_){
        return parent_->childs_.indexOf( const_cast<ListTreeViewItem*>(this) );
    }
    return 0;
}

void ListTreeViewItem::addChild(ListTreeViewItem *child) {
    childs_.append( child );
}

void ListTreeViewItem::removeChilds() {
    qDeleteAll(childs_);
    childs_.clear();
}

ListTreeViewItem *ListTreeViewItem::child(int number) {
    return childs_[number];
}

ListTreeViewItem *ListTreeViewItem::parent(){
    return parent_;
}

} //gui
} //tsunami
