#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "defines.h"
namespace tsunami{
namespace spice{

class GraphNode {
    friend class Circuit;
public:
    GraphNode(const QString& name);
    void link( GraphNode* node );
    void unlink( GraphNode* node);

    QVector<GraphNode*> getNodes();
    GraphNode* getNode(int i);
    int countConnectedNodes();

    const QString& name() const;
    const int& id() const;


private:
    QString name_;
    QVector<GraphNode*> nodes_;
    static int nextId_;
    int id_;
};


}
}
#endif // GRAPHNODE_H
