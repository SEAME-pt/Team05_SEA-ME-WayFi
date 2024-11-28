#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , process(new QProcess(this))
{
    ui->setupUi(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::read_speed);
    connect(ui->dial, &QDial::valueChanged, this, &MainWindow::on_dial_valueChanged);

    QString program = "../get_speed.sh";
    process->start(program);
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

void MainWindow::read_speed() {
    QByteArray output = get_process()->readAllStandardOutput();
    bool ok;
    int speed = output.toInt(&ok);
    if (ok) {
        get_ui()->label->setText(QString("Speed: %1 m/s").arg(speed));
        get_ui()->dial->setValue(speed);
    } else {
        std::cerr << "Error reading speed data\n";
    }
}

void MainWindow::on_dial_valueChanged(int value)
{
    get_ui()->label->setText(QString("Speed: %1 m/s").arg(value));
}

