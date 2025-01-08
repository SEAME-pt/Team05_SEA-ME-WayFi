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
    : QWidget(parent), current(30), max(100)
{
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
    QPainter painter(this);  // create qpainter object
    painter.setRenderHint(QPainter::Antialiasing, true);
    int radius = qMin(width(), height()) / 2 - 10;  // Ensure it fits inside the widget

    painter.setPen(QPen(QColor(0, 230, 140, 60), 20)); // Draw background circle
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -270 * 16);

    int angle_progress = (current * 270) / max;
    int segmentCount = 50; // Number of segments for smooth transition
    int segmentAngle = angle_progress / segmentCount;
    QColor startColor(0, 65, 74);  // Base color
    QColor endColor(0, 255, 186);  // Brighter green for 100% battery
    // Adjust color based on the battery level
    if (current < 50) {
        // Darker green for lower battery levels (below or at 50)
        startColor = QColor(0, 65, 74);  // Darker base color
        endColor = QColor(0, 228, 220, 255);   // Darker bright green at max
    }
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



