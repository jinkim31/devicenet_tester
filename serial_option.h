#ifndef SERIALOPTION_H
#define SERIALOPTION_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QWidget>


class SerialOption : public QWidget
{
    Q_OBJECT
public:
    using Option = struct{
        QSerialPortInfo port;
        unsigned int baud;
    };

    SerialOption();
    const Option get();
private:
    QPushButton btnRefresh;
    QComboBox cboxPort;
    QComboBox cboxBaud;
    QHBoxLayout layout;
    QList<QSerialPortInfo> ports;
    QLabel lblPort;
    QLabel lblBaud;

    void refreshPorts();
};

#endif // SERIALOPTION_H
