#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>

QString load(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream in(&file);
    return in.readAll();
}

void save(const QString& fileName, const QString& text)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << text;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    isTestRunning = false;

    ui->setupUi(this);
    connect(&timer, &QTimer::timeout, this, &MainWindow::timerCallback);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    ports = QSerialPortInfo::availablePorts();
    connect(&serial, &QSerialPort::readyRead, this, [this]{ rxCallback(&serial); });
    connect(&socket, &QSerialPort::readyRead, this, [this]{ rxCallback(&socket); });

    ui->vboxCommOption->addWidget(&serialOption);
    ui->vboxCommOption->addWidget(&tcpOption);

    ui->txtRequest->setText(load("request.txt"));
    ui->txtResponse->setText(load("response.txt"));

    this->setOption("Serial");
}

MainWindow::~MainWindow()
{
    save("request.txt", ui->txtRequest->toPlainText());
    save("response.txt", ui->txtResponse->toPlainText());
    delete ui;
}

void MainWindow::on_btnStart_clicked()
{
    if(isTestRunning)
    {
        if(stop()) isTestRunning = false;
    }
    else
    {
        if(start()) isTestRunning = true;
    }

}

QString byteArrayStr(const QString str)
{
    QString res = "[";
    for(int i=0; i<str.length(); i++)
    {
        res += QString::number(str[i].unicode()) +" ";
    }

    return res + "]";
}
void MainWindow::timerCallback()
{
    // process response of previous request
    bool received = false;
    QString read;

    if(currentRequest != "")
    {
        while(!rxQueue.isEmpty())
        {
            if(translator.push(rxQueue.dequeue()))
            {
                received = true;
                read = translator.getStr();
            }
        }

        if(!received)
        {
            ui->txtLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss") +": (" + QString::number(currentIdx) + ") " + currentRequest + " " + byteArrayStr(currentRequest) + " / NO RESPONSE");
        }
        else
        {
            // check match
            QRegularExpression re(currentExpectedResponse);
            //read.chop(1);
            //qDebug()<<currentExpectedResponse<<"("<<currentExpectedResponse.size()<<") "<<read<<"("<<read.size()<<")";
            QRegularExpressionMatch match = re.match(read);
            if(match.hasMatch())
            {
                //ui->txtTerminal->append("(" + QString::number(currentIdx) + ") " + currentRequest + " / " +read);
            }
            else
            {
                ui->txtLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + ": (" + QString::number(currentIdx) + ") " + currentRequest + " " + byteArrayStr(currentRequest) + " / " +read+ " " + byteArrayStr(read));
            }
        }

    }

    if(++currentIdx >= requests.size()) currentIdx = 0;

    currentRequest = requests[currentIdx];
    currentExpectedResponse = responses[currentIdx];

    ui->txtTerminal->append(QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss") +" tx: "+currentRequest);

    if(ui->cboxComm->currentText() == "Serial")
    {
        serial.write(QString(currentRequest + "\r\n").toLocal8Bit());
        serial.flush();
    }
    else if(ui->cboxComm->currentText() == "TCP")
    {
        qDebug()<<"socket write";
        socket.write(QString(currentRequest + "\r\n").toLocal8Bit());
        socket.flush();
    }
}

void MainWindow::rxCallback(QIODevice* device)
{
    auto byteArray = device->readAll();
    QString str;
    for(int i=0; i<byteArray.size(); i++)
    {
        rxQueue.append(byteArray[i]);
        str+=byteArray[i];
    }

    ui->txtTerminal->append(QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss") +" rx: "+str);
}

bool MainWindow::start()
{
    requests = ui->txtRequest->toPlainText().trimmed().split('\n');
    responses = ui->txtResponse->toPlainText().trimmed().split('\n');

    if(requests.size() != responses.size())
    {
        ui->txtTerminal->append("Request and expected responses are not configured properly.");
        return false;
    }

    if(ui->cboxComm->currentText() == "Serial")
    {
        auto option = serialOption.get();
        serial.setBaudRate(option.baud);
        serial.setPort(option.port);

        if(serial.open(QIODeviceBase::ReadWrite))
        {
            ui->txtTerminal->append("Port opened");
        }
        else
        {
            ui->txtTerminal->append("Port open failed("+ QString::number(serial.error())+").");
            return false;
        }
    }
    else if(ui->cboxComm->currentText() == "TCP")
    {
        auto option = tcpOption.get();
        socket.connectToHost(option.ip, option.port, QAbstractSocket::ReadWrite);
        qDebug()<<option.ip<<option.port;

        if(socket.waitForConnected(1000))
        {
            ui->txtTerminal->append("Socket connected");
        }
        else
        {
            ui->txtTerminal->append("Socket connection failed("+ QString::number(socket.error())+").");
            return false;
        }
    }

    currentIdx = requests.size();
    timer.start(1000.0 / ui->sboxHz->value());

    ui->btnStart->setText("Stop");
    ui->txtRequest->setDisabled(true);
    ui->txtResponse->setDisabled(true);
    return true;
}

bool MainWindow::stop()
{
    timer.stop();

    if(ui->cboxComm->currentText() == "Serial")
    {
        serial.close();
    }
    else if(ui->cboxComm->currentText() == "TCP")
    {
        socket.close();
    }


    ui->btnStart->setText("Start");
    ui->txtRequest->setDisabled(false);
    ui->txtResponse->setDisabled(false);
    return true;
}

void MainWindow::setOption(QString commType)
{
    serialOption.hide();
    tcpOption.hide();
    if(commType == "Serial") serialOption.show();
    else if(commType == "TCP") tcpOption.show();
}


void MainWindow::on_cboxComm_currentTextChanged(const QString& str)
{
    setOption(str);
}


void MainWindow::on_btnTerminalClear_clicked()
{
    ui->txtTerminal->clear();
}


void MainWindow::on_btnLogClear_clicked()
{
    ui->txtLog->clear();
}

