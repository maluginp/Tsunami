#ifndef KEYVALUEDELEGATE_H
#define KEYVALUEDELEGATE_H

#include <QItemDelegate>
#include "views/KeyValueView.h"

namespace tsunami {
namespace gui{
class TSUNAMI_EXPORT KeyValueDelegate : public QItemDelegate {
public:
    KeyValueDelegate(const QList<KeyValuePair>& pairs, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);

private:
     int itemIndex(const KeyValuePair& pair, const QString &foundKey ) const;


    QList<KeyValuePair> pairs_;
};

} //gui
} //tsunami

#endif // KEYVALUEDELEGATE_H
