#ifndef CONNECTORDIAGRAM_H
#define CONNECTORDIAGRAM_H

#include <QObject>
#include "statediagram.h"
#include "connectortype.h"
#include <QColor>

class ConnectorDiagram : public QObject, public QGraphicsPathItem
{

    Q_OBJECT

public:
    // Constructor
    ConnectorDiagram(StateDiagram *, int);
    StateDiagram *Specification;



    int getType() const;
    void setType(int value);

private slots:
    void rePaint();
    void onStateRenamed();
private:
    QColor brushColor, penColor;
    int type;

protected:
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget = 0)Q_DECL_OVERRIDE;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
};
#endif // CONNECTORDIAGRAM_H
