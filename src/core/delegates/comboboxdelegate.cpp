#include "comboboxdelegate.h"
#include <QApplication>
#include <QComboBox>
#include <QDebug>
ComboBoxDelegate::ComboBoxDelegate(int row, int column, QObject *parent) :
    rowItem_(row), columnItem_(column) {

}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    if( index.row() == rowItem_ && index.column() == columnItem_ ){
        QComboBox* editor = new QComboBox(parent);
        foreach(QString key,items_.keys()){
            editor->addItem( key, items_.value( key ) );
        }
        return editor;
    }
    return QItemDelegate::createEditor(parent,option,index);
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if( index.row() == rowItem_ && index.column() == columnItem_ ){
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QVariant value  = index.model()->data(index, Qt::EditRole);
        qDebug() << value;
        comboBox->setCurrentIndex(  itemIndex(value)  );
    }else{
        QItemDelegate::setEditorData(editor,index);
    }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if( index.row() == rowItem_ && index.column() == columnItem_ ){
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString key = items_.key(comboBox->itemData( comboBox->currentIndex(), Qt::UserRole ));
        qDebug() <<  QString("Key:%1, User:%2, Display:%3, Edit:%4")
                     .arg(key)
                     .arg(comboBox->itemData( comboBox->currentIndex(), Qt::UserRole ).toString())
                     .arg(comboBox->itemData( comboBox->currentIndex(), Qt::DisplayRole ).toString())
                     .arg(comboBox->itemData( comboBox->currentIndex(), Qt::EditRole ).toString());

        model->setData(index, comboBox->itemData( comboBox->currentIndex(), Qt::UserRole ), Qt::DisplayRole );
//        model->setData(index , key, Qt::DisplayRole);
    }else{
        QItemDelegate::setModelData(editor,model,index);
    }
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.row() == rowItem_ && index.column() == columnItem_ ){
        editor->setGeometry(option.rect);
    }else{
        QItemDelegate::updateEditorGeometry(editor,option,index);
    }
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.row() == rowItem_ && index.column() == columnItem_ ){
        QStyleOptionViewItemV4 myOption = option;
        QString text = items_.key(index.data(Qt::DisplayRole));

        myOption.text = text;

        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
    }else{
        QItemDelegate::paint(painter,option,index);
    }
}

void ComboBoxDelegate::setItems(const QVariantMap &items) {
    items_ = items;
}

int ComboBoxDelegate::itemIndex(const QVariant &value) const {
    int row=0;
    foreach(QVariant item,items_.values()){
        if(value == item){
            return row;
        }
        ++row;
    }
    return -1;
}

