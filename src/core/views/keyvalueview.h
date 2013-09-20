#ifndef KEYVALUEVIEW_H
#define KEYVALUEVIEW_H

#include <QAbstractItemModel>

struct KeyValuePair{
    QString  key;
    QVariant value;
    KeyValueView::ValueType type;

    KeyValuePair() :
        key(QString()),value(QVariant()),type(KeyValueView::TYPE_TEXT) {}
    KeyValuePair(const KeyValuePair& pair):
        key(pair.key),value(pair.value),type(pair.type) {}
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

    // Model
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;



    // API
    void addPairs( const KeyValuePair* pairs,  const int& num );
    void addPair( const QString& key, const QVariant& value, const ValueType& type );
    void setValue( const QString& key, const QVariant& value );
    void setPair( const QString& key, const KeyValuePair& pair );

    const QList<KeyValuePair>& getPairs() const { return keyValues_; }
    const KeyValuePair& getPair( const int& index ) const;


signals:
    
public slots:
private:
    QList<KeyValuePair> keyValues_;


};

#endif // KEYVALUEVIEW_H
