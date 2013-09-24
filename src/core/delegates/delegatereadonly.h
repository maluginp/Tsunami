#ifndef DELEGATEREADONLY_H
#define DELEGATEREADONLY_H

#include <QItemDelegate>

class DelegateReadOnly : public QItemDelegate
{
public:
    explicit DelegateReadOnly(QObject *parent = 0);
    QWidget* createEditor ( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const;

};

#endif // DELEGATEREADONLY_H
