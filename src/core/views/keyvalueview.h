#ifndef KEYVALUEVIEW_H
#define KEYVALUEVIEW_H

#include <QAbstractItemModel>

struct KeyValuePair{
    QString  key;
    QVariant value;
    KeyValueView::ValueType type;
};

class KeyValueView : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum ValueType{
        TYPE_TEXT,
        TYPE_CHECKBOX,
        TYPE_READONLY
    };

    explicit KeyValueView(QObject *parent = 0);
//    void setValues(const QVariantMap& keyValues);
    void addPair( const QString& key, const QVariant& value, const ValueType& type );
    void setValue( const QString& key, const QVariant& value );


    const QList<KeyValuePair>& getPairs() const { return keyValues_; }

signals:
    
public slots:
private:
    QList<KeyValuePair> keyValues_;


};

#endif // KEYVALUEVIEW_H
