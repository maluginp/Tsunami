#include "terminal.h"

namespace tsunami{
namespace core{

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

void Terminal::isRef() {
    return isRef_;
}



}
}
