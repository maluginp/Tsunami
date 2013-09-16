#ifndef PARAMETERITEMVIEW_H
#define PARAMETERITEMVIEW_H

#include <QAbstractItemModel>

class ParameterItemView : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParameterItemView(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // PARAMETERITEMVIEW_H
