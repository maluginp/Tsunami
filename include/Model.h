#ifndef MODEL_H
#define MODEL_H
#include <QString>
class Model{
protected:
    Model();
    bool compare(const QString& str1, const QString& str2 ){
        return (str1.compare(str2, Qt::CaseInsensitive ) == 0);
    }

};

#endif // MODEL_H
