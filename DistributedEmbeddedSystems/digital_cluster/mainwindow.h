#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <QTimer>
#include <QProcess>
#include <cstdlib>
#include <QDebug>
#include "speedometer.h"


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

    //void update_speed_display();
    void read_speed();
    Speedometer * get_speed();
    Ui::MainWindow *get_ui(void);
    QProcess *get_process(void);

private:
    Ui::MainWindow *ui;
    QProcess *process;
    Speedometer *speed;
};
#endif // MAINWINDOW_H
