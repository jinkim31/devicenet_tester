#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, &QTimer::timeout, this, &MainWindow::timerCallback);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    ports = QSerialPortInfo::availablePorts();
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::rxCallback);
    for(int i=0; i<ports.size(); i++)
    {
        qDebug()<<ports[i].portName();
        ui->cboxPort->addItem(ports[i].portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnStart_clicked()
{
    requests = ui->txtRequest->toPlainText().trimmed().split('\n');
    responses = ui->txtResponse->toPlainText().trimmed().split('\n');

    if(requests.size() != responses.size())
    {
        ui->txtTerminal->append("Request and expected responses are not configured properly.");
        return;
    }

    serial.setBaudRate(ui->cboxBaud->currentText().toInt());
    serial.setPort(ports[ui->cboxPort->currentIndex()]);
    if(serial.open(QIODeviceBase::ReadWrite))
    {
        ui->txtTerminal->append("Port opened\n");
    }
    else
    {
        ui->txtTerminal->append("Port open failed("+ QString::number(serial.error())+").");
        return;
    }

    currentIdx = requests.size();
    timer.start(1000.0 / ui->sboxHz->value());

}

void MainWindow::timerCallback()
{
    // process response of previous request
    bool received = false;

    if(currentRequest != "")
    {
        while(!rxQueue.isEmpty())
        {
            if(translator.push(rxQueue.dequeue()))
            {
                received = true;

                QString read = translator.getStr();

                if(read == currentExpectedResponse)
                {
                    ui->txtTerminal->append("(" + QString::number(currentIdx) + ") " + currentRequest + " / " +read);
                }
                else
                {
                    ui->txtLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + ": (" + QString::number(currentIdx) + ") " + currentRequest + " / " +read);
                }
                break;
            }
        }

        if(!received)
        {
            ui->txtLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss") +": (" + QString::number(currentIdx) + ") " + currentRequest + " / NO RESPONSE");
        }
    }

    if(++currentIdx >= requests.size()) currentIdx = 0;

    currentRequest = requests[currentIdx];
    currentExpectedResponse = responses[currentIdx];

    serial.write(QString(currentRequest + "\r\n").toLocal8Bit());
    serial.flush();
}

void MainWindow::rxCallback()
{
    auto byteArray = serial.readAll();
    for(int i=0; i<byteArray.size(); i++) rxQueue.append(byteArray[i]);
}

