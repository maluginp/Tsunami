#include "DelegateDoubleItem.h"


DelegateDoubleItem::DelegateDoubleItem(QObject *parent)
{

}

QWidget *DelegateDoubleItem::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {


    QLineEdit* editor = new QLineEdit(parent);
    return editor;

}

void DelegateDoubleItem::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QLineEdit *textEdit = static_cast<QLineEdit*>(editor);
    QString data = index.model()->data(index,Qt::DisplayRole).toString();
    textEdit->setText( data );
}

void DelegateDoubleItem::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QLineEdit *textEdit = static_cast<QLineEdit*>(editor);

    QString data = textEdit->text();

    data.replace( "m","e-3" );
    data.replace( "u", "e-6" );
    data.replace("n","e-9");
    data.replace("p","e-12");
    data.replace("f","e-15");
    data.replace("K","e3");
    data.replace("M","e6");
    data.replace("G","e9");
    data.replace("T","e12");

    double prevData = model->data(index,Qt::EditRole).toDouble();
    bool ok;
    double value = data.toDouble(&ok);
    if(!ok) value = prevData;
    model->setData(index,value,Qt::EditRole);
}



//DoubleTextEdit::DoubleTextEdit(QWidget *parent) : QTextEdit(parent) {

//}

//void DoubleTextEdit::keyPressEvent(QKeyEvent *e) {
//    if(e->key() == Qt::Key_Return){
//        e->ignore();
//    }else{
//        QTextEdit::keyPressEvent( e );
//    }
//}
