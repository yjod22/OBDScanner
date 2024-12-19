#include "ProtocolHandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent)
    : QObject(parent), serialPort_(new QSerialPort(this))
{
    connect(serialPort_, &QSerialPort::readyRead, this, &ProtocolHandler::onReadyRead);
}

ProtocolHandler::~ProtocolHandler()
{
    if (serialPort_->isOpen())
    {
        serialPort_->close();
    }
}

bool ProtocolHandler::openPort(const QString &portName, qint32 baudRate)
{
    serialPort_->setPortName(portName);
    serialPort_->setBaudRate(baudRate);
    serialPort_->setDataBits(QSerialPort::Data8);
    serialPort_->setParity(QSerialPort::NoParity);
    serialPort_->setStopBits(QSerialPort::OneStop);
    serialPort_->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort_->open(QIODevice::ReadWrite))
    {
        qDebug() << "Port opened successfully!";
        return true;
    }
    else
    {
        qDebug() << "Failed to open port:" << serialPort_->errorString();
        return false;
    }
}

void ProtocolHandler::closePort()
{
    if (serialPort_->isOpen())
    {
        serialPort_->close();
        qDebug() << "Port closed!";
    }
}

void ProtocolHandler::writeData(const QByteArray &data)
{
    if (serialPort_->isOpen())
    {
        serialPort_->write(data);
    }
    else
    {
        qDebug() << "Port not open. Cannot write data.";
    }
}

bool ProtocolHandler::isOpen() const
{
    return serialPort_->isOpen();
}

void ProtocolHandler::onReadyRead()
{
    if (serialPort_->isOpen())
    {
        QByteArray qStream = serialPort_->readAll();
        std::vector<uint8_t>stream(qStream.begin(), qStream.end());
        streamDecoder_.reset();

        for (;;)
        {
            DecodeResult result = streamDecoder_.decode(stream);
            switch (result.state)
            {
                case DecodeResult::Completed:
                    qDebug() << "Decoded stream: ";
                    for (uint8_t byte : result.data)
                    {
                        qDebug() << static_cast<int>(byte) << " ";
                    }

                    // qDebug() << "Parsing packet...";
                    // packetParser_(result.data);
                    break;
                case DecodeResult::Ongoing:
                    qDebug() << "Waiting for more data...";
                    return;
                case DecodeResult::Error:
                    qDebug() << "Error: Malformed stream!";
                    return;
            }
        }
    }
}
