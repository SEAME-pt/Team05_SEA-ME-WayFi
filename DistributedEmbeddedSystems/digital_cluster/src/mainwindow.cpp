#include "../include/mainwindow.h"
#include <iostream>
#include <QDir>
#include <QCoreApplication>
#include <QFontDatabase>
#include <unistd.h>
#include <QProcessEnvironment>

//adding layouts and widgets to main window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
     , mqttClient(new QMqttClient(this))
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    left_dial = new Speed(this);
    right_dial = new Battery(this);
    QHBoxLayout* layout = new QHBoxLayout(); 
    layout->addWidget(left_dial, 1, Qt::AlignCenter);
    layout->addWidget(right_dial, 1, Qt::AlignCenter);
    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addLayout(layout, 4);

    temp = new Temperature(this);
    autonomy = new Autonomy(this);
    QHBoxLayout* layoutbar = new QHBoxLayout();
    layoutbar->setSpacing(80);
    layoutbar->addWidget(temp, 1, Qt::AlignRight);
    layoutbar->addWidget(autonomy, 1, Qt::AlignLeft);
    mainlayout->addLayout(layoutbar, 1);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainlayout);
    setCentralWidget(centralWidget);

    init_mqtt();
}

//close main window at destruction
MainWindow::~MainWindow()
{
    close();
}

//connecting to mqtt via cloud or localhost or to jetracer via network
void MainWindow::init_mqtt() {
    mqttClient->setHostname("972e24210b544ba49bfb9c1d3164d02b.s1.eu.hivemq.cloud"); //cloud
    mqttClient->setPort(8883);
    QString user = qgetenv("user");
    mqttClient->setUsername(user); 
    QString pass = qgetenv("password");
    mqttClient->setPassword(pass); 
    // mqttClient->setHostname("10.21.221.67"); //when on the same network
    // mqttClient->setPort(1883);
    // mqttClient->setHostname("127.0.0.1"); //when cross-compiling with jetracer

    connect(mqttClient, &QMqttClient::connected, this, &MainWindow::onMqttConnected);
    connect(mqttClient, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(mqttClient, &QMqttClient::errorChanged, this, [](QMqttClient::ClientError error) {
        qDebug() << "MQTT Client error:" << error;
    });
    mqttClient->connectToHostEncrypted(); //for cloud needs to be encrypted, for jetracer network or localhost its not encrypted
}

//subscribing to topic of mqtt
void MainWindow::onMqttConnected() {
    QMqttTopicFilter topic("jetracer/speed");
    auto subscription = mqttClient->subscribe(topic);
    QMqttTopicFilter battery("jetracer/battery");
    auto subscribe = mqttClient->subscribe(battery);
    QMqttTopicFilter temp("jetracer/temperature");
    auto sub = mqttClient->subscribe(temp);
    // QMqttTopicFilter autono("jetracer/autonomy");
    // auto subs = mqttClient->subscribe(autono);
    if (!subscription || !sub | !subscribe) {   // || !subs
        qDebug() << "Failed to subscribe to topic";
    } else {
        qDebug() << "Successfully subscribed to topic";
    }
}

//receiving message and updating current
void MainWindow::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "Message received on topic" << topic.name() << ":" << message;
    bool ok;
    double msg = message.toDouble(&ok); 
    if (ok) {
        if (topic.name() == "jetracer/speed") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                left_dial->set_current(static_cast<float>(msg));
            }, Qt::QueuedConnection);
        }
        else if (topic.name() == "jetracer/battery") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                right_dial->set_current(msg);
            }, Qt::QueuedConnection);
        }
        else if (topic.name() == "jetracer/temperature") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                temp->set_temperature(msg);
            }, Qt::QueuedConnection);
        }
        // else if (topic.name() == "jetracer/autonomy") {
        //     QMetaObject::invokeMethod(this, [this, msg]() {
        //         qDebug() << "Updating autonomy: " << msg;
        //         autonomy->set_autonomy(msg);
        //     }, Qt::QueuedConnection);
        // }
    } else {
        qDebug() << "Invalid data received";
    }
}
