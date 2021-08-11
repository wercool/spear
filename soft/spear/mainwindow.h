#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openPortButton_clicked();

    void on_closePortButton_clicked();

    void on_clearLogOutputPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QMessageBox msgBox;

    QString selectedPort;
    int selectedPortBaudRate;
};
#endif // MAINWINDOW_H
