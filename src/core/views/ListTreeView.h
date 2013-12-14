#ifndef LISTTREEVIEW_H
#define LISTTREEVIEW_H

#include "defines.h"

namespace tsunami{
namespace gui {

class  TSUNAMI_EXPORT ListTreeViewItem{
public:
    ListTreeViewItem( QString name, QVariant value, ListTreeViewItem* parent = 0 );
    ~ListTreeViewItem();
    int numberChilds();
    const QVariant& value() const;
    const QString& name() const;
    int row() const;

    void addChild(ListTreeViewItem* child );
    void removeChilds();
    ListTreeViewItem* child( int number );
    ListTreeViewItem* parent();


private:
    ListTreeViewItem* parent_;
    QList<ListTreeViewItem*> childs_;
    QString name_;
    QVariant value_;

};

class TSUNAMI_EXPORT ListTreeView : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ListTreeView(const QString& title, const QVariantMap& rootMap, QObject *parent = 0);
    ~ListTreeView();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void addChild(const QString& rootName, const QString& name, const QVariant& value );

    void clear();
private:
    ListTreeViewItem* rootItem_;
    QString title_;
signals:

public slots:
private:

    QMap<QString, QPair<QString,QVariant> > items_;

};

}
}

#endif // LISTTREEVIEW_H
