#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTimer>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
