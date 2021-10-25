#ifndef QDMGRAPHICSBACKGROUND_H
#define QDMGRAPHICSBACKGROUND_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPointF>

class QDMGraphicsBackground : public QGraphicsItem
{
public:
    QDMGraphicsBackground();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, QStyleOptionGraphicsItem const *styleOptions, QWidget *widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // QDMGRAPHICSBACKGROUND_H