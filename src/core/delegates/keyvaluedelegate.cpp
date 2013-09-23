#include "keyvaluedelegate.h"
#include <QComboBox>
#include <QApplication>

KeyValueDelegate::KeyValueDelegate(const QList<KeyValuePair> &pairs, QObject *parent) {
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
        }
    }

    QItemDelegate::paint(painter,option,index);
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

