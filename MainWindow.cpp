#include <ui_MainWindow.h>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    connect(ui_->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(ui_->sendPart1Button, &QPushButton::clicked, this, &MainWindow::onSendPart1ButtonClicked);
    connect(ui_->sendPart2Button, &QPushButton::clicked, this, &MainWindow::onSendPart2ButtonClicked);
    connect(ui_->sendCANMessageButton, &QPushButton::clicked, this, &MainWindow::onSendCANMessageButtonClicked);

    protocolHandler_.setCANMessageCb(std::bind(&MainWindow::onCANMessage, this, std::placeholders::_1));

    if(!serialWrite_.isOpen())
    {
        serialWrite_.openPort("COM7", 115200);
    }
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onConnectButtonClicked()
{
    if(!protocolHandler_.isOpen())
    {
        protocolHandler_.openPort("COM8", 115200);
    }
}


void MainWindow::onSendPart1ButtonClicked()
{
    if(serialWrite_.isOpen())
    {
        QByteArray dataToSend = QByteArray::fromRawData("\x02\x07\x01\x00\x03\x11\x22\x02", 8);
        serialWrite_.writeData(dataToSend);
    }
}


void MainWindow::onSendPart2ButtonClicked()
{
    if(serialWrite_.isOpen())
    {
        QByteArray dataToSend = QByteArray::fromRawData("\x33\x00", 2);
        serialWrite_.writeData(dataToSend);
    }
}

void MainWindow::onSendCANMessageButtonClicked()
{
    if(serialWrite_.isOpen())
    {
        QByteArray dataToSend = QByteArray::fromRawData("\x01\x02\x01\x01\x01\x02\x02\x01\x01\x03\x03\x01\x0B\x08\x10\x20\x30\x40\x50\x60\x70\x80\x05\x00", 24);
        serialWrite_.writeData(dataToSend);
    }
}


 void MainWindow::onCANMessage(CANMessage& message)
{
    qDebug() << "msgCounter:" << static_cast<int>(message.msgCounter);
    qDebug() << "stdId:" << static_cast<int>(message.stdId);
    qDebug() << "extId:" << static_cast<int>(message.extId);
    qDebug() << "dlc:" << static_cast<int>(message.dlc);
    qDebug() << "data[0]:" << static_cast<int>(message.data[0]);
    qDebug() << "data[1]:" << static_cast<int>(message.data[1]);
    qDebug() << "data[2]:" << static_cast<int>(message.data[2]);
    qDebug() << "data[3]:" << static_cast<int>(message.data[3]);
    qDebug() << "data[4]:" << static_cast<int>(message.data[4]);
    qDebug() << "data[5]:" << static_cast<int>(message.data[5]);
    qDebug() << "data[6]:" << static_cast<int>(message.data[6]);
    qDebug() << "data[7]:" << static_cast<int>(message.data[7]);
}
