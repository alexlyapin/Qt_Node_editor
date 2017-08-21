#ifndef STATEDIAGRAM_H
#define STATEDIAGRAM_H

#include <QGraphicsObject>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include "state.h"


class ConnectorDiagram;

// Class Declaration
class StateDiagram : public QGraphicsObject {

    Q_OBJECT

signals:
    // Notifiers
    void editState(State *);
    void changeInitial(State *);

public:
    // Constructor
    StateDiagram(State *);

    // Getters
    State* state();

    // Helpers
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;

    ConnectorDiagram *getInputDiagram() const;
    void setInputDiagram(ConnectorDiagram *value);

    ConnectorDiagram *getOutputDiagram() const;
    void setOutputDiagram(ConnectorDiagram *value);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private slots:
    // Modifiers
    void onRepositioned();
    void onDimension();
    void onInitialChanged(State *, State *);

    // Menu
    void onEdit();
    void onInitial();
    void onDelete();

private:
    // Attributes
    State *Specification;
    QRectF Name;
    QColor color;

    ConnectorDiagram *inputDiagram;
    ConnectorDiagram *outputDiagram;

    // Helpers
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange, const QVariant &) Q_DECL_OVERRIDE;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // STATEDIAGRAM_H
