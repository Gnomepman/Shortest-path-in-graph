#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Лабораторна робота №6, Деркач Д.А. ПІ-201");
    w.show();
    return a.exec();
}
