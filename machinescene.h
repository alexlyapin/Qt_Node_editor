#ifndef MACHINESCENE_H
#define MACHINESCENE_H

#include <QGraphicsScene>
#include "machine.h"
#include "statediagram.h"
#include "jointdiagram.h"
#include "weightdiagram.h"
#include "transitiondiagram.h"
#include "connectordiagram.h"
#include "connectorpathdiagram.h"

// Class Declaration
class MachineScene : public QGraphicsScene {

    Q_OBJECT

signals:
    // Notifiers
    void editState(State *);
    void changeInitial(State *);
    void editWeight(Transition *);
    void startDrawConnector(QPointF);

public:
    // Constructors
    MachineScene(QObject *, Machine *);

    // Desttructor
    ~MachineScene();

private slots:
    // Modifiers
    void onAddedState(State *);
    void onStateToDelete(State *);

    void onTransitionToDelete(Transition *);
    void onJointsChanged(Transition *);

    void highlightTransition(Transition *, bool);
    void onStartDrawConnector(QPointF);
    void onDrawConnectorTo(QPointF p);
    void onRemoveConnector();

public slots:
    void onAddedTransition(Transition *);

protected:
    void drawBackground(QPainter *p, const QRectF &clip);
private:
    // Attributes
    Machine *Specification;
    QHash<State *, StateDiagram *> States;
    QHash<Joint *, JointDiagram *> Joints;
    QList<TransitionDiagram *> Transitions;
    QList<WeightDiagram *> Weights;
    ConnectorPathDiagram *connector;
    QImage* bg_image_;
};

#endif // MACHINESCENE_H
