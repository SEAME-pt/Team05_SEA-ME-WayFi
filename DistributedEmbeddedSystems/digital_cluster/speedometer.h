#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QRectF>

class Speedometer : public QWidget
{
    Q_OBJECT
public:
    explicit Speedometer(QWidget *parent = nullptr);
    void set_speed(int n);
    int get_speed();
    void paintEvent(QPaintEvent *) override;

private :
    int speed;

};

#endif // SPEEDOMETER_H
