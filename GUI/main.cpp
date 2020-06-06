#include "mainwindow.h"
#include "core/Utils.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("UFJF - Machine Learning Toolkit");
    w.show();

    return a.exec();
}
