#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QDebug>

#include "StreamDecoder.h"

class ProtocolHandler : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolHandler(QObject *parent = nullptr);
    ~ProtocolHandler();

    bool openPort(const QString &portName, qint32 baudRate);
    void closePort();
    bool isOpen() const;
    void writeData(const QByteArray &data);

private:
    void onReadyRead();

    QSerialPort *serialPort_;
    StreamDecoder streamDecoder_;
};

