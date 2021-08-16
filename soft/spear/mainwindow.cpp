#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    msgBox.setWindowTitle("Spear Control");
    startConfirmBox.setWindowTitle("Confirm LAUNCH");

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
    connect(repeatCommandTimer, SIGNAL(timeout()), this, SLOT(repeatCommandTimerTimeout()));

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);
    ui->graphVerticalLayout->addWidget(chartView);

    QSizePolicy chartViewSizePolicy;
    chartViewSizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    chartViewSizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    chartView->setSizePolicy(chartViewSizePolicy);

//    QLineSeries *magnetomerXSeries = new QLineSeries;
//    QVector<QPointF> magnetomerXReadings;
//    QValueAxis *magnetomerXValueAxis = new QValueAxis;

    timeAxis->setLabelFormat("%d");
    timeAxis->setTickCount(10);
    chart->addAxis(timeAxis, Qt::AlignBottom);
    chart->addAxis(barometerValueAxis, Qt::AlignLeft);
    chart->addAxis(magnetomerXValueAxis, Qt::AlignLeft);

    chart->addSeries(barometerSeries);
    chart->addSeries(magnetomerXSeries);

//    barometerValueAxis->setTitleText("barometer");
    barometerValueAxis->setTickCount(3);
    barometerValueAxis->setLabelsAngle(-90);
    barometerValueAxis->setLinePenColor(barometerSeries->pen().color());
    barometerSeries->attachAxis(timeAxis);
    barometerSeries->attachAxis(barometerValueAxis);
    barometerSeries->setName("barometer");
    barometerSeries->setUseOpenGL(true);

//    magnetomerXValueAxis->setTitleText("magnetometer Z");
    magnetomerXValueAxis->setTickCount(3);
    magnetomerXValueAxis->setLabelsAngle(-90);
    magnetomerXValueAxis->setLinePenColor(magnetomerXSeries->pen().color());
    magnetomerXSeries->attachAxis(timeAxis);
    magnetomerXSeries->attachAxis(magnetomerXValueAxis);
    magnetomerXSeries->setName("magnetometer Z");
    magnetomerXSeries->setUseOpenGL(true);
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
        QByteArray buffer = "<c>" + QByteArray::fromStdString(command.toStdString()) + "</c>";
        serialPort->write(buffer);

        logMessage = QString("<span style='color: #aaaaaa;'>← %2</span>").arg(QString(buffer));
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
//    qDebug() << strBuffer;

    QRegularExpressionMatchIterator messageMatchIterator = messageRegExp.globalMatch(strBuffer);
    while (messageMatchIterator.hasNext()) {
       QRegularExpressionMatch messageMatch = messageMatchIterator.next();
       if (messageMatch.hasMatch()) {
           QString rawMessage = messageMatch.captured(0);
           QString message = rawMessage.mid(3, rawMessage.length() - 7);
           QStringList messageParts = message.split(',');

           unsigned long messageTime = messageParts[0].toULong();
           QString isInFligh;
           if (messageParts.length() > 1) {
               isInFligh = messageParts[1];
           }

           /*
            * Process message
            */
           if (isInFligh == "F") {
               if (messageTime > lastMessageTime) {
                   lastMessageTime = messageTime;
                   float barometerReading = messageParts[2].toFloat();

                   barometerReadings.append(QPointF(messageTime, barometerReading));

                   double maxReadingsTime = barometerReadings.last().x();

                   timeAxis->setRange(0, maxReadingsTime);

                   double maxBarometerReading = 0;
                   foreach (QPointF p, barometerReadings) {
                       if (p.y() > maxBarometerReading) {
                           maxBarometerReading = p.y();
                       }
                   }
                   barometerValueAxis->setRange(0, maxBarometerReading);

                   barometerSeries->replace(barometerReadings);
               }
           }

           /*
           * Message log output, after processing
           */
           QString logMessage = QString("→ %2").arg(QString(message));

           ui->logOutput->append(logMessage);
       }
    }


   if (charBuffer.length() > 64) {
       charBuffer = "";
       strBuffer = "";
   }
}

void MainWindow::repeatCommandTimerTimeout() {
    if (repeatCommand.length() > 0 && repeatCommandNum > 0) {
        if (repeatCommandCnt < repeatCommandNum) {
            sendCommand(repeatCommand);
            repeatCommandCnt++;
        } else {
            repeatCommandTimer->stop();
            repeatCommandCnt = 0;
            repeatCommandNum = 0;
            repeatCommand = "";
        }
    }
}


void MainWindow::on_checkLinkButton_clicked()
{
    QString pingMessage = QString("PING");
    sendCommand(pingMessage);
}


void MainWindow::on_launchButton_clicked()
{
    startConfirmBox.setText("Initiate LAUNCH?");
    startConfirmBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    startConfirmBox.setDefaultButton(QMessageBox::Cancel);
    int desicion = startConfirmBox.exec();

    if (desicion == QMessageBox::Ok) {
        repeatCommand = QString("START");
        repeatCommandNum = 100;
        repeatCommandTimer->setInterval(10);
        repeatCommandTimer->start();
    }
}


void MainWindow::on_stopButton_clicked()
{
    repeatCommand = QString("STOP");
    repeatCommandNum = 500;
    repeatCommandTimer->setInterval(25);
    repeatCommandTimer->start();
}

