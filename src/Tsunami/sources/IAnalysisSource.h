#ifndef IANALYSISSOURCE_H
#define IANALYSISSOURCE_H

#include "views/KeyValueView.h"
namespace tsunami{
class IAnalysisSource {
public:
    IAnalysisSource();

    virtual gui::KeyValuePairs pairs() const = 0;
};

}

#endif // IANALYSISSOURCE_H
