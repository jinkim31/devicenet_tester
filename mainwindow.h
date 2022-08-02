#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

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

    void timerCallback();
};
#endif // MAINWINDOW_H
