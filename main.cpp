#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication Application(argc, argv);
    MainWindow Main;
    Main.show();
    return Application.exec();
}
