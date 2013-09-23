#ifndef KEYVALUEDELEGATE_H
#define KEYVALUEDELEGATE_H

#include <QItemDelegate>
#include "../views/keyvalueview.h"

class KeyValueDelegate : public QItemDelegate
{
public:
    KeyValueDelegate(const QList<KeyValuePair>& pairs, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
     int itemIndex(const KeyValuePair& pair, const QVariant& value ) const;


    QList<KeyValuePair> pairs_;
};

#endif // KEYVALUEDELEGATE_H
