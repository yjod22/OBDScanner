#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QDebug>

#include "StreamDecoder.hpp"
#include "PacketParser.hpp"

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
    void setCableDisconnectedCb(const std::function<void(void)> callback)
    {
        cableDisconnectedCb_ = callback;
    }
    void setCANMessageCb(const std::function<void(CANMessage&)> callback)
    {
        packetParser_.setCANMessageCb(callback);
    }

private:
    void onReadyRead();
    void onErrorOccurred(QSerialPort::SerialPortError error);

    QSerialPort *serialPort_;
    PacketParser packetParser_;
    StreamDecoder streamDecoder_;
    std::function<void(void)> cableDisconnectedCb_;
};

