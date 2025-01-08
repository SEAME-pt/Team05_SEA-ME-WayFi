#include "customdial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <iostream>

CustomDial::CustomDial(QWidget *parent)
    : QWidget(parent), current(10), max(12)
{
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../digital-7.ttf"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QString calculator_path = QDir(path).filePath("../Calculator.ttf");
    calculator_path = QDir::cleanPath(calculator_path);

    font_id = font.addApplicationFont(digital_path);
    font_id2 = font2.addApplicationFont(calculator_path);
}

CustomDial::~CustomDial() {
    QFontDatabase::removeApplicationFont(font_id);
    QFontDatabase::removeApplicationFont(font_id2);
    std::cout << "Remove custom dial" << std::endl;
}

void CustomDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this); // create qpainter object
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 10;  // Ensure it fits inside the widget

    // Draw background circle
    painter.setPen(QPen(QColor(0, 120, 120, 98), 20));
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -270 * 16);

    //draw progression
    int angle_progress = (current * 270) / max;
    // painter.setPen(QPen(Qt::cyan, 20));
    QColor startColor(0, 80, 100);  // Deep cyan base color
    QColor endColor(0, 255, 255);  

    int segmentCount = 60; // Number of segments for smooth transition
    int segmentAngle = angle_progress / segmentCount;
    for (int i = 0; i < segmentCount; ++i) {
        float t = static_cast<float>(i) / segmentCount; // Transition factor (0 to 1)
        // Smooth transition from startColor to endColor
        QColor color = QColor::fromRgbF(
            (1 - t) * startColor.redF() + t * endColor.redF(),   // Red
            (1 - t) * startColor.greenF() + t * endColor.greenF(), // Green
            (1 - t) * startColor.blueF() + t * endColor.blueF()  // Blue
        );
        QPen pen(color, 20);
        painter.setPen(pen);
        painter.drawArc(10, 10, radius * 2, radius * 2, (225 - i * segmentAngle) * 16, -segmentAngle * 16);
    }

    //painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -angle_progress * 16);
    painter.setPen(QPen(Qt::cyan));
    painter.setFont(QFont("Digital-7", 100, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(current));

    painter.setPen(QPen(Qt::darkCyan));
    painter.setFont(QFont("Calculator", 35));
    QRect rect1 = rect();
    rect1.setTop(rect1.top() + 300);
    painter.drawText(rect1, Qt::AlignCenter, "km/h");
}

void CustomDial::set_current(int n) {
    current = n;
    update();
}

int CustomDial::get_current() {
    return current;
}

