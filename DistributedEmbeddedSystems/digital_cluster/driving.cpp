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
    QString script = QDir(app).filePath("../motors/motors.py");
    QString venv = QDir(app).filePath("../motors/venv/bin/python");
    script = QDir::cleanPath(script);
    venv = QDir::cleanPath(venv);
    if (!QFile::exists(script)) {
        std::cerr << "Motor script not found\n" << std::flush;
        return;
    }
    if (!QFile::exists(venv)) {
        std::cerr << "Python virtual environment not found\n" << std::flush;
        return;
    }
    process = new QProcess();
    QStringList args;
    args << script;
    connect(process, &QProcess::readyReadStandardOutput, this, [&]() {
        QByteArray output = process->readAllStandardOutput();
        std::cout << output.toStdString() << std::endl;
    });

    process->start(venv, args);  // Start the process in the worker thread
    if (process && process->waitForStarted())
        std::cout << "Process started\n" << std::flush;
    if (process && process->state() != QProcess::NotRunning)
        std::cout << "Process running\n" << std::flush;
}

void Driving::stop_process() {
    if (process && process->state() != QProcess::NotRunning) {
        std::cout << "Stopping process\n" << std::flush;
        process->terminate();
        if (!process->waitForFinished(300)) { // Wait for graceful termination
            process->kill();       // Force termination if needed
        }
    }
    std::cout << "Deleting process\n" << std::flush;
    delete process;
    process = nullptr;
}
