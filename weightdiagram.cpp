#include <QMenu>
#include <QAction>
#include "weightdiagram.h"

// Constructor
WeightDiagram::WeightDiagram(Transition *T) : QObject(), QGraphicsSimpleTextItem() {

    Specification = T;
    connect(Specification, SIGNAL(weightChanged()), this, SLOT(onWeightChanged()));
    setPos(Specification->position());
    onWeightChanged();
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
}

// Slots
void WeightDiagram::onWeightChanged() { setText(QString::number(Specification->weight())); }

void WeightDiagram::onWeight() { emit editWeight(Specification); }

// Getters
Transition *WeightDiagram::transition() { return Specification; }

// Helpers
QVariant WeightDiagram::itemChange(GraphicsItemChange Change, const QVariant &Value) {

    if (Change == ItemPositionHasChanged)
        Specification->setPosition(pos().toPoint());

    return Value;
}

void WeightDiagram::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit weightDrag(Specification, true);
    QGraphicsSimpleTextItem::mousePressEvent(event);
}

void WeightDiagram::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit weightDrag(Specification, false);
    QGraphicsSimpleTextItem::mouseReleaseEvent(event);
}

void WeightDiagram::contextMenuEvent(QGraphicsSceneContextMenuEvent *Event) {

    QAction *Weight = new QAction(tr("Edit Weight"), this);
    connect(Weight, SIGNAL(triggered()), this, SLOT(onWeight()));
    QMenu *Menu = new QMenu;
    Menu->addAction(Weight);
    Menu->popup(Event->screenPos());
}

void WeightDiagram::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) { onWeight(); }
