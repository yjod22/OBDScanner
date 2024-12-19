#pragma once

#include <QMainWindow>

#include "ProtocolHandler.h"

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
    void onConnectButtonClicked();
    void onSendPart1ButtonClicked();
    void onSendPart2ButtonClicked();

private:
    Ui::MainWindow *ui_;
    ProtocolHandler serialWrite_;
    ProtocolHandler protocolHandler_;
};
