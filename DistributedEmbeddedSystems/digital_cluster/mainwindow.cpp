#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLCDNumber>
#include <iostream>
#include <QVBoxLayout>
#include <QLabel>



// Speedometer is a QObject-based class, pass the this pointer (or an appropriate parent) to its constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , process(new QProcess(this))
    , speed(new Speedometer(this))
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(); // Create a new layout
    QLabel *title = new QLabel("Speedometer", this); // Create the label
    QFont font = title->font(); // Get the current font
    font.setPointSize(24); // Set the font size
    font.setBold(true); 
    title->setFont(font);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);  // Set text color to light gray
    title->setPalette(palette);
    layout->addWidget(title, 0, Qt::AlignCenter); // Add the label to the layout
    layout->addWidget(speed, 0, Qt::AlignCenter); // Add the speedometer widget

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    speed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    speed->setMinimumSize(400, 400);  // Set a minimum size for the widget

    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::read_speed);
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

Speedometer* MainWindow::get_speed() {
    return speed;
}


// void MainWindow::update_speed_display() {
//     get_ui()->lcdNumber->display(get_speed());
// }

void MainWindow::read_speed() {
    QByteArray output = get_process()->readAllStandardOutput();
    bool ok;
    int speed = output.toInt(&ok);

    if (ok) {
        get_speed()->set_speed(speed);
        // update_speed_display();
    } else {
        std::cerr << "Error reading speed data\n";
        return ;
    }
}
