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

    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::serialReadData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    on_closePortButton_clicked();
    event->accept();
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
        portOpened = true;
        serialPort->setRequestToSend(false);
        ui->logOutput->append("Serial port <b>" + selectedPort + "</b> opened successfully");
    } else {
        msgBox.setText(serialPort->errorString());
        msgBox.exec();
    }

}


void MainWindow::on_closePortButton_clicked()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        portOpened = false;
        ui->logOutput->append("Serial port <b>" + selectedPort + "</b> closed");
        qDebug() << "Serial port " + selectedPort + " closed";
    }
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

void MainWindow::sendCommand(QString command) {
    QString logMessage;
    if (portOpened) {
        serialPort->setRequestToSend(true);
        QByteArray buffer = "!{" + QByteArray::fromStdString(command.toStdString()) + "}!\n";
        serialPort->write(buffer);
        serialPort->setRequestToSend(false);

        qDebug() << "← " + QString(buffer);

        logMessage = QString("<span style='color: #aaaaaa;'>%1: ← %2</span>")
                    .arg(QDateTime::currentMSecsSinceEpoch() - startTime)
                    .arg(QString(buffer));
    } else {
        logMessage = "<span style='color: red;'>Port is not opened</span>";
    }

    ui->logOutput->append(logMessage);
}

void MainWindow::serialReadData()
{
   QByteArray data = serialPort->readAll();
   charBuffer.append(data);

   strBuffer = QString(charBuffer);

   if (strBuffer.startsWith("!{") && strBuffer.endsWith("}!\n"))
   {
       qDebug() << "→ " + strBuffer;

       strBuffer = strBuffer.trimmed();

       QString logMessage = QString("<span style='color: #aaaaaa;'>%1:</span> → %2")
                           .arg(QDateTime::currentMSecsSinceEpoch() - startTime)
                           .arg(QString(strBuffer));
       ui->logOutput->append(logMessage);

       charBuffer = "";
       strBuffer = "";
   }

   if (strBuffer.length() > 1024) {
       strBuffer = "";
   }
}


void MainWindow::on_checkLinkButton_clicked()
{
    sendCommand("PING");
}

