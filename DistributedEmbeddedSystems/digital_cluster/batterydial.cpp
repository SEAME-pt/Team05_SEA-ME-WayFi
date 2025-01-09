#include "batterydial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QLinearGradient>
#include <iostream>

BatteryDial::BatteryDial(QWidget *parent)
    : QWidget(parent), current(1), max(100)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    setFixedSize(400, 400);
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../digital-7.ttf"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QString calculator_path = QDir(path).filePath("../Calculator.ttf");
    calculator_path = QDir::cleanPath(calculator_path);

    font_id = font.addApplicationFont(digital_path);
    font_id2 = font2.addApplicationFont(calculator_path);
}

BatteryDial::~BatteryDial() {
    QFontDatabase::removeApplicationFont(font_id);
    QFontDatabase::removeApplicationFont(font_id2);
    std::cout << "Remove custom dial" << std::endl;
}

void BatteryDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this);  //create qpainter object
    painter.setRenderHint(QPainter::Antialiasing, true);
    int radius = qMin(width(), height()) / 2 - 10;  

    painter.setPen(QPen(QColor(0, 230, 140, 60), 20)); //background circle
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -270 * 16);

    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    int segments = 50; //nb of segments for smooth transition
    float segment_angle = angle_progress / segments;
    QColor start_color(0, 65, 74); 
    QColor end_color(0, 255, 186);
    if (current < 50) {
        start_color = QColor(0, 65, 74); 
        end_color = QColor(0, 228, 220, 255); 
    }
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / segments; //factor (0 to 1)
        QColor color = QColor::fromRgbF(
            (1 - t) * start_color.redF() + t * end_color.redF(),  
            (1 - t) * start_color.greenF() + t * end_color.greenF(), 
            (1 - t) * start_color.blueF() + t * end_color.blueF()  
        );
        QPen pen(color, 20);
        painter.setPen(pen);
        painter.drawArc(10, 10, radius * 2, radius * 2, (225 - i * segment_angle) * 16, -segment_angle * 16);
    }
    painter.setPen(QPen(QColor(0, 250, 195)));
    painter.setFont(QFont("Digital-7", 100, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(current));

    painter.setPen(QPen(QColor(0, 250, 195, 150)));
    QFont font("Digital-7", 40);
    painter.setFont(font);
    QRect percentage = rect();
    percentage.translate(0, 20);
    percentage.setWidth(percentage.width() - 90);
    painter.drawText(percentage, Qt::AlignRight | Qt::AlignVCenter, "%");

    painter.setPen(QPen(QColor(0, 255, 160, 150)));
    painter.setFont(QFont("Calculator", 35));
    QRect rect1 = rect();
    rect1.setTop(rect1.top() + 300);
    painter.drawText(rect1, Qt::AlignCenter, "Power");
}

void BatteryDial::set_current(int n) {
    current = n;
    update();
}



