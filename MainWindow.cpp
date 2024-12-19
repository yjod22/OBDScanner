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
