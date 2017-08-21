#include "connectorpathdiagram.h"
#include <QPainterPath>
#include <QtDebug>
#include <QPen>
#include <QPainter>
#include <QColor>

#include "bezierpath.h"

ConnectorPathDiagram::ConnectorPathDiagram(QPointF P) : QGraphicsPathItem()
{
    pStart = P;
    setPen(QPen(QColor(51,208,129), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void ConnectorPathDiagram::updatePath(QPointF p1, QPointF p2){
    BezierPath bezierPath(p1,p2,nullptr,nullptr);
    setPath(bezierPath.getPath());
}

void ConnectorPathDiagram::getNewPoint(QPointF p){
    updatePath(pStart, p);
}

void ConnectorPathDiagram::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsPathItem::paint(painter, option, widget);
}
