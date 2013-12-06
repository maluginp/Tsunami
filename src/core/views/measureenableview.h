#ifndef MEASUREENABLEVIEW_H
#define MEASUREENABLEVIEW_H
#include <QAbstractItemModel>
#include <QVector>
namespace tsunami{
namespace gui{

struct MeasureEnableViewItem{
    bool enable;
    int id;
    QString name;
    QString type;

    MeasureEnableViewItem() : enable(false), id(-1) {}

    MeasureEnableViewItem(const MeasureEnableViewItem& other)
        : enable(other.enable), id(other.id), name(other.name),type(other.type){

    }

    MeasureEnableViewItem& operator=(const MeasureEnableViewItem& other){
        enable = other.enable;
        id = other.id;
        name = other.name;
        type = other.type;
        return *this;
    }
};

class MeasureEnableView : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit MeasureEnableView(int deviceId,QObject *parent = 0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QList<int> listEnabledMeasures();
private:
    QVector<MeasureEnableViewItem> items_;
    int deviceId_;
};

}
}

#endif // MEASUREENABLEVIEW_H
