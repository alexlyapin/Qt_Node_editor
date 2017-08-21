#include "positionable.h"

// Constructor
Positionable::Positionable(const QPoint P) : QObject() { Position = P; }

// Destructor
Positionable::~Positionable() { }

// Getters
QPoint Positionable::position() const { return Position; }

// Modifiers
void Positionable::setPosition(const QPoint P) {

    if (Position != P) {
        Position = P;
        emit repositioned();
    }
}
