#ifndef MACHINE_H
#define MACHINE_H

#include "state.h"

class Machine : public QObject {

    Q_OBJECT

signals:
    // Notifiers
    void initialChanged(State *Old, State *New);
    void addedState(State *);
    void stateToDelete(State *);


public:
    // Constructor
    explicit Machine();

    // Destructor
    ~Machine();

    // Getters
    int numberOfStates() const;
    State *state(const int);
    State *initial();

    // Setters
    State *addState(const QString &Name = "State");
    void removeState(State *);
    void setInitial(State *);

private:
    // Attributes
    State *Initial;
    QList<State *> States;
};

#endif // MACHINE_H
