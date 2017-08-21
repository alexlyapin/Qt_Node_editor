#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#include <QObject>
#include <QPoint>

// Class Declaration
class Positionable : public QObject {

    Q_OBJECT

signals:
    // Notifiers
    void repositioned();

public:
    // Destructor
    virtual ~Positionable();

    // Getters
    QPoint position() const;

    // Modifiers
    void setPosition(const QPoint);

protected:
    // Constructor
    explicit Positionable(const QPoint = QPoint());

    // Attributes
    QPoint Position;
};

#endif // POSITIONABLE_H
