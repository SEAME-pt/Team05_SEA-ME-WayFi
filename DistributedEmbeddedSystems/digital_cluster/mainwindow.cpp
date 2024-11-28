#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLCDNumber>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , process(new QProcess(this))
{
    ui->setupUi(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::read_speed);

    QString program = "../get_speed.sh";
    QStringList arguments;  // Add any necessary arguments if required
    process->start(program, arguments);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Ui::MainWindow *MainWindow::get_ui(void) {
    return ui;
}

QProcess *MainWindow::get_process(void) {
    return process;
}

int MainWindow::get_speed() {
    return speed;
}

void MainWindow::set_speed(int speed1) {
    speed = speed1;
}

void MainWindow::update_speed_display() {
    get_ui()->lcdNumber->display(get_speed());
}

void MainWindow::read_speed() {
    QByteArray output = get_process()->readAllStandardOutput();
    bool ok;
    int speed = output.toInt(&ok);

    if (ok) {
        set_speed(speed);
        update_speed_display();
    } else {
        std::cerr << "Error reading speed data\n";
    }
}
