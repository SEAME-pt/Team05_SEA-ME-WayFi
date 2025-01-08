#ifndef BATTERYDIAL_H
#define BATTERYDIAL_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QFontDatabase>

class BatteryDial : public QWidget
{
    Q_OBJECT
public:
    BatteryDial(QWidget *parent = nullptr);
    ~BatteryDial();
    void set_current(int n);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
    int font_id;
    int font_id2;
    QFontDatabase font;
    QFontDatabase font2;
signals:
};

#endif // BATTERYDIAL_H
