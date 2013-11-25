#ifndef PARAMETERITEMVIEW_H
#define PARAMETERITEMVIEW_H

#include <QAbstractItemModel>
#include "../dbstorage/parameterstorage.h"

namespace tsunami {
namespace gui{

class ParameterItemView : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParameterItemView(db::LibraryModel* library, QObject *parent = 0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole )  const;

signals:
    
private:
    int columns_;
    db::LibraryModel* library_;
};

}
}

#endif // PARAMETERITEMVIEW_H
