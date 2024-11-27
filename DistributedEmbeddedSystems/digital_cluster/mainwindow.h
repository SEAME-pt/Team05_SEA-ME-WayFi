#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <QTimer>
#include <QProcess>
#include <iostream>
#include <cstdlib>
#include <QDebug>


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

    void update_speed_display();
    void read_speed();
    int get_speed();
    void set_speed(int speed);
    Ui::MainWindow *get_ui(void);
    QProcess *get_process(void);

private:
    Ui::MainWindow *ui;
    int speed;
    QProcess *process;
};
#endif // MAINWINDOW_H
