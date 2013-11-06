#ifndef TERMINAL_H
#define TERMINAL_H

#include "graphnode.h"

namespace tsunami{
namespace core{

class Terminal : GraphNode  {
public:
    Terminal(const QString& name);

    void attachDevice(GraphNode* node);
    void detachDevice(GraphNode* node);

    void setRef();
    void isRef();
private:
    bool isRef_;

};

}
}


#endif // TERMINAL_H
