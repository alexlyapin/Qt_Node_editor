#ifndef STATE_H
#define STATE_H

#include <QList>
#include "transition.h"

// Forward Declaration
class Machine;

// Class Declaration
class State : public Positionable {

    Q_OBJECT

signals:
    // Notifiers
    void renamed();
    void addedTransition(Transition *);
    void transitionToDelete(Transition *);

public:
    // Constructor
    explicit State(Machine *, const QString &Name = tr("State"));

    // Destructor
    ~State();

    // Getters
    const QString &name();
    Machine *machine();
    int numberOfTransitions() const;
    Transition *transition(const int);
    bool isConnectedWith(State *);

    // Modifiers
    void rename(const QString &);
    Transition *addTransition(State *Target, const double Weight = 1.0);
    void removeTransition(State *Target);

private:
    // Attributes
    QString Name;
    Machine *Owner;
    QList<Transition *> Transitions;
};

#endif // STATE_H
