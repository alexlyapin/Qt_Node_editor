#include <QMenu>
#include <QAction>
#include "machineview.h"
#include <QTimeLine>
#include <QDebug>
#include <QScrollBar>
#include <QtMath>

#include <connectordiagram.h>
#include <connectortype.h>

// Constructor
MachineView::MachineView(QGraphicsScene *Scene, QWidget *Parent) : QGraphicsView(Scene, Parent) {
    setTransformationAnchor(AnchorUnderMouse);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    this->machineEditor = Parent;
    stateNum=0;
    zoomLevel=0;

    drawConnection = false;
}

// Slots
void MachineView::onNew() { emit newState(QPoint(Position.x(), Position.y())); }

void MachineView::onCreateTransition(){
    setReadyToTransit(true);
}

// Helpers
void MachineView::contextMenuEvent(QContextMenuEvent *Event) {

    Position = Event->pos();
    QGraphicsItem* Item = itemAt(Position.x(), Position.y());
    if (Item != NULL) {
        StateDiagram* S = dynamic_cast<StateDiagram *>(Item);
        JointDiagram* J = dynamic_cast<JointDiagram *>(Item);
        TransitionDiagram* T = dynamic_cast<TransitionDiagram *>(Item);
        WeightDiagram* W = dynamic_cast<WeightDiagram *>(Item);
        if (S || J || T || W) {
            QGraphicsView::contextMenuEvent(Event);
            return;
        }
    }
    Position = mapToScene(Event->pos());

    QMenu *Menu = new QMenu;
    QAction *New = new QAction(tr("New State"), this);
    connect(New, SIGNAL(triggered()), this, SLOT(onNew()));
    Menu->addAction(New);

    Menu->popup(Event->globalPos());
}



bool MachineView::isReadyToTransit() const
{
    return readyToTransit;
}

void MachineView::setReadyToTransit(bool value)
{
    readyToTransit = value;
}

void MachineView::createTransition(StateDiagram *s1, StateDiagram *s2)
{
    if(!s1->state()->isConnectedWith(s2->state()) &&
            !s2->state()->isConnectedWith(s1->state()))
        s1->state()->addTransition(s2->state());
}

void MachineView::wheelEvent ( QWheelEvent * pWheelEvent )
{

    if (pWheelEvent->modifiers() == Qt::ControlModifier) {
        double scaleFactor=1.15;
        if (pWheelEvent->delta() > 0){
            if(zoomLevel<3){
                this->scale(scaleFactor,scaleFactor);
                zoomLevel++;
            }
        }else{
            if(zoomLevel>-5){
                this->scale(1/scaleFactor,1/scaleFactor);
                zoomLevel--;
            }
        }
    }
}

void MachineView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ShiftModifier){
        if (dynamic_cast<StateDiagram *>(this->itemAt(event->pos()))){
            firstState = static_cast<StateDiagram *>(this->itemAt(event->pos()));
            drawConnection = true;
            emit startDrawConnector(mapToScene(event->pos()));
        }
    }

    if (event->button() == Qt::MiddleButton && event->modifiers() == Qt::ControlModifier)
    {
        setDragMode(ScrollHandDrag);
        QMouseEvent fake(event->type(),
                         event->pos(),
                         Qt::LeftButton,
                         Qt::LeftButton,
                         event->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    }
    else QGraphicsView::mousePressEvent(event);
}

void MachineView::mouseMoveEvent(QMouseEvent *event) {

    if (drawConnection){
        emit drawConnectorTo(mapToScene(event->pos()));
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MachineView::mouseReleaseEvent(QMouseEvent *event) {

    if (drawConnection){
        emit removeConnector();
        if (dynamic_cast<StateDiagram *>(this->itemAt(event->pos()))){
            secondState = static_cast<StateDiagram *>(this->itemAt(event->pos()));
            if (firstState!=secondState) createTransition(firstState, secondState);
        }
        drawConnection = false;
    }


    if (event->button() == Qt::MiddleButton)
    {
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(),
                         event->pos(),
                         Qt::LeftButton,
                         Qt::LeftButton,
                         event->modifiers());
       QGraphicsView::mouseReleaseEvent(&fake);
    }
    else QGraphicsView::mouseReleaseEvent(event);
}


