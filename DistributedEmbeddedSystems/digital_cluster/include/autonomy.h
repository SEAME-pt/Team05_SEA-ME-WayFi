#ifndef AUTONOMY_H
#define AUTONOMY_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QCoreApplication>

class Autonomy : public QWidget
{
    Q_OBJECT
public:
    Autonomy(QWidget *parent = nullptr);
    ~Autonomy();
    void set_autonomy(int i);

private:
    int nb_sections;
    QVector<QWidget*> sections;
    QHBoxLayout *layout;
    QVBoxLayout *main_layout;
    QLabel *label;
};

#endif // Autonomy_H
