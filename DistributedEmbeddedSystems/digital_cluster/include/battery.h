#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QPaintEvent>

class Battery : public QWidget
{
    Q_OBJECT
public:
    Battery(QWidget *parent = nullptr);
    ~Battery();
    void set_current(int n);
    void paint_text(QPainter &painter);
    int get_current();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
};


#ifdef BUILD_TESTS
#include <gmock/gmock.h>

class MockQPainter : public QPainter {
public:
    MOCK_METHOD6(drawArc, void(int, int, int, int, int, int));
    MOCK_METHOD1(setPen, void(const QPen&));
    MOCK_METHOD3(drawText, void(const QRect&, int, const QString&));
    MOCK_METHOD2(drawPixmap, void(const QRect&, const QPixmap&));
};
#endif

#endif // BATTERY_H

