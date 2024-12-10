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
    // Set up the layout
    layout = new QVBoxLayout();
    layout->addWidget(custom_dial, 0, Qt::AlignCenter);

    // Create a central widget and set it
    centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    thread = new QThread();
    drive->moveToThread(thread);
    thread->start();
    QMetaObject::invokeMethod(drive, "start_process", Qt::QueuedConnection);
    can_read = new CanReader(this);
    can_read->start_reading(custom_dial);
}

MainWindow::~MainWindow()
{
    delete layout;
    delete custom_dial;
    delete centralWidget;

    QMetaObject::invokeMethod(drive, "stop_process", Qt::QueuedConnection);
    if (thread->isRunning()) {
        thread->quit();
        thread->wait();  // Ensure the thread finishes properly
        delete thread;
    }
    if (drive)
        delete drive;
    std::cout << "Removing mainwindow" << std::endl;
}
