#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include "stateeditor.h"
#include "machine.h"

// Constructor
StateEditor::StateEditor(QWidget *Parent, State *Specification) : QDialog(Parent) {

    Name = new QLineEdit(Specification->name(), this);
    Buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(Buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QFormLayout *NameLayout = new QFormLayout;
    NameLayout->addRow(tr("State Name:"), Name);

    Initial = new QCheckBox(tr("Make Initial"), this);
    Initial->setChecked(false);
    if (Specification->machine()->initial() == Specification)
        Initial->setVisible(false);

    QVBoxLayout *Layout = new QVBoxLayout;
    Layout->addLayout(NameLayout);
    Layout->addWidget(Initial);
    Layout->addWidget(Buttons);
    setLayout(Layout);
    setWindowTitle(tr("Edit State"));
}

// Getters
const QString StateEditor::name() { return Name->text(); }

bool StateEditor::initial() { return Initial->isChecked(); }
