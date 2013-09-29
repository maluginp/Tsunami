#ifndef PARAMETERPLOTVIEW_H
#define PARAMETERPLOTVIEW_H

#include <QAbstractItemModel>

class ParameterPlotView : public QAbstractItemModel {
Q_OBJECT
public:
    explicit ParameterPlotView(QVariantMap parameters, QObject *parent = 0);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QPair<QString,QVariant> itemAt(int index) const;
    QPair<QString,QVariant> changeAt(int index) const;


private:
    QVariantMap items_;
    QVariantMap changes_;
};

#endif // PARAMETERPLOTVIEW_H
