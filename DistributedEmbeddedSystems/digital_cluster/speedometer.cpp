#include "speedometer.h"
#include <iostream>
#include <QPen>
#include <cmath>

#define qDegreesToRadians(deg) ((deg) * M_PI / 180.0)

Speedometer::Speedometer(QWidget *parent)
    : QWidget{parent}
{
    speed = 0;
}

int Speedometer::get_speed() {
    return speed;
}

void Speedometer::set_speed(int value) {
    speed = qBound(0, value, 200);
    update();
}

void Speedometer::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // better image

    //draw circle
    QPen pen1(Qt::black, 1);
    painter.setPen(pen1);
    QRectF rect(0, 0, width() - 30, height() - 30);
    painter.drawEllipse(rect);

    //draw ticks
    for (int i = 0; i <= 200; i++) {
        painter.save();
        painter.translate(rect.center());
        painter.rotate(-135.0 + (i / 200.0) * 270.0);
        QPen pen2(Qt::black, 1);
        painter.setPen(pen2);
        painter.drawLine(0, -rect.height() / 2, 0, -(rect.height() / 2) + 10);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 12));
        if (i % 20 == 0) {  // Show a number every 20 ticks (for readability)
            painter.drawText(10, -(rect.height() / 2) + 15, QString::number(i));
        }
        painter.restore();
        i+= 9;
    }

    //draw needle
    painter.save();
    painter.translate(rect.center());
    painter.rotate(-135.0 + (speed / 200.0) * 270.0);
    QPen pen(Qt::red, 3);
    painter.setPen(pen);
    painter.drawLine(0, 0, 0, -rect.height() / 2 + 10);
    painter.restore();
}
