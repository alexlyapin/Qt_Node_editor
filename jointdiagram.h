#ifndef JOINTDIAGRAM_H
#define JOINTDIAGRAM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include "transition.h"

// Class Declaration
class JointDiagram : public QObject, public QGraphicsRectItem {

    Q_OBJECT

signals:
    // Notifiers
    void editWeight(Transition *);
    void jointDrag(Transition *, bool);

public:
    // Constructor
    JointDiagram(Transition *, Joint *);

    // Getters
    Transition *transition();
    Joint *joint();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    // Menu
    void onWeight();
    void onDelete();

private:
    // Attributes
    Transition *Connection;
    Joint *Specification;

    // Helpers
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange, const QVariant &) Q_DECL_OVERRIDE;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *Event);
};

#endif // JOINTDIAGRAM_H
