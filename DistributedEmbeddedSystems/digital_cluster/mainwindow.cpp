#include "mainwindow.h"
#include <iostream>
#include <QDir>
#include <QCoreApplication>
#include <QFontDatabase>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , drive(new Driving())
    , custom_dial(new CustomDial())
{
    setStyleSheet("background-color: rgb(4, 2, 54);");
    custom_dial->setFixedSize(600, 600);
    
    layout = new QVBoxLayout(); // Set up the layout
    layout->addWidget(custom_dial, 0, Qt::AlignCenter);

    centralWidget = new QWidget(); // Create a central widget and set it
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    thread = new QThread();
    drive->moveToThread(thread);
    thread->start();
    QMetaObject::invokeMethod(drive, "start_process", Qt::QueuedConnection);
    
    can_read = new CanReader();
    thread_can = new QThread();
    can_read->moveToThread(thread_can);
    thread_can->start();
    QMetaObject::invokeMethod(can_read, "start_reading", Qt::QueuedConnection,  Q_ARG(CustomDial*, custom_dial));
}

MainWindow::~MainWindow()
{
    delete layout;
    delete custom_dial;
    delete centralWidget;
    QMetaObject::invokeMethod(drive, "stop_process", Qt::QueuedConnection);
    QMetaObject::invokeMethod(can_read, "stop_reading", Qt::QueuedConnection);

    thread->quit();
    thread->wait();  // Ensure the thread finishes properly
    delete thread;

    thread_can->quit();
    thread_can->wait();  // Ensure the thread finishes properly
    delete thread_can;
    delete drive;
    delete can_read;
    std::cout << "Removing mainwindow" << std::endl;
}
