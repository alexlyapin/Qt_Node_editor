#include "machinescene.h"
#include <QDebug>
#include "connectortype.h"
#include "connectordiagram.h"
#include "connectorpathdiagram.h"
#include <QPainter>

// Constructor
MachineScene::MachineScene(QObject *Parent, Machine *M) : QGraphicsScene(Parent) {
    setSceneRect(-1000, -1000, 2000, 2000);

    bg_image_ = new QImage("d:/1.jpg");

    Specification = M;
    connect(Specification, SIGNAL(addedState(State*)), this, SLOT(onAddedState(State*)));
    connect(Specification, SIGNAL(stateToDelete(State*)), this, SLOT(onStateToDelete(State*)), Qt::DirectConnection);   

    States = QHash<State *, StateDiagram *>();
    Joints = QHash<Joint *, JointDiagram *>();
    Transitions = QList<TransitionDiagram *>();
    Weights = QList<WeightDiagram *>();

    for (int i = 0; i < Specification->numberOfStates(); ++i)
        onAddedState(Specification->state(i));

    for (int i = 0; i < Specification->numberOfStates(); ++i)
        for (int j = 0; j < Specification->state(i)->numberOfTransitions(); ++j)
            onAddedTransition(Specification->state(i)->transition(j));

    connect(this,SIGNAL(startDrawConnector(QPointF)),this,SLOT(onStartDrawConnector(QPointF)));

    /*
     * Text hints
     *
     *
     * */

    QGraphicsTextItem *text = new QGraphicsTextItem("To draw connection hold SHIFT, press left button and drag cursor from one state to another; The scene-zoom and scene-drag could be done with holding CTRL and mouse wheel pressing (rotation and drag)");

    text->setPos(-450,-250);
    text->setTextWidth(500);
    text->setFont(QFont("Times",12,QFont::Normal));
    this->addItem(text);
}

// Destructor
MachineScene::~MachineScene() {

    for (int i = 0; i < Weights.size(); ++i)
        delete Weights[i];
    for (int i = 0; i < Transitions.size(); ++i)
        delete Transitions[i];
    foreach (Joint *J, Joints.keys())
        delete Joints.value(J);
    foreach (State *S, States.keys())
        delete States.value(S);
}

// Slots
void MachineScene::onAddedState(State *S) {


    connect(S, SIGNAL(addedTransition(Transition*)), this, SLOT(onAddedTransition(Transition*)));
    connect(S, SIGNAL(transitionToDelete(Transition*)), this, SLOT(onTransitionToDelete(Transition*)), Qt::DirectConnection);
    StateDiagram *Diagram = new StateDiagram(S);

    connect(Diagram, SIGNAL(editState(State*)), this, SIGNAL(editState(State*)));
    connect(Diagram, SIGNAL(changeInitial(State*)), this, SIGNAL(changeInitial(State*)));
    States.insert(S, Diagram);
    addItem(Diagram);

/*


  Connectors


*/
//    ConnectorDiagram *InputConnector = new ConnectorDiagram(Diagram, ConnectorType::input);
//    addItem(InputConnector);
//    Diagram->setInputDiagram(InputConnector);
//    connect(S,SIGNAL(renamed()),InputConnector, SLOT(onStateRenamed()));

//    ConnectorDiagram *OutputConnector = new ConnectorDiagram(Diagram, ConnectorType::output);
//    addItem(OutputConnector);
//    Diagram->setOutputDiagram(OutputConnector);
//    connect(S,SIGNAL(renamed()),OutputConnector, SLOT(onStateRenamed()));
}

void MachineScene::onStateToDelete(State *S) {
    StateDiagram *Diagram = States.value(S);
    removeItem(Diagram);

    ConnectorDiagram *inputDiagram = Diagram->getInputDiagram();
    ConnectorDiagram *outputDiagram = Diagram->getOutputDiagram();
    removeItem(inputDiagram);
    removeItem(outputDiagram);

    delete inputDiagram;
    delete outputDiagram;

    States.remove(S);
    delete Diagram;
    update();
}

