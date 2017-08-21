#include <QMenu>
#include <QAction>
#include <QBrush>
#include "jointdiagram.h"
#include <QtDebug>

JointDiagram::JointDiagram(Transition *L, Joint *J) : QObject(), QGraphicsRectItem() {
    Connection = L;
    Specification = J;
    setPos(Specification->position());
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);

    setBrush(QBrush(Qt::black, Qt::SolidPattern));
    setRect(-3, -3, 6, 6);
}

// Slots
void JointDiagram::onWeight() { emit editWeight(Connection); }

void JointDiagram::onDelete() { Connection->removeJoint(Specification); }

// Getters
Transition *JointDiagram::transition() { return Connection; }

Joint *JointDiagram::joint() { return Specification; }

// Helpers
QVariant JointDiagram::itemChange(GraphicsItemChange Change, const QVariant &Value) {
    if (Change == ItemPositionHasChanged)
        Specification->setPosition(pos().toPoint());
    return Value;
}

void JointDiagram::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *Event)
{
    onDelete();
}

void JointDiagram::contextMenuEvent(QGraphicsSceneContextMenuEvent *Event) {
    QMenu *Menu = new QMenu;
    QAction *Weight = new QAction(tr("Edit Weight"), this);
    connect(Weight, SIGNAL(triggered()), this, SLOT(onWeight()));
    Menu->addAction(Weight);
    if ((Connection->numberOfJoints() > 1) || (Connection->source() != Connection->target())) {
        QAction *Delete = new QAction(tr("Delete Joint"), this);
        connect(Delete, SIGNAL(triggered()), this, SLOT(onDelete()));
        Menu->addAction(Delete);
    }
    Menu->popup(Event->screenPos());
}


void JointDiagram::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit jointDrag(Connection, true);
    QGraphicsRectItem::mousePressEvent(event);
}

void JointDiagram::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit jointDrag(Connection, false);
    QGraphicsRectItem::mouseReleaseEvent(event);
}
