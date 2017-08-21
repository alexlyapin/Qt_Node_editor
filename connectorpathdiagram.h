#ifndef CONNECTORPATHDIAGRAM_H
#define CONNECTORPATHDIAGRAM_H

#include "connectordiagram.h"

class ConnectorPathDiagram : public QGraphicsPathItem
{
public:
    ConnectorPathDiagram(QPointF);
    void getNewPoint(QPointF p);
protected:
    void updatePath(QPointF p1, QPointF p2);
    QPointF pStart;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget = 0)Q_DECL_OVERRIDE;
};

#endif // CONNECTORPATHDIAGRAM_H
