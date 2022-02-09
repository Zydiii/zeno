#ifndef __ZENO_SUBGRAPH_SCENE_H__
#define __ZENO_SUBGRAPH_SCENE_H__

#include <QtWidgets>
#include <zenoui/render/ztfutil.h>
#include <zenoui/nodesys/nodesys_common.h>
#include <zenoui/model/modeldata.h>


class SubGraphModel;
class ZenoNode;
class ZenoFullLink;
class ZenoTempLink;
class NodeGridItem;

class ZenoSubGraphScene : public QGraphicsScene
{
	Q_OBJECT
public:
    ZenoSubGraphScene(QObject* parent = nullptr);
    ~ZenoSubGraphScene();
    void initModel(SubGraphModel* pModel);
    QPointF getSocketPos(bool bInput, const QString &nodeid, const QString &portName);
    void undo();
    void redo();
    void copy();
    void paste(QPointF pos);
    QRectF nodesBoundingRect() const;
    SubGraphModel* model() const;
    void select(const QString& id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event);

public slots:
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowsInserted(const QModelIndex& parent, int first, int last);
    void onLinkChanged(bool bAdd, const QString &outputId, const QString &outputPort, const QString &inputId, const QString &inputPort);
    void onViewTransformChanged(qreal factor);

private slots:
    void reload();
    void clearLayout();
    void onSocketPosInited(const QString& nodeid, const QString& sockName, bool bInput);

private:
    void updateLinkPos(ZenoNode *pNode, QPointF newPos);

    QRectF m_viewRect;
    NodeUtilParam m_nodeParams;
	SubGraphModel* m_subgraphModel;
    std::map<QString, ZenoNode*> m_nodes;
    std::map<EdgeInfo, ZenoFullLink*/*, cmpEdge*/> m_links;
    ZenoTempLink* m_tempLink;
};

#endif