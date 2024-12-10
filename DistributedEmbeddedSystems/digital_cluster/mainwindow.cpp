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

    //connect(thread, &QThread::started, drive, &Driving::start_process);
    //drive->start_process();
    thread->start();
    QMetaObject::invokeMethod(drive, "start_process", Qt::QueuedConnection);
    // init();

}

MainWindow::~MainWindow()
{
    delete layout;
    delete custom_dial;
    delete centralWidget;

    //disconnect(thread, &QThread::started, drive, &Driving::start_process);
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

// void MainWindow::init() {
    // while(1) {
    //     std::cout << "Main thread\n";
    //     usleep(1000);
    //     break ;
    // }
//     if (!process) {
//         std::cerr << "Process not initialized\n";
//     }
//     connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::read_speed);
//     QString app = QCoreApplication::applicationDirPath();
//     QString script = QDir(app).filePath("../get_speed.sh");
//     script = QDir::cleanPath(script);
//     if (!QFile::exists(script)) {
//         std::cerr << "Script not found" << std::endl;
//         return;
//     }
//     QStringList arguments;
//     if (!process) {
//         std::cerr << "Process is not initialized properly" << std::endl;
//         return;
//     }
//     process->start(script, arguments);
// }

// void MainWindow::read_speed() {
//     if (!process->waitForStarted()) {
//         std::cerr << "Failed to start process!" << std::endl;
//         return;
//     }
//     QByteArray output = process->readAllStandardOutput();
//     bool ok;
//     int speed = output.toInt(&ok);
//     if (ok) {
//         custom_dial->set_current(speed);
//         custom_dial->update();
//     }
// }

