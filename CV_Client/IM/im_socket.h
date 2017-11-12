#ifndef IMSOCKET_H
#define IMSOCKET_H

#include <QObject>

class IMSocket : public QObject
{
    Q_OBJECT
public:
    IMSocket(QObject *parent = 0);
};

#endif // IMSOCKET_H
