#ifndef MEASUREITEMVIEW_H
#define MEASUREITEMVIEW_H

#include "defines.h"
#include "dbstorage/measurestorage.h"

namespace tsunami {
namespace gui{

class  MeasureItemView : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit MeasureItemView(db::MeasureModel* model, QObject *parent = 0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    bool isColumnReadOnly(int index );

signals:
    

private:

    int rows_;
    int columns_;

    db::MeasureModel* measure_;

};

}
}
#endif // MEASUREITEMVIEW_H
