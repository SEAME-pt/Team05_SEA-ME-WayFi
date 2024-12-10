#ifndef CANREADER_H
#define CANREADER_H

#include <QObject>

class CanReader : public QObject
{
    Q_OBJECT
public:
    explicit CanReader(QObject *parent = nullptr);

signals:
};

#endif // CANREADER_H
