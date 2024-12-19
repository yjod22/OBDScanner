#pragma once

#include <QMainWindow>

#include "ProtocolHandler.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

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
    void onCANMessage(CANMessage& message);

    Ui::MainWindow *ui_;
    ProtocolHandler serialWrite_;
    ProtocolHandler protocolHandler_;
};
