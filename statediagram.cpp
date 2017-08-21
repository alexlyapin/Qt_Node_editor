#include <QApplication>
#include <QFontMetricsF>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include "statediagram.h"
#include "machine.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>

// Constructor
StateDiagram::StateDiagram(State *S) : QGraphicsObject() {

    Specification = S;
    connect(Specification, SIGNAL(renamed()), this, SLOT(onDimension()));
    connect(Specification, SIGNAL(repositioned()), this, SLOT(onRepositioned()));
    connect(Specification->machine(), SIGNAL(initialChanged(State*,State*)), this, SLOT(onInitialChanged(State*,State*)));

    onDimension();
    setPos(Specification->position());
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(5);
    shadow->setXOffset(-2);
    shadow->setYOffset(2);
    shadow->setColor(Qt::gray);
    setGraphicsEffect(shadow);

    color = Qt::gray;
}

// Slots
void StateDiagram::onDimension() {

    prepareGeometryChange();

    QFontMetricsF Metrics = QFontMetricsF(qApp->font());
    Name = Metrics.boundingRect(QString(Specification->name()));
    Name.moveCenter(QPointF(0, 0.5 * Name.height()));
    Name.adjust(-1, -1, 1, 1);

    update();
}

void StateDiagram::onRepositioned() { setPos(Specification->position()); }

void StateDiagram::onInitialChanged(State *Old, State *New) {

    if ((Specification == Old) || (Specification == New))
        update();
}

void StateDiagram::onEdit() { emit editState(Specification); }

void StateDiagram::onInitial() { emit changeInitial(Specification); }

void StateDiagram::onDelete() { Specification->machine()->removeState(Specification); }

ConnectorDiagram *StateDiagram::getOutputDiagram() const
{
    return outputDiagram;
}

void StateDiagram::setOutputDiagram(ConnectorDiagram *value)
{
    outputDiagram = value;
}

ConnectorDiagram *StateDiagram::getInputDiagram() const
{
    return inputDiagram;
}

void StateDiagram::setInputDiagram(ConnectorDiagram *value)
{
    inputDiagram = value;
}

// Getters
State *StateDiagram::state() { return Specification; }

// Helpers
QRectF StateDiagram::boundingRect() const { return Name.adjusted(-30, -10, 30, 10); }

void StateDiagram::paint(QPainter *Painter, const QStyleOptionGraphicsItem *, QWidget *w) {

    Painter->save();
    Painter->setRenderHint(QPainter::Antialiasing);
    Painter->setBrush(QBrush(QColor(190,255,241,60), Qt::SolidPattern));

    if (Specification->machine()->initial() == Specification) {
        Painter->setBrush(QBrush(QColor(133,255,176,100), Qt::SolidPattern));
    }

    Painter->setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //Painter->drawRect(boundingRect());
    Painter->drawRoundRect(boundingRect(), 1200 / int(boundingRect().width()), 1200 / int(boundingRect().height()));
//    if (Specification->machine()->initial() == Specification) {
//        QRectF Inner = boundingRect().adjusted(4, 4, -4, -4);
//        Painter->drawRoundRect(Inner, 1200 / int(Inner.width()), 1200 / int(Inner.height()));
//    }

    Painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    Painter->drawText(Name, Qt::AlignCenter, QString(Specification->name()));
    Painter->restore();

}

QVariant StateDiagram::itemChange(GraphicsItemChange Change, const QVariant &Value) {

    if (Change == ItemPositionHasChanged)
        Specification->setPosition(pos().toPoint());
    return Value;
}

void StateDiagram::contextMenuEvent(QGraphicsSceneContextMenuEvent *Event) {

    QMenu *Menu = new QMenu;
    QAction *Edit = new QAction(tr("Edit"), this);
    connect(Edit, SIGNAL(triggered()), this, SLOT(onEdit()));
    QAction *Initial = new QAction(tr("Make Initial"), this);
    connect(Initial, SIGNAL(triggered()), this, SLOT(onInitial()));
    QAction *Delete = new QAction(tr("Delete"), this);
    connect(Delete, SIGNAL(triggered()), this, SLOT(onDelete()));
    Menu->addAction(Edit);
    if (Specification->machine()->initial() != Specification) {
        Menu->addAction(Initial);
        Menu->addAction(Delete);
    }
    Menu->popup(Event->screenPos());
}

void StateDiagram::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) { emit onEdit(); }

void StateDiagram::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    //color = Qt::blue;
}

void StateDiagram::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    //color = Qt::gray;
}


void StateDiagram::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton) {
        if(event->modifiers() == Qt::ShiftModifier) {
            qDebug() << "Custom item left clicked with shift key.";
            setFlag(ItemIsMovable,false);
        }
    }
    QGraphicsObject::mousePressEvent(event);
}

void StateDiagram::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setFlag(ItemIsMovable,true);
    QGraphicsObject::mouseReleaseEvent(event);
}
