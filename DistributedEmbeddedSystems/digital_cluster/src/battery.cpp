#include "../include/battery.h"
#include <QSettings>
#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

Battery::Battery(QWidget *parent)
    : QWidget(parent), current(20), max(100)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    setFixedSize(400, 470);
}

Battery::~Battery() {
    std::cout << "Remove battery" << std::endl;
}

//fading
void Battery::paintEvent(QPaintEvent *event) {
    QPainter painter(this); 
    painter.setRenderHint(QPainter::Antialiasing, true);
    int radius = qMin(width(), height()) / 2 - 10;  
    int segments = 300; 
    float segment_angle = 270.0f / segments; 
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / (segments);
        int alpha = static_cast<int>(120 * (1 - std::abs(2 * t - 1))); 
        QColor color(0, 52, 50, alpha); 
        QPen pen(color, 20);
        painter.setPen(pen);
        float overlap = 1.1f; 
        painter.drawArc(10, 10, radius * 2, radius * 2, (180 - i * segment_angle) * 16, -segment_angle * 16 * overlap);
    }
    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    segment_angle = angle_progress / segments;
    QColor start_color(0, 65, 74); 
    QColor end_color(0, 255, 200);
    if (current < 40) {
        segments = 200;
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
        int alpha = static_cast<int>(255 * (1 - std::abs(2 * t - 1)));
        color.setAlpha(alpha);
        QPen pen(color, 20);
        painter.setPen(pen);
        painter.drawArc(10, 10, radius * 2, radius * 2, (270 + i * segment_angle) * 16, segment_angle * 16);
    }
    paint_text(painter);
}

void Battery::paint_text(QPainter &painter) {
    painter.setPen(QPen(QColor(0, 250, 195)));
    painter.setFont(QFont("Digital-7", width() / 4, QFont::Bold));
    QRect textRect = rect();
    textRect.translate(0, -20); 
    painter.drawText(textRect, Qt::AlignCenter, QString::number(current));

    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/charging-station.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  
    QImage img = pixmap.toImage();
    for (int x = 0; x < img.width(); ++x) {
        for (int y = 0; y < img.height(); ++y) {
            QColor color = img.pixelColor(x, y);
            if (color == QColor(0, 0, 0)) { 
                color.setRgb(0, 120, 100); 
                img.setPixelColor(x, y, color);
            }
        }
    }
    pixmap.convertFromImage(img);
    pixmap = pixmap.scaled(35, 35, Qt::KeepAspectRatio);
    QRect rectBottom = this->rect();
    int yPos = rectBottom.bottom() - pixmap.height() - 10; // Position at the bottom
    QRect bottomRect = QRect((rectBottom.width() - pixmap.width()) / 2, yPos, pixmap.width(), pixmap.height());
    painter.drawPixmap(bottomRect, pixmap);

    QFont font("Calculator", width() / 18);
    painter.setFont(font);
    painter.setPen(QPen(QColor(0, 120, 100)));
    int xPos = bottomRect.right() + 10; 
    painter.drawText(xPos, yPos + 35, "%");
 
}

void Battery::set_current(int n) {
    current = n;
    update();
}
