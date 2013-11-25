#ifndef DELEGATECHECKBOX_H
#define DELEGATECHECKBOX_H

#include <QtCore>
#include <QtGui>

class DelegateCheckBox : public QItemDelegate
{
public:
    DelegateCheckBox(QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //Qt::ItemFlags flags ( const QModelIndex & index ) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);


};

#endif // DELEGATECHECKBOX_H
