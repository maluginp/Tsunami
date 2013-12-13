#ifndef DELEGATEDOUBLEITEM_H
#define DELEGATEDOUBLEITEM_H
#include <QItemDelegate>
#include <QTextEdit>
#include <QKeyEvent>

class DelegateDoubleItem : public QItemDelegate {
public:
    DelegateDoubleItem(QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;


};

#endif // DELEGATEDOUBLEITEM_H
