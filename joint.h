#ifndef JOINT_H
#define JOINT_H

#include "positionable.h"

// Class Declaration
class Joint : public Positionable {

public:
    // Constructor
    explicit Joint(const QPoint = QPoint());
};


#endif // JOINT_H
