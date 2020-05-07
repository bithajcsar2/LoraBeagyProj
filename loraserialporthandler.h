#ifndef LORASERIALPORTHANDLER_H
#define LORASERIALPORTHANDLER_H

#include <QByteArray>
#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QDebug>
class LoraSerialPortHandler : public QObject
{
      Q_OBJECT
public:
    LoraSerialPortHandler(QObject *parent = nullptr);
    void write(QByteArray writeData);

private slots:
    void handleBytesWritten(qint64 bytes);
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort=nullptr;
    QByteArray m_writeData;
    QByteArray m_readData;
    qint64 m_bytesWritten = 0;
    QTimer m_timer;
};

#endif // LORASERIALPORTHANDLER_H
