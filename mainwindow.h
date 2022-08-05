#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QQueue>
#include <QByteArray>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QIODevice>
#include "packet_translator.h"
#include "serial_option.h"
#include "tcp_option.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();

    void on_cboxComm_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> ports;
    QSerialPort serial;
    QList<QString> requests;
    QList<QString> responses;
    QString currentRequest;
    QString currentExpectedResponse;
    unsigned int currentIdx;
    QTimer timer;
    QQueue<char> rxQueue;
    PacketTranslator translator;
    SerialOption serialOption;
    TCPOption tcpOption;
    QTcpSocket socket;

    bool isTestRunning;
    void timerCallback();
    void rxCallback(QIODevice* device);
    bool start();
    bool stop();
    void setOption(QString commType);
};
#endif // MAINWINDOW_H
