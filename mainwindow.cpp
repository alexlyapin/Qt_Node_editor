#include "mainwindow.h"
#include "machineeditor.h"

// Constructor
MainWindow::MainWindow(QWidget *Parent) : QMainWindow(Parent) {

    Machine *Specification = new Machine();
    MachineEditor *Editor = new MachineEditor(this, Specification);

    setWindowTitle(tr("Challenge"));
    setCentralWidget(Editor);
}
