#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QFontDatabase>

class CustomDial : public QWidget
{
    Q_OBJECT
public:
    CustomDial(QWidget *parent = nullptr);
    ~CustomDial();
    void set_current(int n);
    int get_current();
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

#endif // CUSTOMDIAL_H
