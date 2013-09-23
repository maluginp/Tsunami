#ifndef KEYVALUEDELEGATE_H
#define KEYVALUEDELEGATE_H

#include <QItemDelegate>
#include "../views/keyvalueview.h"

class KeyValueDelegate : public QItemDelegate
{
public:
    KeyValueDelegate(const QList<KeyValuePair>& pairs, QObject *parent = 0);

private:
    QList<KeyValuePair> pairs_;
};

#endif // KEYVALUEDELEGATE_H
