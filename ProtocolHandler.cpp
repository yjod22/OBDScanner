#include "ProtocolHandler.hpp"

ProtocolHandler::ProtocolHandler(QObject *parent)
    : QObject(parent), serialPort_(new QSerialPort(this))
{
    connect(serialPort_, &QSerialPort::readyRead, this, &ProtocolHandler::onReadyRead);
    connect(serialPort_, &QSerialPort::errorOccurred, this, &ProtocolHandler::onErrorOccurred);
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

    return (serialPort_->open(QIODevice::ReadWrite));
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
                    packetParser_.parse(result.data);
                    break;
                case DecodeResult::Ongoing:
                    return;
                case DecodeResult::Error:
                    qDebug() << "Error: Malformed stream!";
                    return;
            }
        }
    }
}

void ProtocolHandler::onErrorOccurred(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ResourceError)
    {
        cableDisconnectedCb_();
    }
}
