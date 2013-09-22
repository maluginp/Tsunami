#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;


class ComboBoxDelegate : public QItemDelegate {
Q_OBJECT
public:
  ComboBoxDelegate(int row, int column, QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

  void setItems( const QVariantMap& items );

private:
    QVariantMap items_;
    int rowItem_;
    int columnItem_;
};

#endif // COMBOBOXDELEGATE_H
