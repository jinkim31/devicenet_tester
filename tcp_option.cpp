#include "tcp_option.h"

TCPOption::TCPOption()
{
    this->setLayout(&layout);

    layout.addWidget(&lblIp);
    layout.addWidget(&editIP);
    layout.addWidget(&lblPort);
    layout.addWidget(&editPort);

    lblIp.setText("IP");
    //lblIp.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    lblPort.setText("Port");
    //lblPort.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    editIP.setText("192.168.10.1");
    editPort.setText("100");
}

const TCPOption::Option TCPOption::get()
{
    Option option;
    option.ip = editIP.text();
    option.port = editPort.text().toInt();

    return option;
}
