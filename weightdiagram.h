#ifndef WEIGHTDIAGRAM_H
#define WEIGHTDIAGRAM_H

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneContextMenuEvent>
#include "transition.h"

// Class Declaration
class WeightDiagram : public QObject, public QGraphicsSimpleTextItem {

    Q_OBJECT

signals:
    // Notifiers
    void editWeight(Transition *);
    void weightDrag(Transition *, bool);

public:
    // Constructor
    WeightDiagram(Transition *);

    // Getters
    Transition *transition();

private slots:
    // Modifiers
    void onWeightChanged();

    // Menu
    void onWeight();

private:
    // Attributes
    Transition *Specification;

    // Helpers
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange, const QVariant &) Q_DECL_OVERRIDE;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // WEIGHTDIAGRAM_H
