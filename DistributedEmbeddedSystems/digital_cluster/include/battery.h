#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QPaintEvent>
#include <gmock/gmock.h>

class PainterInterface {
public:
    virtual ~PainterInterface() = default;
    virtual void drawPixmap(const QRectF &targetRect, const QPixmap &pixmap) = 0;
    virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) = 0;
    virtual void setPen(const QPen &pen) = 0;
    virtual void drawText(const QRectF &rectangle, int flags, const QString &text) = 0;
    virtual void setRenderHint(QPainter::RenderHint hint, bool on) = 0;
    virtual bool begin(QPaintDevice *device) = 0;
    virtual bool end() = 0;
    virtual bool isActive() const = 0;
    virtual void setFont(const QFont &font) = 0;
    virtual QRect boundingRect(const QRect &rect, int flags, const QString &text) = 0;
};

class MockPainter : public PainterInterface {
public:
    MOCK_METHOD(void, drawPixmap, (const QRectF &targetRect, const QPixmap &pixmap), (override));
    MOCK_METHOD(void, drawArc, (int x, int y, int width, int height, int startAngle, int spanAngle), (override)); // Updated
    MOCK_METHOD(void, setPen, (const QPen &pen), (override));
    MOCK_METHOD(void, drawText, (const QRectF &rectangle, int flags, const QString &text), (override));
    MOCK_METHOD(void, setRenderHint, (QPainter::RenderHint hint, bool on), (override));
    MOCK_METHOD(bool, begin, (QPaintDevice *device), (override));
    MOCK_METHOD(bool, end, (), (override));
    MOCK_METHOD(bool, isActive, (), (const, override));
    MOCK_METHOD(void, setFont, (const QFont &font), (override));
    MOCK_METHOD(QRect, boundingRect, (const QRect &rect, int flags, const QString &text), (override));
};

class QPainterWrapper : public PainterInterface {
public:
    QPainterWrapper(QPainter *painter) : painter(painter) {}
      void setPen(const QPen &pen) override {
        painter->setPen(pen);
    }
    void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override {
        painter->drawArc(x, y, width, height, startAngle, spanAngle);
    }
    void drawPixmap(const QRectF &targetRect, const QPixmap &pixmap) override {
        // Convert QRectF to QRect and then call the appropriate overload of drawPixmap
        painter->drawPixmap(QRect(targetRect.toRect()), pixmap);
    }
    void drawText(const QRectF &rectangle, int flags, const QString &text) override {
        painter->drawText(rectangle, flags, text);
    }
    void setRenderHint(QPainter::RenderHint hint, bool on) override {
        painter->setRenderHint(hint, on);
    }
    bool begin(QPaintDevice *device) override {
        return painter->begin(device);
    }
    bool end() override {
        painter->end();
        return true;
    }
    bool isActive() const override {
        return painter->isActive();
    }
    void setFont(const QFont &font) override {
        painter->setFont(font);
    }
    QRect boundingRect(const QRect &rect, int flags, const QString &text) override {
        return painter->boundingRect(rect, flags, text);
    }
    private:
        QPainter *painter;
};


class Battery : public QWidget
{
    Q_OBJECT
public:
    Battery(QWidget *parent = nullptr);
    ~Battery();
    void set_current(int n);
    void draw_text(PainterInterface *painter, QRect bottomRect);
    void draw_arcs(PainterInterface *activePainter);
    void draw_pixmap(PainterInterface *activePainter);
    int get_current();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
    PainterInterface* test_painter = nullptr;  // Pointer to an external painter for testing

public:
    void setTestPainter(PainterInterface* painter) { test_painter = painter; }
};

#endif // BATTERY_H

