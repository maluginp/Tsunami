#ifndef PARAMETERSEXTRACTIONVIEW_H
#define PARAMETERSEXTRACTIONVIEW_H

#include "defines.h"

namespace tsunami{
namespace db{
    class LibraryModel;
}
namespace gui{

class TSUNAMI_EXPORT ParametersExtractionView : public QAbstractItemModel {
public:
    ParametersExtractionView(db::LibraryModel* library,QObject *parent=0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

private:
    db::LibraryModel* library_;

};

}
}
#endif // PARAMETERSEXTRACTIONVIEW_H
