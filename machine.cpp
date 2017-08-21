#include "machine.h"
#include "transition.h"

Machine::Machine() {

    States = QList<State *>();
    Initial = addState(tr("Initial"));
    State* newState = addState(tr("State 1"));

    newState->setPosition(QPoint(200,0));
    Initial->setPosition(QPoint(-200,0));
    Initial->addTransition(newState);
}

// Destructor
Machine::~Machine() {

    for (int i = 0; i < States.size(); ++i) {
        emit stateToDelete(States[i]);
        delete States[i];
    }
}

// Getters
int Machine::numberOfStates() const { return States.size(); }

State *Machine::state(const int Index) { return States[Index]; }

State *Machine::initial() { return Initial; }

// Modifiers
State *Machine::addState(const QString &N) {

    State *S = new State(this, N);
    States.append(S);
    emit addedState(S);
    return S;
}

void Machine::removeState(State *S) {

    if (States.size() > 1) {
        for (int i = 0; i < States.size(); ++i)
            States[i]->removeTransition(S);
        if (S != Initial) {
            emit stateToDelete(S);
            States.removeOne(S);
            delete S;
        }
    }
}

void Machine::setInitial(State *S) {

    if (Initial != S) {
        State *Old = Initial;
        Initial = S;
        emit initialChanged(Old, Initial);
    }
}
