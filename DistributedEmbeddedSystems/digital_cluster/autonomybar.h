#ifndef AUTONOMYBAR_H
#define AUTONOMYBAR_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>

class AutonomyBar : public QWidget
{
    Q_OBJECT
public:
    AutonomyBar(QWidget *parent = nullptr);
    ~AutonomyBar();
    void set_autonomy(int i);

private:
    int nb_sections;
    int current;
    QVector<QWidget*> sections;
    QHBoxLayout *layout;
};

#endif // AutonomyBar_H
