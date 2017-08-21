#ifndef MACHINEVIEW_H
#define MACHINEVIEW_H

#include <QGraphicsView>
#include <QContextMenuEvent>
#include "machinescene.h"
#include "connectordiagram.h"
#include <QRubberBand>

// Class Declaration
class MachineView : public QGraphicsView {

    Q_OBJECT

signals:
    // Notifications
    void newState(QPoint);
    void startDrawConnector(QPointF);
    void drawConnectorTo(QPointF);
    void removeConnector();
public:
    // Constructor
    explicit MachineView(QGraphicsScene *, QWidget *);




    bool isReadyToTransit() const;
    void setReadyToTransit(bool value);

protected:

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    // Helpers
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *pWheelEvent);

    int stateNum;

    StateDiagram *firstState;
    StateDiagram *secondState;

    void createTransition(StateDiagram *s1, StateDiagram *s2);


private slots:
    // Menu
    void onNew();
    void onCreateTransition();

private:
    QPointF Position;
    QWidget *machineEditor;
    bool readyToTransit;
    int zoomLevel;

    bool drawConnection;
};

#endif // MACHINEVIEW_H
