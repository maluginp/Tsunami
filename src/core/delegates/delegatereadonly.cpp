#include "delegatereadonly.h"
#include <QtGui>
DelegateReadOnly::DelegateReadOnly(QObject *parent) {
    Q_UNUSED(parent);
}

QWidget *DelegateReadOnly::createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return 0;
}

void DelegateReadOnly::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
//    QColor color(51,51,51);

    QColor background(0xDA,0xDA,0xDA);
    QStyleOptionViewItemV2 opt = setOptions(index, option);
    opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
//    opt.palette.setColor(QPalette::Text,color);
//    opt.font.setBold(true);

    painter->fillRect(option.rect,background);

    painter->setBackground(background);
    QItemDelegate::paint(painter, opt, index);
}
