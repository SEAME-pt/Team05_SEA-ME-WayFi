#ifndef CANREADER_H
#define CANREADER_H

#include <QObject>
#include <QtCore>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <customdial.h>>

class CanReader : public QObject
{
    Q_OBJECT
public:
    CanReader(QObject *parent = nullptr);
    void start_reading(CustomDial *dial);
    ~CanReader();

private :
    int can_fd = -1;
signals:
};

#endif // CANREADER_H
