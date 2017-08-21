#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include "machineeditor.h"
#include "stateeditor.h"
#include "weighteditor.h"

// Constructor
MachineEditor::MachineEditor(QWidget *Parent, Machine *S) : QWidget(Parent) {

    Specification = S;
    Scene = new MachineScene(this, Specification);
    connect(Scene, SIGNAL(editState(State*)), this, SLOT(onEditState(State*)));
    connect(Scene, SIGNAL(changeInitial(State*)), this, SLOT(onChangeInitial(State*)));
    connect(Scene, SIGNAL(editWeight(Transition*)), this, SLOT(onEditWeight(Transition*)));
    View = new MachineView(Scene, this);
    connect(View, SIGNAL(newState(QPoint)), this, SLOT(onNewState(QPoint)));

    connect(View, SIGNAL(startDrawConnector(QPointF)), Scene, SIGNAL(startDrawConnector(QPointF)));
    connect(View, SIGNAL(drawConnectorTo(QPointF)), Scene, SLOT(onDrawConnectorTo(QPointF)));
    connect(View, SIGNAL(removeConnector()), Scene, SLOT(onRemoveConnector()));


    QVBoxLayout *Layout = new QVBoxLayout;
    Layout->addWidget(View);
    setLayout(Layout);

}

// Slots
void MachineEditor::onNewState(QPoint Position) {

    State *New = Specification->addState(tr("State"));

    New->setPosition(Position);
    StateEditor *Dialog = new StateEditor(this, New);
    if (Dialog->exec()) {
        New->rename(QString(Dialog->name()));
        if (Dialog->initial())
            Specification->setInitial(New);
    } else
        Specification->removeState(New);
    delete Dialog;
}


void MachineEditor::onEditState(State *S) {

    StateEditor *Dialog = new StateEditor(this, S);
    if (Dialog->exec()) {
        S->rename(QString(Dialog->name()));
        if (Dialog->initial())
            onChangeInitial(S);
    }
    delete Dialog;
}

void MachineEditor::onChangeInitial(State *S) { Specification->setInitial(S); }

void MachineEditor::onEditWeight(Transition *T) {

    WeightEditor *Dialog = new WeightEditor(this, T);
    if (Dialog->exec())
        T->setWeight(Dialog->weight());
    delete Dialog;
}
