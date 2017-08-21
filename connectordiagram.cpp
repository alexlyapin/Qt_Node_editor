#include "connectordiagram.h"
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QtDebug>
#include <QColor>
#include <QPainterPath>

ConnectorDiagram::ConnectorDiagram(StateDiagram *S, int t): QObject(), QGraphicsPathItem()
{
    Specification = S;
    setType(t);

    connect(S->state(), SIGNAL(repositioned()), this, SLOT(rePaint()));

    brushColor = Qt::white;
    penColor = Qt::gray;
    setBrush(QBrush(brushColor, Qt::SolidPattern));
    setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    rePaint();


    setAcceptHoverEvents(true);
}



void ConnectorDiagram::rePaint()
{
    QPointF leftTop;
    QPointF rightBottom;
    if(getType() == ConnectorType::input){
        leftTop = QPointF(Specification->sceneBoundingRect().x()-10,
                        Specification->sceneBoundingRect().y()+Specification->boundingRect().height()*0.5-10);
        rightBottom = QPointF(Specification->sceneBoundingRect().x(),
                            Specification->sceneBoundingRect().y()+Specification->boundingRect().height()*0.5+10);
    }else{
        leftTop = QPointF(Specification->sceneBoundingRect().x()+Specification->boundingRect().width(),
                        Specification->sceneBoundingRect().y()+Specification->boundingRect().height()*0.5-10);
        rightBottom = QPointF(Specification->sceneBoundingRect().x()+10+Specification->boundingRect().width(),
                            Specification->sceneBoundingRect().y()+Specification->boundingRect().height()*0.5+10);
    }

    QPainterPath path(leftTop);

    path.lineTo(leftTop+QPointF(0,rightBottom.y()-leftTop.y()));
    path.lineTo(rightBottom);
    path.lineTo(rightBottom-QPointF(0,rightBottom.y()-leftTop.y()));
    path.lineTo(leftTop);
    setPath(path);
}

void ConnectorDiagram::onStateRenamed()
{
    rePaint();
}

int ConnectorDiagram::getType() const
{
    return type;
}

void ConnectorDiagram::setType(int value)
{
    type = value;
}

void ConnectorDiagram::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsPathItem::paint(painter, option, widget);
}

void ConnectorDiagram::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    penColor = QColor(130,182,228);

    setBrush(QBrush(brushColor, Qt::SolidPattern));
    setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    rePaint();
}

void ConnectorDiagram::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    penColor = Qt::gray;

    setBrush(QBrush(brushColor, Qt::SolidPattern));
    setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    rePaint();
}


