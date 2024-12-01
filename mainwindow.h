#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ProtocolHandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void on_sendPart1Button_clicked();
    void on_sendPart2Button_clicked();

private:
    Ui::MainWindow *ui;

    ProtocolHandler serialWrite_;
    ProtocolHandler protocolHandler_;
};
#endif // MAINWINDOW_H
