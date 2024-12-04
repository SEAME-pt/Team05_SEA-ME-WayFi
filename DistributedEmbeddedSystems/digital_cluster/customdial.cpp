#include "customdial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

CustomDial::CustomDial(QWidget *parent)
    : QWidget(parent), current(0), max(10)
{}

void CustomDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this); // create qpainter object
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 10;  // Ensure it fits inside the widget

    // Draw background circle
    painter.setPen(QPen(Qt::darkCyan, 20));
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -270 * 16);

    //draw progression
    int angle_progress = (current * 270) / max;
    painter.setPen(QPen(Qt::cyan, 20));
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -angle_progress * 16);

    painter.setPen(QPen(Qt::cyan));

    QFontDatabase font;
    QFontDatabase font2;
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../digital-7.ttf"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QString calculator_path = QDir(path).filePath("../Calculator.ttf");
    calculator_path = QDir::cleanPath(calculator_path);

    font.addApplicationFont(digital_path);
    font2.addApplicationFont(calculator_path);

    painter.setFont(QFont("Digital-7", 150, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(current));

    painter.setPen(QPen(Qt::darkCyan));
    painter.setFont(QFont("Calculator", 35));
    QRect rect1 = rect();
    rect1.setTop(rect1.top() + 270);
    painter.drawText(rect1, Qt::AlignCenter, "km/h");
}

void CustomDial::set_current(int n) {
    current = n;
}



