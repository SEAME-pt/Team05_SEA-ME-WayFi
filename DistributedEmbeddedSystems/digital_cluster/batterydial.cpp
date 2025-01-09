#include "batterydial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QSettings>
#include <QLinearGradient>
#include <iostream>

BatteryDial::BatteryDial(QWidget *parent)
    : QWidget(parent), current(100), max(100)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    setFixedSize(400, 400);
}

BatteryDial::~BatteryDial() {
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
    int segments = 90; //nb of segments for smooth transition
    float segment_angle = angle_progress / segments;
    QColor start_color(0, 65, 74); 
    QColor end_color(0, 255, 200);
    if (current < 40) {
        start_color = QColor(0, 65, 74); 
        end_color = QColor(0, 140, 150, 255); 
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
    painter.setFont(QFont("Digital-7", 80, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(current));

    painter.setPen(QPen(QColor(0, 250, 195, 150)));
    QFont font("Digital-7", 40);
    painter.setFont(font);
    QRect percentage = rect();
    percentage.translate(0, 20);
    percentage.setWidth(percentage.width() - 80);
    painter.drawText(percentage, Qt::AlignCenter | Qt::AlignRight, "%");

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



