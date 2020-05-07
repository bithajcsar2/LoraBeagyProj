#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_dht22= new DHT22(this);
    sendHumTimer.setSingleShot(true);

    getDataTimer.start(10000);
    getDataTimer.setSingleShot(false);
    connect(&getDataTimer,&QTimer::timeout,this, &MainWindow::callDHT);


    m_loraSerialPortHandler = new LoraSerialPortHandler(this);

    QByteArray array = qPrintable("mac join abp");
    m_loraSerialPortHandler->write(array);
    /*QByteArray array2 = qPrintable("radio set sf sf12");
    m_loraSerialPortHandler->write(array2);*/

}

void MainWindow::callDHT()
{
    getDataTimer.start(1000*60*5);
    while(m_dht22->returnedData.size()!=2){
        m_dht22->read_dht_data();
        delay(2000);
    }
    ui->humLabel->setText(QString("Humidity: %1%").arg(m_dht22->returnedData.at(1)));
    ui->tempLabel->setText(QString("Temp: %1C°").arg(m_dht22->returnedData.at(0)));
    sendLoraMsgTemp();
}

void MainWindow::sendLoraMsgTemp()
{
    QString temp=QString::number(static_cast<int>(m_dht22->read_dht_data().at(0)),16);

    QByteArray array = qPrintable("mac tx cnf 5 ");
    array.append(temp);
    m_loraSerialPortHandler->write(array);
    sendHumTimer.start(1000*60);
}

void MainWindow::sendLoraMsgHum()
{
    QString hum=QString::number(static_cast<int>(m_dht22->read_dht_data().at(1)),16);

    QByteArray array = qPrintable("mac tx cnf 6 ");
    array.append(hum);
    m_loraSerialPortHandler->write(array);
}



MainWindow::~MainWindow()
{
    delete ui;
}

