#include "customdial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>

CustomDial::CustomDial(QWidget *parent)
    : QWidget(parent), current(0), max(10)
{
}

void CustomDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this); // create qpainter object
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(width(), height()) / 2 - 20;  // Ensure it fits inside the widget

    // Draw background circle
    painter.setPen(QPen(Qt::darkCyan, 10));
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -270 * 16);

    int angleProgress = (current * 270) / max;
    painter.setPen(QPen(Qt::cyan, 8));
    painter.drawArc(10, 10, radius * 2, radius * 2, 225 * 16, -angleProgress * 16);

    painter.setPen(QPen(Qt::cyan));
    painter.setFont(QFont("Arial", 25, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(current) + " km/h");
}

void CustomDial::set_current(int n) {
    current = n;
}



