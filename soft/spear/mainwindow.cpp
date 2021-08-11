#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    msgBox.setWindowTitle("Spear Control");

    ui->serialPortBaudRateComboBox->addItems({
                                                 "2400",
                                                 "4800",
                                                 "9600",
                                                 "19200",
                                                 "38400",
                                                 "57600",
                                                 "115200"
                                             });
    ui->serialPortBaudRateComboBox->setCurrentIndex(2);

    serialPort = new QSerialPort(this);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->serialPortsComboBox->addItem(serialPortInfo.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openPortButton_clicked()
{
    selectedPort = ui->serialPortsComboBox->currentText();
    selectedPortBaudRate = ui->serialPortBaudRateComboBox->currentText().toInt();
    serialPort->setPortName(selectedPort);
    serialPort->setBaudRate(selectedPortBaudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::SoftwareControl);

    if (serialPort->open(QIODevice::ReadWrite)){
        qDebug() << "Serial port " + selectedPort + " opened successfully";
        ui->logOutput->append("Serial port <b>" + selectedPort + "</b> opened successfully\n");
    } else {
        qDebug() << serialPort->errorString();
        msgBox.setText(serialPort->errorString());
        msgBox.exec();
    }

}


void MainWindow::on_closePortButton_clicked()
{
    serialPort->close();
    qDebug() << "Serial port " + selectedPort + " closed";
    ui->logOutput->append("Serial port <b>" + selectedPort + "</b> closed\n");
}


void MainWindow::on_clearLogOutputPushButton_clicked()
{
    ui->logOutput->clear();
}


void MainWindow::on_refreshPortListpushButton_clicked()
{
    ui->serialPortsComboBox->clear();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->serialPortsComboBox->addItem(serialPortInfo.portName());
    }
}

