#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>

class CustomDial : public QWidget
{
    Q_OBJECT
public:
    explicit CustomDial(QWidget *parent = nullptr);
    void set_current(int n);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
signals:
};

#endif // CUSTOMDIAL_H
