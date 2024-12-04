#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <QTimer>
#include <QProcess>
#include <iostream>
#include <cstdlib>
#include <QThread>
#include "customdial.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void read_speed();
    void init();

private:
    //Ui::MainWindow *ui;
    QProcess *process;
    CustomDial *custom_dial;
};
#endif // MAINWINDOW_H
