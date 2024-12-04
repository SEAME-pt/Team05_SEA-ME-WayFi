#include "mainwindow.h"
//#include "./ui_mainwindow.h"
#include <QDir>
#include <QVBoxLayout>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , process(new QProcess(this))
    , custom_dial(new CustomDial(this))
{
    setStyleSheet("background-color: rgb(4, 2, 54);");
    custom_dial->setFixedSize(500, 500);
    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(custom_dial, 0, Qt::AlignCenter);

    // Create a central widget and set it
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::read_speed);

    QTimer::singleShot(0, this, &MainWindow::init);
}

MainWindow::~MainWindow()
{
    //delete ui;
    delete process;
    delete custom_dial;
}

void MainWindow::init() {
    QString app = QCoreApplication::applicationDirPath();
    QString script = QDir(app).filePath("../get_speed.sh");
    script = QDir::cleanPath(script);

    if (!QFile::exists(script)) {
        std::cerr << "Script not found" << std::endl;
        return;
    }
    QStringList arguments;
    process->start(script, arguments);

    if (!process->waitForStarted()) {
        std::cerr << "Failed to start process!" << std::endl;
    }
}

void MainWindow::read_speed() {
    if (!process->waitForStarted()) {
        std::cerr << "Failed to start process!" << std::endl;
        return;
    }
    QByteArray output = process->readAllStandardOutput();
    bool ok;
    int speed = output.toInt(&ok);
    if (ok) {
        custom_dial->set_current(speed);
        custom_dial->update();
    } else {
        std::cerr << "Error reading speed data\n";
    }
}

