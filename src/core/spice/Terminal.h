#ifndef TERMINAL_H
#define TERMINAL_H

#include "GraphNode.h"

namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT Terminal : public GraphNode  {
public:
    Terminal(const QString& name);

    void attachDevice(GraphNode* node);
    void detachDevice(GraphNode* node);

    void setRef();
    bool isRef();
private:
    bool isRef_;

};

}
}


#endif // TERMINAL_H
