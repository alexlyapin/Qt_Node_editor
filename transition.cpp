#include "transition.h"
#include <QDebug>
// Constructors
Transition::Transition(State *S, State *T, const double W) : Positionable() {

    Source = S;
    Target = T;
    Weight = W;
    Joints = QList<Joint *>();
}

// Destructor
Transition::~Transition() {

    for (int i = 0; i < Joints.size(); ++i)
        delete Joints[i];
}

// Getters
State *Transition::source() { return Source; }

State *Transition::target() { return Target; }

double Transition::weight() const { return Weight; }

int Transition::numberOfJoints() const { return Joints.size(); }

Joint *Transition::joint(const int Index) { return Joints[Index]; }

// Modifiers
void Transition::setWeight(const double W) {

    if (Weight != W) {
        Weight = W;
        emit weightChanged();
    }
}

Joint *Transition::insertJoint(const int Index, const QPoint P) {

    if ((Index >= 0) && (Index <= Joints.size())) {
        Joint *New = new Joint(P);
        Joints.insert(Index, New);
        emit jointsChanged(this);
        return New;
    }

    return NULL;
}

void Transition::removeJoint(Joint *J) {

    Joints.removeOne(J);
    delete J;
    emit jointsChanged(this);
}
