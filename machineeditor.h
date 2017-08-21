#ifndef MACHINEEDITOR_H
#define MACHINEEDITOR_H

#include <QWidget>
#include "machinescene.h"
#include "machineview.h"

// Constructor
class MachineEditor : public QWidget {

    Q_OBJECT

public:
    // Constructor
    explicit MachineEditor(QWidget *, Machine *);

private slots:
    // Menu
    void onNewState(QPoint);
    void onEditState(State *);
    void onChangeInitial(State *);
    void onEditWeight(Transition *);

private:
    // Attributes
    Machine *Specification;
    MachineScene *Scene;
    MachineView *View;
};

#endif // MACHINEEDITOR_H
