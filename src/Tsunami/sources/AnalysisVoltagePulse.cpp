#include "AnalysisVoltagePulse.h"

namespace tsunami{
//using namespace spice;
using namespace gui;

AnalysisVoltagePulse::AnalysisVoltagePulse()
    : IAnalysisSource()
{
//    source_ = VoltagePulseDevice();
}


KeyValuePairs AnalysisVoltagePulse::pairs() const {
    QList<KeyValuePair> pairs;

//    KeyValuePair pair;
//    pair.key = "vinitial";
//    pair.value = QString::number( source_.voltageInitial() );
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title =  "Voltage initial";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "von";
//    pair.value = QString::number( source_.voltageOn());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Voltage on";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "tdelay";
//    pair.value = QString::number(source_.timeDelay());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Time delay";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "trise";
//    pair.value = QString::number(source_.timeRise());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Time rise";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "tfall";
//    pair.value = QString::number(source_.timeFall());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Time fall";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "ton";
//    pair.value = QString::number(source_.timeOn());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Time on";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "tperiod";
//    pair.value = QString::number(source_.timePeriod());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Time period";
//    pairs.append( pair );

//    pair = KeyValuePair();
//    pair.key = "ncycles";
//    pair.value = QString::number(source_.numberCycles());
//    pair.type = KeyValuePair::TYPE_TEXT;
//    pair.title = "Number cycles";
//    pairs.append( pair );

    return pairs;
}

}
