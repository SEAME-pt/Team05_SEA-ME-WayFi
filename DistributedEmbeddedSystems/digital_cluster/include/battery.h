#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>

class Battery : public QWidget
{
    Q_OBJECT
public:
    Battery(QWidget *parent = nullptr);
    ~Battery();
    void set_current(int n);
    void paint_text(QPainter &painter);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
};

#endif // BATTERY_H
