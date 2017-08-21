#ifndef TRANSITIONDIAGRAM_H
#define TRANSITIONDIAGRAM_H

#include <QGraphicsObject>
#include <QGraphicsSceneContextMenuEvent>
#include "jointdiagram.h"
#include "statediagram.h"

// Class Declaration
class TransitionDiagram : public QGraphicsObject {

    Q_OBJECT

signals:
    // Notifiers
    void editWeight(Transition *);

public:
    // Constructors
    TransitionDiagram(Transition *, QObject *, QObject *);

    // Getters
    Transition *transition();

    bool isHighlighted() const;
    void setHighlighted(bool value);

private slots:
    // Modifiers
    void onRepositioned();

    // Menu
    void onNewJoint();
    void onWeight();
    void onDelete();

private:
    // Attributes
    Transition *Specification;
    StateDiagram *State1, *State2;
    JointDiagram *Joint1, *Joint2;
    QPointF Position;
    QPointF p1, p2;
    bool haveArrow;

    QPainterPath currentPath;
    QPainterPath shape() const;

    // Helpers
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *) Q_DECL_OVERRIDE;
    QRectF boundingRect() const;

    bool highlighted;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *Event);

};

#endif // TRANSITIONDIAGRAM_H
