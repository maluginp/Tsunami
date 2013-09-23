#include "keyvaluedelegate.h"
#include <QComboBox>
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
            comboBox->setCurrentIndex(  itemIndex(value)  );
        }
    }
}

void KeyValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_LIST){
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            //            QString key = items_.key(comboBox->itemData( comboBox->currentIndex(), Qt::UserRole ));

            model->setData(index, comboBox->itemData( comboBox->currentIndex(), Qt::UserRole ), Qt::DisplayRole );
        }
    }

}

void KeyValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_LIST){
            editor->setGeometry(option.rect);
        }
    }
}

void KeyValueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.column() == 1 && index.row() < pairs_.size() ){
        KeyValuePair pair = pairs_[index.row()];
        if(pair.type == KeyValuePair::TYPE_LIST){

            QStyleOptionViewItemV4 myOption = option;
            QString text = items_.key(index.data(Qt::DisplayRole));
            myOption.text = text;
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);

        }
    }
}