void MachineScene::onAddedTransition(Transition *T) {

    qDebug()<<"on added; before";
    qDebug()<<"Number of Transitions --- "<<Transitions.length();
    qDebug()<<"Number of Joints --- "<<Joints.size();
    qDebug()<<"Number of Weights --- "<<Weights.length();
    qDebug()<<"Number of States --- "<<States.size();

    connect(T, SIGNAL(jointsChanged(Transition*)), this, SLOT(onJointsChanged(Transition*)));



    QPointF weightDiagramPos;

    if (T->numberOfJoints() == 0) {
        TransitionDiagram *Diagram = new TransitionDiagram(T, States.value(T->source()), States.value(T->target()));
        connect(Diagram, SIGNAL(editWeight(Transition*)), this, SIGNAL(editWeight(Transition*)));

        connect(States.value(T->source())->state(), SIGNAL(renamed()), Diagram, SLOT(onRepositioned()));
        connect(States.value(T->target())->state(), SIGNAL(renamed()), Diagram, SLOT(onRepositioned()));

        Diagram->setZValue(-100);
        Transitions.append(Diagram);
        addItem(Diagram);
        weightDiagramPos = Diagram->sceneBoundingRect().center();
    }

    for (int i = 0; i < T->numberOfJoints(); ++i) {
        JointDiagram *Diagram = new JointDiagram(T, T->joint(i));
        connect(Diagram, SIGNAL(editWeight(Transition*)), this, SIGNAL(editWeight(Transition*)));
        connect(Diagram, SIGNAL(jointDrag(Transition*, bool)), this, SLOT(highlightTransition(Transition*, bool)));

        Diagram->setZValue(-50);
        Joints.insert(T->joint(i), Diagram);
        addItem(Diagram);

        if (i == 0) {
            TransitionDiagram *First = new TransitionDiagram(T, States.value(T->source()), Diagram);
            connect(First, SIGNAL(editWeight(Transition*)), this, SIGNAL(editWeight(Transition*)));

            connect(States.value(T->source())->state(), SIGNAL(renamed()), First, SLOT(onRepositioned()));

            First->setZValue(-100);
            Transitions.append(First);
            addItem(First);
            weightDiagramPos = First->sceneBoundingRect().center()+QPointF(10,10);

        } else {
            TransitionDiagram *Intermediate = new TransitionDiagram(T, Joints.value(T->joint(i - 1)), Diagram);
            connect(Intermediate, SIGNAL(editWeight(Transition*)), this, SIGNAL(editWeight(Transition*)));
            Intermediate->setZValue(-100);
            Transitions.append(Intermediate);
            addItem(Intermediate);
        }

        if (i == T->numberOfJoints() - 1) {
            TransitionDiagram *Last = new TransitionDiagram(T, Diagram, States.value(T->target()));
            connect(Last, SIGNAL(editWeight(Transition*)), this, SIGNAL(editWeight(Transition*)));

            connect(States.value(T->target())->state(), SIGNAL(renamed()), Last, SLOT(onRepositioned()));

            Last->setZValue(-100);
            Transitions.append(Last);
            addItem(Last);
        }
    }

    WeightDiagram *Diagram = new WeightDiagram(T);
    connect(Diagram, SIGNAL(editWeight(Transition*)), this, SIGNAL(editWeight(Transition*)));
    connect(Diagram, SIGNAL(weightDrag(Transition*,bool)), this, SLOT(highlightTransition(Transition*, bool)));
    Diagram->setZValue(-50);
    Weights.append(Diagram);
    addItem(Diagram);
    Diagram->setPos(weightDiagramPos);

    qDebug()<<"on added; after";
    qDebug()<<"Number of Transitions --- "<<Transitions.length();
    qDebug()<<"Number of Joints --- "<<Joints.size();
    qDebug()<<"Number of Weights --- "<<Weights.length();
    qDebug()<<"Number of States --- "<<States.size();
}


void MachineScene::onTransitionToDelete(Transition *T) {

    //    QHash<Joint *, JointDiagram *> Joints;
    QMutableHashIterator<Joint *, JointDiagram *> jIt(Joints);
    while (jIt.hasNext()) {
         JointDiagram *Diagram = jIt.next().value();
         if (Diagram->transition() == T) {
             removeItem(Diagram);
             Diagram->deleteLater();
             jIt.remove();
         }
     }


//    foreach (Joint *J, Joints.keys())
//        if (Joints.value(J)->transition() == T) {
//            JointDiagram *Diagram = Joints.value(J);
//            removeItem(Diagram);
//            Joints.remove(J);
//            delete Diagram;
//        }

    //    QList<TransitionDiagram *> Transitions;

    QMutableListIterator<TransitionDiagram *> tIt(Transitions);
    while (tIt.hasNext()) {
         TransitionDiagram *Diagram = tIt.next();
         if (Diagram->transition() == T) {
             removeItem(Diagram);
             Diagram->deleteLater();
             tIt.remove();
         }
     }


//    foreach (TransitionDiagram *Diagram, Transitions)
//        if (Diagram->transition() == T) {
//            removeItem(Diagram);
//            Transitions.removeOne(Diagram);
//            delete Diagram;
//        }


        //    QList<WeightDiagram *> Weights;


    QMutableListIterator<WeightDiagram *> wIt(Weights);
    while (wIt.hasNext()) {
         WeightDiagram *Diagram = wIt.next();
         if (Diagram->transition() == T) {
             removeItem(Diagram);
             Diagram->deleteLater();
             wIt.remove();
             break;
         }
     }

//    for (int i = 0; i < Weights.size(); ++i)
//        if (Weights[i]->transition() == T) {
//            WeightDiagram *Diagram = Weights[i];
//            removeItem(Diagram);
//            Weights.removeOne(Diagram);
//            delete Diagram;
//            break;
//        }

    update();

}

void MachineScene::onJointsChanged(Transition *T) {
    onTransitionToDelete(T);
    onAddedTransition(T);
}

void MachineScene::highlightTransition(Transition *T, bool b)
{
    QMutableListIterator<TransitionDiagram *> tIt(Transitions);
    while (tIt.hasNext()) {
         TransitionDiagram *Diagram = tIt.next();
         if (Diagram->transition() == T) {
             Diagram->setHighlighted(b);
         }
    }
}

void MachineScene::onStartDrawConnector(QPointF p)
{
    connector = new ConnectorPathDiagram(p);
    this->addItem(connector);
}

void MachineScene::onDrawConnectorTo(QPointF p)
{
    connector->getNewPoint(p);
}

void MachineScene::onRemoveConnector()
{
    this->removeItem(connector);
}

void MachineScene::drawBackground( QPainter* painter, const QRectF& rect )
{

    const int gridSize = 25;

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));


    painter->setBrush(QBrush(Qt::white));
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);
    painter->setPen(QPen(QColor(215,241,255), 0));
    painter->drawLines(lines.data(), lines.size());

}
