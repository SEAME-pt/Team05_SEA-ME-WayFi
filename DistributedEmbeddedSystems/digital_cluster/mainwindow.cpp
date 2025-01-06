#include "mainwindow.h"
#include <iostream>
#include <QDir>
#include <QCoreApplication>
#include <QFontDatabase>
#include <unistd.h>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mqttClient(new QMqttClient(this))
{
    setStyleSheet("background-color: rgb(4, 2, 54);");
    left_dial = new CustomDial();
    left_dial->setStyleSheet("background-color: rgb(4, 2, 54);");
    left_dial->setFixedSize(400, 400);

    right_dial = new BatteryDial();
    right_dial->setStyleSheet("background-color: rgb(4, 2, 54);");
    right_dial->setFixedSize(400, 400);

    QHBoxLayout* layout = new QHBoxLayout(); //horizontal layout
    layout->addWidget(left_dial, 0, Qt::AlignCenter);
    layout->addWidget(right_dial, 0, Qt::AlignCenter);

    QWidget* centralWidget = new QWidget(); // Create a central widget and set it
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    init_mqtt();
    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, &MainWindow::publishMessage);
    // timer->start(1000); // Publish every 1000 milliseconds (1 second)
}

void MainWindow::init_mqtt() {
    mqttClient->setHostname("972e24210b544ba49bfb9c1d3164d02b.s1.eu.hivemq.cloud");
    mqttClient->setPort(8883);
    mqttClient->setUsername("jetracer"); 
    mqttClient->setPassword("Ft_seame5"); 

    connect(mqttClient, &QMqttClient::connected, this, &MainWindow::onMqttConnected);
    connect(mqttClient, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(mqttClient, &QMqttClient::stateChanged, this, &MainWindow::onMqttStateChanged);
    connect(mqttClient, &QMqttClient::errorChanged, this, [](QMqttClient::ClientError error) {
    qDebug() << "MQTT Client error:" << error;
    });

    mqttClient->connectToHostEncrypted();
}

MainWindow::~MainWindow()
{
    delete layout;
    delete left_dial;
    delete right_dial;
    delete centralWidget;

    std::cout << "Removing mainwindow" << std::endl;
}

void MainWindow::onMqttConnected() {
    qDebug() << "Connected to MQTT broker";
    QMqttTopicFilter topic("jetracer/speed");
    auto subscription = mqttClient->subscribe(topic);
    if (!subscription) {
        qDebug() << "Failed to subscribe to topic!";
    } else {
        qDebug() << "Successfully subscribed to topic!";
    }
}

void MainWindow::publishMessage()
{
    double speed = left_dial->get_current();
    QMqttTopicName topic("jetracer/speed");
    QByteArray message = QByteArray::number(speed);
    mqttClient->publish(topic, message); // Use QByteArray para a mensagem
    left_dial->set_current(speed);
}

void MainWindow::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "Message received on topic" << topic.name() << ":" << message;
    bool ok;
    double speed = message.toDouble(&ok); 
    if (ok) {
        // If the message is valid speed, update the dial
        QMetaObject::invokeMethod(this, [this, speed]() {
            qDebug() << "Updating left dial with speed:" << speed;
            left_dial->set_current(speed); // Update left dial with the speed
        }, Qt::QueuedConnection);
    } else {
        qDebug() << "Invalid speed data received!";
    }
}


void MainWindow::onMqttStateChanged(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::Disconnected:
        qDebug() << "MQTT Client Disconnected";
        break;
    case QMqttClient::Connecting:
        qDebug() << "MQTT Client Connecting...";
        break;
    case QMqttClient::Connected:
        qDebug() << "MQTT Client Connected!";
        break;
    }
}

