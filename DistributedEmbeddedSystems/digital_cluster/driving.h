#ifndef DRIVING_H
#define DRIVING_H

#include <QObject>
#include <QThread>
#include <QProcess>

class Driving : public QObject
{
    Q_OBJECT
public:
    Driving(QObject *parent = nullptr);
    ~Driving();

public slots:
    void start_process() ;
    void stop_process() ;
signals:

private :
    QProcess *process;

};

#endif // DRIVING_H
