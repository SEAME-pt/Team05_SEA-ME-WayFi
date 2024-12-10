#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <QTimer>
#include <QProcess>
#include <cstdlib>
#include <QThread>
#include "customdial.h"
#include "driving.h"
#include <QVBoxLayout>


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
    CustomDial *custom_dial = nullptr;
    QThread *thread = nullptr;
    Driving *drive = nullptr;
    QVBoxLayout *layout = nullptr;
    QWidget *centralWidget = nullptr;
};
#endif // MAINWINDOW_H
