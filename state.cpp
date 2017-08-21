#include "state.h"
#include "machine.h"
#include <QtDebug>

// Constructor
State::State(Machine *M, const QString &N) : Positionable() {

    Name = N;
    Owner = M;
    Transitions = QList<Transition *>();
}

// Destructor
State::~State() {

    for (int i = 0; i < Transitions.size(); ++i) {
        emit transitionToDelete(Transitions[i]);
        delete Transitions[i];
    }
}

// Getters
const QString &State::name() { return Name; }

Machine *State::machine() { return Owner; }

int State::numberOfTransitions() const { return Transitions.size(); }

Transition *State::transition(const int Index) { return Transitions[Index]; }

bool State::isConnectedWith(State *S)
{
    QMutableListIterator<Transition *> tIt(Transitions);
    while (tIt.hasNext()) {
        Transition *T = tIt.next();
        if (T->target() == S) {
            return true;
        }
    }

    return false;
}

// Modifiers
void State::rename(const QString &N) {

    if (N != Name) {
        Name = N;
        emit renamed();
    }
}

Transition *State::addTransition(State *Target, const double Weight) {
    Transition *T = new Transition(this, Target, Weight);
    Transitions.append(T);
    emit addedTransition(T);
    return T;
}

void State::removeTransition(State *Target) {

    for (int i = 0; i < Transitions.size(); )
        if (Transitions[i]->target() == Target) {
            Transition *T = Transitions[i];
            emit transitionToDelete(T);
            Transitions.removeAt(i);
            delete T;
        } else
            i++;
}
