#include "DelegateCheckBox.h"

static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options) {
  QStyleOptionButton check_box_style_option;
  QRect check_box_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator,&check_box_style_option);
  QPoint check_box_point(view_item_style_options.rect.x() +
                         view_item_style_options.rect.width() / 2 -
                         check_box_rect.width() / 2,
                         view_item_style_options.rect.y() +
                         view_item_style_options.rect.height() / 2 -
                         check_box_rect.height() / 2);
  return QRect(check_box_point, check_box_rect.size());
}


DelegateCheckBox::DelegateCheckBox(QObject *parent){
}

QWidget *DelegateCheckBox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *editor = new QCheckBox(parent);
    editor->installEventFilter(const_cast<DelegateCheckBox*>(this));

//     return editor;
    return NULL;
}

void DelegateCheckBox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool value = index.model()->data(index, Qt::DisplayRole).toBool();
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);

    if(value){
        checkBox->setCheckState(Qt::Checked);
    }else{
        checkBox->setCheckState(Qt::Unchecked);
    }
}

void DelegateCheckBox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);

    int value = 0;

    if(checkBox->checkState() == Qt::Checked){
        value = 1;
    }else{
        value = 0;
    }

    model->setData(index, value,  Qt::CheckStateRole);

}

void DelegateCheckBox::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void DelegateCheckBox::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    QStyleOptionButton check_box_style_option;
    check_box_style_option.state |= QStyle::State_Enabled;
    if (checked) {
      check_box_style_option.state |= QStyle::State_On;
    } else {
      check_box_style_option.state |= QStyle::State_Off;
    }
    check_box_style_option.rect = CheckBoxRect(option);

    QApplication::style()->drawControl(QStyle::CE_CheckBox,&check_box_style_option,painter);

}

bool DelegateCheckBox::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if ((event->type() == QEvent::MouseButtonRelease) ||
        (event->type() == QEvent::MouseButtonDblClick)) {
      QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
      if (mouse_event->button() != Qt::LeftButton ||
          !CheckBoxRect(option).contains(mouse_event->pos())) {
        return false;
      }
      if (event->type() == QEvent::MouseButtonDblClick) {
        return true;
      }
    } else if (event->type() == QEvent::KeyPress) {
      if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
          static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select) {
        return false;
      }
    } else {
      return false;
    }

    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
    return model->setData(index, !checked, Qt::EditRole);

}
