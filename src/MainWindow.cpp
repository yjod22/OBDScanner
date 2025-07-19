#include <ui_MainWindow.h>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->portComboBox->setCurrentIndex(7);
    ui_->baudRateComboBox->setCurrentIndex(14);

    connect(ui_->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);

    protocolHandler_.setCANMessageCb(std::bind(&MainWindow::onCANMessage, this, std::placeholders::_1));
    protocolHandler_.setCableDisconnectedCb(std::bind(&MainWindow::onCableDisconnected, this));

#if DEBUG_MODE
    connect(ui_->sendCANMessageButton, &QPushButton::clicked, this, &MainWindow::onSendCANMessageButtonClicked);
    if (!serialWrite_.isOpen())
    {
        serialWrite_.openPort("COM7", 921600);
    }
#else
    ui_->sendCANMessageButton->hide();
#endif
}

MainWindow::~MainWindow()
{
    if (protocolHandler_.isOpen())
    {
        protocolHandler_.closePort();
    }

    delete ui_;
}

void MainWindow::onConnectButtonClicked()
{
    if (ui_->connectButton->text() == "Connect" &&
        protocolHandler_.openPort(ui_->portComboBox->currentText(), ui_->baudRateComboBox->currentText().toInt()))
    {
        portState_ = PortState::ACTIVATED;
    }

    if (ui_->connectButton->text() == "Disconnect")
    {
        portState_ = PortState::DEACTIVATED;
    }

    runPortStateMachine();
}

#if DEBUG_MODE
void MainWindow::onSendCANMessageButtonClicked()
{
    if (serialWrite_.isOpen())
    {
        QByteArray dataToSend = QByteArray::fromRawData(
            "\x02\x01\x02\x02\x01\x01\x02\x03\x01\x01\x0D\x04\x05\x06\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\x07\x00", 24);
        serialWrite_.writeData(dataToSend);
    }
}
#endif

void MainWindow::runPortStateMachine()
{
    switch (portState_)
    {
    case PortState::IDLE:
        break;

    case PortState::ACTIVATED:
        ui_->connectButton->setText("Disconnect");
        ui_->tableWidget->clearContents();
        ui_->tableWidget->setRowCount(0);
        ui_->portComboBox->setDisabled(true);
        ui_->baudRateComboBox->setDisabled(true);
        break;

    case PortState::DEACTIVATED:
        protocolHandler_.closePort();
        ui_->connectButton->setText("Connect");
        ui_->portComboBox->setDisabled(false);
        ui_->baudRateComboBox->setDisabled(false);
        break;
    }
}

void MainWindow::onCANMessage(CANMessage& message)
{
    ui_->tableWidget->insertRow(0);
    ui_->tableWidget->setVerticalHeaderLabels(QStringList("message"));

    for (int i = 0; i < ui_->tableWidget->columnCount(); i++)
    {
        ui_->tableWidget->setItem(0, i, new QTableWidgetItem());
    }
    ui_->tableWidget->item(0, 0)->setText(QString::number(message.getMsgCounter()));
    ui_->tableWidget->item(0, 1)->setText(QString::number(message.getStdId()));
    ui_->tableWidget->item(0, 2)->setText(QString::number(message.getExtId()));
    ui_->tableWidget->item(0, 3)->setText(QString::number(message.getIde()));
    ui_->tableWidget->item(0, 4)->setText(QString::number(message.getRtr()));
    ui_->tableWidget->item(0, 5)->setText(QString::number(message.getDlc()));
    for (int i = 0; i < 8; i++)
    {
        ui_->tableWidget->item(0, 6 + i)->setText(QString::number(message.getData()[i]));
    }
    ui_->tableWidget->item(0, 14)->setText(QString::number(message.getFmi()));
}

void MainWindow::onCableDisconnected()
{
    portState_ = PortState::DEACTIVATED;
    runPortStateMachine();
}
