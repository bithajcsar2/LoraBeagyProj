#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "loraserialporthandler.h"
#include "dht22.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
    QTimer getDataTimer;
    LoraSerialPortHandler *m_loraSerialPortHandler;
    DHT22 *m_dht22;
    QTimer sendHumTimer;

public slots:
    void callDHT();
    void sendLoraMsgTemp();
    void sendLoraMsgHum();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
