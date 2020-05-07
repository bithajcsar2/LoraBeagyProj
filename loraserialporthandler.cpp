#include "loraserialporthandler.h"

LoraSerialPortHandler::LoraSerialPortHandler(QObject *parent):
    QObject(parent)
{
    QSerialPort serialPort;
    const QString serialPortName = "/dev/ttyUSB0";

    m_serialPort= new QSerialPort();
    m_serialPort->setPortName(serialPortName);
    m_serialPort->setBaudRate(QSerialPort::Baud57600);
    m_serialPort->setParity(QSerialPort::Parity::NoParity);
    m_serialPort->setStopBits(QSerialPort::StopBits::OneStop);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    m_serialPort->open(QIODevice::ReadWrite);

    m_timer.setSingleShot(true);
    connect(m_serialPort, &QSerialPort::bytesWritten,
            this, &LoraSerialPortHandler::handleBytesWritten);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &LoraSerialPortHandler::handleError);
    connect(m_serialPort, &QSerialPort::readyRead,
            this, &LoraSerialPortHandler::handleReadyRead);
    connect(&m_timer, &QTimer::timeout, this, &LoraSerialPortHandler::handleTimeout);
}

void LoraSerialPortHandler::handleReadyRead()
{
    m_readData.append(m_serialPort->readAll());

    if (!m_timer.isActive())
        m_timer.start(1000);
}


void LoraSerialPortHandler::handleTimeout()
{
    if (m_readData.isEmpty()) {
        qDebug() << QObject::tr("No data was currently available "
                                "for reading from port %1")
                            .arg(m_serialPort->portName())
                         << endl;
    } else {
        qDebug() << QObject::tr("Data successfully received from port %1")
                            .arg(m_serialPort->portName())
                         << endl;
        qDebug() << m_readData << endl;
    }

}

void LoraSerialPortHandler::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        qDebug() << QObject::tr("Data successfully sent to port %1")
                            .arg(m_serialPort->portName()) << endl;
    }
}


void LoraSerialPortHandler::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::WriteError) {
        qDebug() << QObject::tr("An I/O error occurred while writing"
                                        " the data to port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;
    }
}

void LoraSerialPortHandler::write(QByteArray writeData)
{
    m_writeData = writeData;
    m_writeData.append("\r\n");
    const qint64 bytesWritten = m_serialPort->write(m_writeData);

    if (bytesWritten == -1) {
        qDebug() << QObject::tr("Failed to write the data to port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;
    } else if (bytesWritten != m_writeData.size()) {
        qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;
    }
     //m_timer.start(5000);
}
