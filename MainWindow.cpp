#include <ui_MainWindow.h>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    if(!serialWrite_.isOpen())
    {
        serialWrite_.openPort("COM7", 115200);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    if(!protocolHandler_.isOpen())
    {
        protocolHandler_.openPort("COM8", 115200);
    }
}


void MainWindow::on_sendPart1Button_clicked()
{
    if(serialWrite_.isOpen())
    {
        QByteArray dataToSend = QByteArray::fromRawData("\x02\x07\x01\x00\x03\x11\x22\x02", 8);
        serialWrite_.writeData(dataToSend);
    }
}


void MainWindow::on_sendPart2Button_clicked()
{
    if(serialWrite_.isOpen())
    {
        QByteArray dataToSend = QByteArray::fromRawData("\x33\x00", 2);
        serialWrite_.writeData(dataToSend);
    }
}
