#include "serial_option.h"

SerialOption::SerialOption()
{
    this->setLayout(&layout);

    layout.addWidget(&btnRefresh);
    layout.addWidget(&lblPort);
    layout.addWidget(&cboxPort);
    layout.addWidget(&lblBaud);
    layout.addWidget(&cboxBaud);

    lblBaud.setText("Baud rate");
    //lblBaud.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    lblPort.setText("Port");
    //lblPort.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    btnRefresh.setText("Refresh");
    //btnRefresh.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cboxBaud.addItem("4800");
    cboxBaud.addItem("9600");
    cboxBaud.addItem("14400");
    cboxBaud.addItem("19200");
    cboxBaud.addItem("28800");
    cboxBaud.addItem("38400");
    cboxBaud.addItem("57600");
    cboxBaud.addItem("115200");
    cboxBaud.addItem("500000");
    cboxBaud.addItem("1000000");
    cboxBaud.setCurrentIndex(7);

    connect(&btnRefresh, &QPushButton::clicked, [&]{refreshPorts();});

    refreshPorts();
}

const SerialOption::Option SerialOption::get()
{

    Option option;
    option.port = ports[cboxPort.currentIndex()];
    option.baud = cboxBaud.currentText().toInt();

    return option;
}

void SerialOption::refreshPorts()
{
    ports = QSerialPortInfo::availablePorts();

    cboxPort.clear();
    foreach (QSerialPortInfo port, ports)
        cboxPort.addItem(port.portName());
}
