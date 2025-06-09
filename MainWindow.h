#pragma once

#include <QMainWindow>

#include "ProtocolHandler.hpp"
#include "SystemMacro.h"

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
    void runPortStateMachine();
    void onCANMessage(CANMessage &message);
    void onCableDisconnected();

    Ui::MainWindow *ui_;
    ProtocolHandler protocolHandler_;
    PortState portState_{};

#if DEBUG_MODE
    void onSendCANMessageButtonClicked();
    ProtocolHandler serialWrite_;
#endif
};
