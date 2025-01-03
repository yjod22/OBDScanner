#pragma once

#include <QMainWindow>

#include "ProtocolHandler.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

enum class PortState
{
    IDLE = 0,
    ACTIVATED,
    DEACTIVATED
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void onConnectButtonClicked();
    void onSendPart1ButtonClicked();
    void onSendPart2ButtonClicked();
    void onSendCANMessageButtonClicked();
    void runPortStateMachine();
    void onCANMessage(CANMessage& message);
    void onCableDisconnected();

    Ui::MainWindow *ui_;
    ProtocolHandler serialWrite_;
    ProtocolHandler protocolHandler_;
    PortState portState_{};
};
