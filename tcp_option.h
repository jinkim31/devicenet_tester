#ifndef TCPOPTION_H
#define TCPOPTION_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QWidget>
#include <QHostAddress>
#include <QLineEdit>

class TCPOption : public QWidget
{
    Q_OBJECT
public:
    using Option = struct{
        QString ip;
        unsigned int port;
    };

    TCPOption();
    const Option get();
private:
    QLineEdit editIP;
    QLineEdit editPort;
    QHBoxLayout layout;
    QLabel lblPort;
    QLabel lblIp;

};
#endif // TCPOPTION_H
