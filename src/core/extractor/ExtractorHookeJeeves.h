#ifndef EXTRACTORHOOKEJEEVES_H
#define EXTRACTORHOOKEJEEVES_H

#include "Extractor.h"
namespace tsunami{
namespace core{

class TSUNAMI_EXPORT ExtractorHookeJeeves : public Extractor {
public:
    enum Masks{
        HJ_DEC,
        HJ_INC,
        HJ_HOLD
    };

    ExtractorHookeJeeves(DeviceType type, db::LibraryModel* library, const QList<int>& measures);
public slots:
    void process();
protected:
    bool findBestNearby();
    void patternStep();
    bool decreaseSteps();

    void mask(int index, Masks mask);
    const Masks& mask(int index);
    bool checkConvergence(bool showMessage = true);
private:

    QMap<int,Masks> masks_;

    double tempFunctionError_;
};

}
}


#endif // EXTRACTORHOOKEJEEVES_H
