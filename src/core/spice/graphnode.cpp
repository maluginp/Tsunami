#include "graphnode.h"

namespace tsunami{
namespace core{

int GraphNode::nextId_ = 1;

GraphNode::GraphNode(const QString &name)
    : name_(name)
    , id_(nextId_) {

    nextId_++;
    nodes_.reserve(6);
}

void GraphNode::link(GraphNode *node) {
    nodes_.append( node );
}

void GraphNode::unlink(GraphNode *node) {
    int countNodes = nodes_.size();
    for(int i=0; i < countNodes; ++i){
        if(nodes_[i] == node){
            nodes_.remove(i);
            break;
        }
    }

}

QVector<GraphNode *> GraphNode::getNodes() {
    return nodes_;
}

GraphNode *GraphNode::getNode(int i) const {
    Q_ASSERT( i>=0 && i < countConnectedNodes() );
    return nodes_[i];
}

int GraphNode::countConnectedNodes() {
    return nodes_.size();
}

const QString &GraphNode::name() const {
    return name_;
}

const int &GraphNode::id() const {
    return id_;
}




}
}
