#ifndef WEIGHTEDITOR_H
#define WEIGHTEDITOR_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include "transition.h"

// Class Declaration
class WeightEditor : public QDialog {

    Q_OBJECT

public:
    // Constructor
    explicit WeightEditor(QWidget *, Transition *);

    // Getters
    double weight() const;

private:
    // Attributes
    QLineEdit *Weight;
    QDialogButtonBox *Buttons;
};

#endif // WEIGHTEDITOR_H
