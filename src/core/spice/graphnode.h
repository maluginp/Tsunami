#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QString>
#include <QVector>

namespace tsunami{
namespace core{

class GraphNode
{
public:
    GraphNode(const QString& name);
    void link( GraphNode* node );
    void unlink( GraphNode* node);

    QVector<GraphNode*> getNodes();
    GraphNode* getNode(int i) const;
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
