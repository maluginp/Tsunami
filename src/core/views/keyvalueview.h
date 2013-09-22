#ifndef KEYVALUEVIEW_H
#define KEYVALUEVIEW_H

#include <QAbstractItemModel>
#include <QString>

class QAbstractItemView;

struct KeyValuePair{
    enum ValueType{
        TYPE_TEXT,
        TYPE_CHECKBOX,
        TYPE_READONLY,
        TYPE_DATE,
        TYPE_LIST
    };

    QString  key;
    QString  title;
    // текущее заданое значение
    QVariant value;
    ValueType type;
    QVariant data;

    KeyValuePair( const QString& keyPair, const QVariant& valuePair, const ValueType& typePair, const QString& titlePair)
        : key(keyPair), value(valuePair), type(typePair), title(titlePair) { }

    KeyValuePair() :
        key(QString()),value(QVariant()),type(KeyValuePair::TYPE_TEXT) {}
    KeyValuePair(const KeyValuePair& pair):
        key(pair.key),value(pair.value),type(pair.type),title(pair.title) {}

};

class KeyValueView : public QAbstractItemModel {
    Q_OBJECT
public:
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
    void setPairs( const KeyValuePair* pairs,  const int& num );
    void addPair ( const QString& key, const QVariant& value, const KeyValuePair::ValueType& type, const QString& title );
    void addPair ( const KeyValuePair& pair );
    void setValue( const QString& key, const QVariant& value );
    void setPairData(  const QString& key, const QVariant& data);
    void setPair ( const QString& key, const KeyValuePair& pair );

    const QList<KeyValuePair>& getPairs() const { return pairs_; }
    const KeyValuePair& getPair( const int& index ) const;
    const KeyValuePair& getPair(const QString& key) const;

    void fillDelegates( QAbstractItemView* view);
signals:    
public slots:
private:
    QList<KeyValuePair> pairs_;

};


#endif // KEYVALUEVIEW_H
