#ifndef STATEEDITOR_H
#define STATEEDITOR_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "state.h"

// Class Declaration
class StateEditor : public QDialog {

    Q_OBJECT

public:
    // Constructor
    StateEditor(QWidget *, State *);

    // Getters
    const QString name();
    bool initial();

private:
    // Attributes
    QLineEdit *Name;
    QCheckBox *Initial;
    QDialogButtonBox *Buttons;
};
#endif // STATEEDITOR_H
