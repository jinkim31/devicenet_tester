#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QQueue>
#include <QByteArray>
#include "packet_translator.h"

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

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> ports;
    QSerialPort serial;
    QList<QString> requests;
    QList<QString> responses;
    QString currentRequest;
    unsigned int currentIdx;
    QTimer timer;
    QQueue<char> rxQueue;
    PacketTranslator translator;

    void timerCallback();
    void rxCallback();
};
#endif // MAINWINDOW_H
