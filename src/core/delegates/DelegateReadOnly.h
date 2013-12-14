#ifndef DELEGATEREADONLY_H
#define DELEGATEREADONLY_H

#include "defines.h"

namespace tsunami{
namespace gui{


class TSUNAMI_EXPORT DelegateReadOnly : public QItemDelegate
{
public:
    explicit DelegateReadOnly(QObject *parent = 0);
    QWidget* createEditor ( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const;

};

}
}

#endif // DELEGATEREADONLY_H
