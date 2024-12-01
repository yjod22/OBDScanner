#include "ProtocolHandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent)
    : QObject(parent), m_serialPort(new QSerialPort(this))
{
    // Connect the readyRead signal to the handler
    connect(m_serialPort, &QSerialPort::readyRead, this, &ProtocolHandler::handleReadyRead);
}

ProtocolHandler::~ProtocolHandler()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

bool ProtocolHandler::openPort(const QString &portName, qint32 baudRate)
{
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Port opened successfully!";
        return true;
    } else {
        qDebug() << "Failed to open port:" << m_serialPort->errorString();
        return false;
    }
}

void ProtocolHandler::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        qDebug() << "Port closed!";
    }
}

void ProtocolHandler::writeData(const QByteArray &data)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->write(data);
    } else {
        qDebug() << "Port not open. Cannot write data.";
    }
}

QByteArray ProtocolHandler::readData()
{
    QByteArray qStream;
    if (m_serialPort->isOpen()) {
        qStream = m_serialPort->readAll();
    }
    return qStream;
}

bool ProtocolHandler::isOpen() const
{
    return m_serialPort->isOpen();
}

void ProtocolHandler::handleReadyRead()
{
    // QByteArray data = readData();
    // qDebug() << "Data received:" << data;

    QByteArray qStream;
    if (m_serialPort->isOpen()) {
        qStream = m_serialPort->readAll();
        std::vector<uint8_t>stream(qStream.begin(), qStream.end());
        streamDecoder_.reset();

        for (;;) {
            DecodeResult result = streamDecoder_.decode(stream);
            switch (result.state) {
            case DecodeResult::Completed:
                qDebug() << "Decoded Packet: ";
                for (uint8_t byte : result.data) {
                    qDebug() << static_cast<int>(byte) << " ";
                }

                // qDebug() << "Parsing packet...";
                // packetParser_(result.data);
                break;
            case DecodeResult::Ongoing:
                qDebug() << "Waiting for more data...";
                return; // Simulate waiting for more data
            case DecodeResult::Error:
                qDebug() << "Error: Malformed packet!";
                return; // Exit with error
            }
        }
    }
}
