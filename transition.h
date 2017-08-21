#ifndef TRANSITION_H
#define TRANSITION_H

#include "joint.h"

// Forward Declaration
class State;

// Class Declaration
class Transition : public Positionable {

    Q_OBJECT

signals:
    // Notifiers
    void weightChanged();
    void jointsChanged(Transition *);

public:
    // Constructor
    explicit Transition(State *Source, State *Target, const double Weight = 1.0);

    // Destructor
    ~Transition();

    // Getters
    State *source();
    State *target();
    double weight() const;
    int numberOfJoints() const;
    Joint *joint(const int);

    // Modifiers
    void setWeight(const double);
    Joint *insertJoint(const int, const QPoint);
    void removeJoint(Joint *);

private:
    // Attributes
    State *Source, *Target;
    double Weight;
    QList<Joint *> Joints;
};

#endif // TRANSITION_H
