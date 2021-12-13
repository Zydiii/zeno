#ifndef __ZENO_LINK_H__
#define __ZENO_LINK_H__

#include <QtWidgets>
#include "nodesys_common.h"

class ZenoSubGraphScene;

class ZenoLink : public QGraphicsObject
{
    Q_OBJECT
    typedef QGraphicsObject _base;

public:
    ZenoLink(QGraphicsItem* parent = nullptr);
    virtual ~ZenoLink();

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, QStyleOptionGraphicsItem const *styleOptions, QWidget *widget) override;

    enum { Type = ZTYPE_LINK };
    int type() const override;

    virtual QPointF getSrcPos() const = 0;
    virtual QPointF getDstPos() const = 0;

private:
    static constexpr float BEZIER = 0.5f, WIDTH = 3;

    mutable QPointF lastSrcPos, lastDstPos;
    mutable bool hasLastPath{false};
    mutable QPainterPath lastPath;
};

class ZenoLinkFull : public ZenoLink
{
public:
    ZenoLinkFull(const EdgeInfo& info);

    virtual QPointF getSrcPos() const override;
    virtual QPointF getDstPos() const override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void updatePos(const QPointF& srcPos, const QPointF& dstPos);
    void updateLink(const EdgeInfo& info);
    EdgeInfo linkInfo() const;

    enum { Type = ZTYPE_FULLLINK };
    int type() const override;

private:
    EdgeInfo m_linkInfo;
    QPointF m_srcPos, m_dstPos;
};

#endif