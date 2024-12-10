#include "driving.h"
#include <iostream>
#include <QDir>
#include <QCoreApplication>
#include <unistd.h>

Driving::Driving(QObject *parent) : QObject(parent)
{
    process = nullptr;
}

Driving::~Driving() {
    std::cout << "Destroying driving"  << std::endl;
}

void Driving::start_process() {
    QString app = QCoreApplication::applicationDirPath();
    QString script = QDir(app).filePath("../get_speed.sh");
    script = QDir::cleanPath(script);
    if (!QFile::exists(script)) {
        std::cerr << "Script not found" << std::endl;
        return;
    }
    process = new QProcess();
    QStringList args;
    process->start(script, args);
    if (process && process->waitForStarted()) {
        std::cout << "Process started" << std::endl;
    }
    if (process && process->state() != QProcess::NotRunning)
        std::cout << "Process running"  << std::endl;

    // QString venv = "../motors/myenv/bin/python";
    // QString script = "../motors/motors.py";
    // QStringList args;
    // args << script;
    // process2->moveToThread(thread);
    // connect(thread, &QThread::started, [=]() {
    //     process2->start(venv, args);  // Start the process in the worker thread
    // });
}

void Driving::stop_process() {
    if (process && process->state() != QProcess::NotRunning) {
        std::cout << "Stopping process" << std::endl;
        process->terminate();
        if (!process->waitForFinished(300)) { // Wait for graceful termination
            process->kill();       // Force termination if needed
        }
    }
    std::cout << "Deleting process" << std::endl;
    delete process;
    process = nullptr;
}
