#ifndef PARAMETERITEMVIEW_H
#define PARAMETERITEMVIEW_H

#include <QAbstractItemModel>
#include "dbstorage/parameterstorage.h"

class LibraryModel;

class ParameterItemView : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParameterItemView(const int &libraryId, QObject *parent = 0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    void openParameters( const int& libraryId );

signals:
    
public slots:
    bool saveParameters();
    void restoreParameters();
private:
    int rows_;
    QVariantList columns_;

    LibraryModel library_;
    LibraryModel storeLibrary_;

    ParameterStorage* storage_;
};

#endif // PARAMETERITEMVIEW_H
