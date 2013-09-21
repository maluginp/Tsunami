#ifndef LISTITEMVIEW_H
#define LISTITEMVIEW_H

#include <QAbstractItemModel>

class ListItemView : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ListItemView(QObject *parent = 0);
    explicit ListItemView(const QVariantMap& items, QObject *parent = 0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    void setItems(const QVariantMap &items);
signals:
public slots:
private:
    QVariant itemAt( const int& index ) const;
    QString  keyAt( const int& index ) const;

    QVariantMap items_;
    bool editable_;
};

#endif // LISTITEMVIEW_H
