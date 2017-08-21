#include <QPainter>
#include <QMenu>
#include <QAction>
#include "transitiondiagram.h"
#include <QGraphicsDropShadowEffect>
#include <QtMath>
#include <QtDebug>
#include "bezierpath.h"

// Constructor
TransitionDiagram::TransitionDiagram(Transition *T, QObject *Object1, QObject* Object2) : QGraphicsObject() {

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(5);
    shadow->setXOffset(-2);
    shadow->setYOffset(2);
    shadow->setColor(Qt::gray);
    setGraphicsEffect(shadow);


    Specification = T;

    State1 = NULL;
    State2 = NULL;
    Joint1 = NULL;
    Joint2 = NULL;

    if (dynamic_cast<StateDiagram *>(Object1)) {
        State1 = static_cast<StateDiagram *>(Object1);
        connect(State1->state(), SIGNAL(repositioned()), this, SLOT(onRepositioned()));
    } else if (dynamic_cast<JointDiagram *>(Object1)) {
        Joint1 = static_cast<JointDiagram *>(Object1);
        connect(Joint1->joint(), SIGNAL(repositioned()), this, SLOT(onRepositioned()));
    }

    if (dynamic_cast<StateDiagram *>(Object2)) {
        State2 = static_cast<StateDiagram *>(Object2);
        connect(State2->state(), SIGNAL(repositioned()), this, SLOT(onRepositioned()));
    } else if (dynamic_cast<JointDiagram *>(Object2)) {
        Joint2 = static_cast<JointDiagram *>(Object2);
        connect(Joint2->joint(), SIGNAL(repositioned()), this, SLOT(onRepositioned()));
    }

    setHighlighted(false);
    haveArrow = false;

    onRepositioned();
}

// Slots
void TransitionDiagram::onRepositioned() {

    prepareGeometryChange();

    if ((State1 != NULL) && (State2 != NULL)){
        p1 = State1->sceneBoundingRect().center();
        p2 = State2->sceneBoundingRect().center();
        haveArrow = true;
    }
    if ((State1 != NULL) && (Joint2 != NULL)){
        p1 = State1->sceneBoundingRect().center();
        p2 = Joint2->pos();
    }
    if ((Joint1 != NULL) && (State2 != NULL)){
        p1 = Joint1->pos();
        p2 = State2->sceneBoundingRect().center();
        haveArrow = true;
    }
    if ((Joint1 != NULL) && (Joint2 != NULL)){
        p1 = Joint1->pos();
        p2 = Joint2->pos();
    }

}

void TransitionDiagram::onNewJoint() {

    if (State1 != NULL)
        Specification->insertJoint(0, Position.toPoint());
    else
        for (int i = 0; i < Specification->numberOfJoints(); ++i)
            if (Specification->joint(i) == Joint1->joint()) {
                Specification->insertJoint(i + 1, Position.toPoint());
                break;
            }
}

void TransitionDiagram::onWeight() { emit editWeight(Specification); }

void TransitionDiagram::onDelete() { Specification->source()->removeTransition(Specification->target()); }

QPainterPath TransitionDiagram::shape() const
{
    QPainterPathStroker stroker;
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    stroker.setWidth(5);
    QPainterPath outline = stroker.createStroke(currentPath);

    return outline;
}

// Getters
Transition *TransitionDiagram::transition() { return Specification; }

// Helpers
void TransitionDiagram::contextMenuEvent(QGraphicsSceneContextMenuEvent *Event) {

    Position = Event->scenePos();
    QAction *Add = new QAction(tr("Add Joint"), this);
    connect(Add, SIGNAL(triggered()), this, SLOT(onNewJoint()));
    QAction *Weight = new QAction(tr("Edit Weight"), this);
    connect(Weight, SIGNAL(triggered()), this, SLOT(onWeight()));
    QAction *Delete = new QAction(tr("Delete Transition"), this);
    connect(Delete, SIGNAL(triggered()), this, SLOT(onDelete()));
    QMenu *Menu = new QMenu;
    Menu->addAction(Add);
    Menu->addAction(Weight);
    Menu->addAction(Delete);
    Menu->popup(Event->screenPos());
}

void TransitionDiagram::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    BezierPath bezierPath(p1,p2,State1,State2);

    QColor color;
    if (isHighlighted()) color = Qt::red; else color=Qt::black;

    painter->setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(bezierPath.getPath());

    currentPath = bezierPath.getPath();

    if (haveArrow){
        QVector<QPointF> arrowPoints = bezierPath.getArrowPoints(15,5);

        QPainterPath arrow(arrowPoints[0]);
        arrow.lineTo(arrowPoints[1]);
        arrow.lineTo(arrowPoints[2]);
        arrow.lineTo(arrowPoints[0]);
        painter->setPen(QPen(Qt::darkCyan, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->fillPath (arrow, QBrush (QColor("#c0c0c0")));
        painter->drawPath(arrow);
    }
}


QRectF TransitionDiagram::boundingRect() const{
    QRectF rect(p1,p2);
    if(p1.x()<p2.x()){
        if(p1.y()<p2.y()){
            return QRectF(p1,p2).adjusted(-10,-10,10,10);
        }else{
            return QRectF(QPointF(p1.x(),p2.y()),QPointF(p2.x(),p1.y())).adjusted(-10,-10,10,10);
        }
    }else{
        if(p1.y()<p2.y()){
            return QRectF(QPointF(p2.x(),p1.y()),QPointF(p1.x(),p2.y())).adjusted(-10,-10,10,10);
        }else{
            return QRectF(p2,p1).adjusted(-10,-10,10,10);
        }
    }
}

bool TransitionDiagram::isHighlighted() const
{
    return highlighted;
}

void TransitionDiagram::setHighlighted(bool value)
{
    highlighted = value;
    update();
}

void TransitionDiagram::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *Event)
{
    Position = Event->scenePos();
    emit onNewJoint();
}


