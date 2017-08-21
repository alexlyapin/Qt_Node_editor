#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include "weighteditor.h"

// Constructor
WeightEditor::WeightEditor(QWidget *Parent, Transition *T) : QDialog(Parent) {

    Weight = new QLineEdit(QString::number(T->weight()), this);

    QLabel *Label = new QLabel(tr("Weight"), this);
    QHBoxLayout *Top = new QHBoxLayout;
    Top->addWidget(Label);
    Top->addWidget(Weight);

    Buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(Buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(Buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *Layout = new QVBoxLayout;
    Layout->addLayout(Top);
    Layout->addWidget(Buttons);
    setLayout(Layout);

    setWindowTitle(tr("Edit Weight"));
}

// Getters
double WeightEditor::weight() const { return Weight->text().toDouble(); }
