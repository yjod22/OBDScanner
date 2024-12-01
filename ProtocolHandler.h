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
    void writeData(const QByteArray &data);
    QByteArray readData();
    bool isOpen() const;

private slots:
    void handleReadyRead();

private:
    QSerialPort *m_serialPort;
    StreamDecoder streamDecoder_;
};

