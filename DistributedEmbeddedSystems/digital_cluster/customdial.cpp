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
    : QWidget(parent), current(1), max(7)
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

CustomDial::~CustomDial() {
    QFontDatabase::removeApplicationFont(font_id);
    QFontDatabase::removeApplicationFont(font_id2);
    std::cout << "Remove custom dial" << std::endl;
}

void CustomDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this); // create qpainter object
    painter.setRenderHint(QPainter::Antialiasing);
    int radius = qMin(width(), height()) / 2 - 10;  // Ensure it fits inside the widget
    painter.setPen(QPen(QColor(0, 120, 120, 98), 20));
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -270 * 16);
    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    angle_progress = std::min(angle_progress, 270.0f);
    QColor start_color(0, 80, 100);  
    QColor end_color(0, 255, 255);  

    int segments = 80;
    float segment_angle = angle_progress / segments;
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
    current = n * 3.6;
    update();
}

int CustomDial::get_current() {
    return current;
}

