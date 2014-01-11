#include "KeyValueDelegate.h"
#include "views/KeyValueView.h"
namespace tsunami{
namespace gui{

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
KeyValueDelegate::KeyValueDelegate(const QList<KeyValuePair> &pairs, QObject *parent) {
    Q_UNUSED(parent);
    pairs_ = pairs;
}

QWidget *KeyValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];

        if(pair.type == KeyValuePair::TYPE_LIST){
            QComboBox* editor = new QComboBox(parent);
            QVariantMap items = pair.data.toMap();

            foreach(QString key,items.keys()){
                editor->addItem( key, items.value( key ) );
            }
            return editor;
        }else if(pair.type == KeyValuePair::TYPE_CHECKBOX){
            QCheckBox *editor = new QCheckBox(parent);
            editor->installEventFilter(const_cast<KeyValueDelegate*>(this));

            return NULL;
        }else if(pair.type == KeyValuePair::TYPE_DATE){
            QDateEdit* editor = new QDateEdit(parent);
            return editor;
        }else if(pair.type == KeyValuePair::TYPE_READONLY){
            return NULL;
        }
    }

    return QItemDelegate::createEditor(parent,option,index);
}

void KeyValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];

        if(pair.type == KeyValuePair::TYPE_LIST){
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QVariant value  = index.model()->data(index, Qt::EditRole);
            comboBox->setCurrentIndex( itemIndex(pair,value) );
            return;
        }else if(pair.type == KeyValuePair::TYPE_CHECKBOX){
            bool value = index.model()->data(index, Qt::DisplayRole).toBool();
            QCheckBox *checkBox = static_cast<QCheckBox*>(editor);

            if(value){
                checkBox->setCheckState(Qt::Checked);
            }else{
                checkBox->setCheckState(Qt::Unchecked);
            }
            return;
        }
    }

    QItemDelegate::setEditorData(editor,index);

}

void KeyValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_LIST){

            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            model->setData(index, comboBox->itemData( comboBox->currentIndex(), Qt::UserRole ), Qt::DisplayRole );
            return;
        }else if(pair.type == KeyValuePair::TYPE_CHECKBOX){
            QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
            int value = 0;
            if(checkBox->checkState() == Qt::Checked){
                value = 1;
            }else{
                value = 0;
            }
            model->setData(index, value,  Qt::EditRole);
            model->setData(index, value,  Qt::CheckStateRole);
            return;
        }
    }

    QItemDelegate::setModelData(editor,model,index);

}

void KeyValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_LIST){
            editor->setGeometry(option.rect);
            return;
        }else if(pair.type == KeyValuePair::TYPE_CHECKBOX){
            editor->setGeometry(option.rect);
            return;
        }
    }

    QItemDelegate::updateEditorGeometry(editor,option,index);
}

void KeyValueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_LIST){
            QVariantMap items = pair.data.toMap();
            QStyleOptionViewItemV4 myOption = option;
            QString text = items.key(index.data(Qt::DisplayRole));
            myOption.text = text;
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
            return;
        }else if(pair.type == KeyValuePair::TYPE_CHECKBOX){
            bool checked = index.model()->data(index, Qt::EditRole).toBool();

            QStyleOptionButton check_box_style_option;
            check_box_style_option.state |= QStyle::State_Enabled;
            if (checked) {
                check_box_style_option.state |= QStyle::State_On;
            } else {
                check_box_style_option.state |= QStyle::State_Off;
            }
            check_box_style_option.rect = CheckBoxRect(option);

            QApplication::style()->drawControl(QStyle::CE_CheckBox,&check_box_style_option,painter);
            return;
        }else if(pair.type == KeyValuePair::TYPE_DATE){
            QStyleOptionViewItemV4 myOption = option;
            QString text = index.data(Qt::EditRole).toDate().toString( "dd.MM.yyyy" );
            myOption.text = text;
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
            return;
        }else if(pair.type == KeyValuePair::TYPE_READONLY){
            QColor background(0xDA,0xDA,0xDA);
            QStyleOptionViewItemV2 opt = setOptions(index, option);
            opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
        //    opt.palette.setColor(QPalette::Text,color);
        //    opt.font.setBold(true);

            painter->fillRect(option.rect,background);

            painter->setBackground(background);
//            return;
        }
    }else if(index.column() == 0){ // Keys style
        QStyleOptionViewItemV2 opt = setOptions(index, option);
        opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
        opt.font.setBold(true);
        QItemDelegate::paint(painter, opt, index);
        return;

    }

    QItemDelegate::paint(painter,option,index);
}

bool KeyValueDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_CHECKBOX){
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
    }
    return QItemDelegate::editorEvent(event,model,option,index);
}

int KeyValueDelegate::itemIndex(const KeyValuePair &pair, const QVariant &value) const {
    if(pair.type == KeyValuePair::TYPE_LIST){
        int index=0;
        QVariantMap items = pair.data.toMap();
        foreach(QVariant item, items.values()){
            if(item==value){
                return index;
            }
            ++index;
        }

    }
    return -1;
}

} // gui
} // tsunami

