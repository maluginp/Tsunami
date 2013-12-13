#ifndef DELEGATEREADONLY_H
#define DELEGATEREADONLY_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QItemDelegate>
#else
#include <QtWidgets/QItemDelegate>
#endif

namespace tsunami{
namespace gui{


class DelegateReadOnly : public QItemDelegate
{
public:
    explicit DelegateReadOnly(QObject *parent = 0);
    QWidget* createEditor ( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const;

};

}
}

#endif // DELEGATEREADONLY_H
