#include "Terminal.h"

namespace tsunami{
namespace spice{

Terminal::Terminal(const QString &name)
    : GraphNode(name)
    , isRef_(false) {

}

void Terminal::attachDevice(GraphNode *node) {
    link(node);
}

void Terminal::detachDevice(GraphNode *node) {
    unlink(node);
}

void Terminal::setRef() {
    isRef_ = true;
}

bool Terminal::isRef() {
    return isRef_;
}



}
}
