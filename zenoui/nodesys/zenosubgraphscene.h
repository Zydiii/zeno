#ifndef __ZENO_SUBGRAPH_SCENE_H__
#define __ZENO_SUBGRAPH_SCENE_H__

#include <QtWidgets>
#include "../render/ztfutil.h"
#include "nodesys_common.h"

class SubGraphModel;
class ZenoNode;
class ZenoLinkFull;

class ZenoSubGraphScene : public QGraphicsScene
{
	Q_OBJECT
public:
    ZenoSubGraphScene(QObject* parent = nullptr);
    void initModel(SubGraphModel* pModel);
    QPointF getSocketPos(bool bInput, const QString &nodeid, const QString &portName);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event);

public slots:
    void onNewNodeCreated();    //todo: category.
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onLinkChanged(bool bAdd, const QString &outputId, const QString &outputPort, const QString &inputId, const QString &inputPort);

private:
    void updateNodePos(ZenoNode *pNode, QPointF newPos);

    NodeUtilParam m_nodeParams;
	SubGraphModel* m_subgraphModel;
    std::map<QString, ZenoNode*> m_nodes;
    std::map<EdgeInfo, ZenoLinkFull*/*, cmpEdge*/> m_links;
};

#endif