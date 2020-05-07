#ifndef DHT22_H
#define DHT22_H

#include <QObject>
#include "wiringPi.h"
#include <QVector>

class DHT22 : public QObject
{
    Q_OBJECT
public:
    explicit DHT22(QObject *parent = nullptr);
    QVector<double> read_dht_data();
    QVector<double> returnedData;
signals:

public slots:
};

#endif // DHT22_H
