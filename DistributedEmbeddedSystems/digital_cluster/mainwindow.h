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
#include "canreader.h"


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

private:
    CustomDial *custom_dial = nullptr;
    QThread *thread = nullptr;
    QThread *thread_can = nullptr;
    Driving *drive = nullptr;
    QVBoxLayout *layout = nullptr;
    QWidget *centralWidget = nullptr;
    CanReader *can_read;
};
#endif // MAINWINDOW_H
