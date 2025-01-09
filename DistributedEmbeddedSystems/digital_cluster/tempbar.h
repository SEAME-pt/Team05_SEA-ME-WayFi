#ifndef TEMPBAR_H
#define TEMPBAR_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>

class TempBar : public QWidget
{
    Q_OBJECT
public:
    TempBar(QWidget *parent = nullptr);
    ~TempBar();
    void set_temperature(int i);

private:
    int nb_sections;
    int current;
    QVector<QWidget*> sections;
    QHBoxLayout *layout;
};

#endif // TEMPBAR_H
