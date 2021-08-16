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
    chart->addAxis(magnetomerYValueAxis, Qt::AlignLeft);
    chart->addAxis(magnetomerZValueAxis, Qt::AlignLeft);
    chart->addAxis(accXValueAxis, Qt::AlignLeft);
    chart->addAxis(accYValueAxis, Qt::AlignLeft);
    chart->addAxis(accZValueAxis, Qt::AlignLeft);

    chart->addSeries(barometerSeries);
    chart->addSeries(magnetomerXSeries);
    chart->addSeries(magnetomerYSeries);
    chart->addSeries(magnetomerZSeries);
    chart->addSeries(accXSeries);
    chart->addSeries(accYSeries);
    chart->addSeries(accZSeries);

    QPen barometerSeriesPen = barometerSeries->pen();
    barometerSeriesPen.setWidth(3);
    barometerSeriesPen.setBrush(QBrush("red"));
    barometerSeries->setPen(barometerSeriesPen);
//    barometerValueAxis->setTitleText("barometer");
    barometerValueAxis->setTickCount(3);
    barometerValueAxis->setLabelsAngle(-90);
    barometerValueAxis->setLinePenColor(barometerSeries->pen().color());
    barometerSeries->attachAxis(timeAxis);
    barometerSeries->attachAxis(barometerValueAxis);
    barometerSeries->setName("barometer");
    barometerSeries->setUseOpenGL(true);

    //magnetomerXValueAxis->setTitleText("magnetometer X");
    magnetomerXValueAxis->setTickCount(3);
    magnetomerXValueAxis->setLabelsAngle(-90);
    magnetomerXValueAxis->setLinePenColor(magnetomerXSeries->pen().color());
    magnetomerXSeries->attachAxis(timeAxis);
    magnetomerXSeries->attachAxis(magnetomerXValueAxis);
    magnetomerXSeries->setName("magX");
    magnetomerXSeries->setUseOpenGL(true);

    //magnetomerYValueAxis->setTitleText("magnetometer Y");
    magnetomerYValueAxis->setTickCount(3);
    magnetomerYValueAxis->setLabelsAngle(-90);
    magnetomerYValueAxis->setLinePenColor(magnetomerYSeries->pen().color());
    magnetomerYSeries->attachAxis(timeAxis);
    magnetomerYSeries->attachAxis(magnetomerYValueAxis);
    magnetomerYSeries->setName("magY");
    magnetomerYSeries->setUseOpenGL(true);

    QPen magnetomerZSeriesPen = magnetomerZSeries->pen();
    magnetomerZSeriesPen.setWidth(3);
    magnetomerZSeriesPen.setBrush(QBrush("blue"));
    magnetomerZSeries->setPen(magnetomerZSeriesPen);
    //magnetomerZValueAxis->setTitleText("magnetometer Z");
    magnetomerZValueAxis->setTickCount(3);
    magnetomerZValueAxis->setLabelsAngle(-90);
    magnetomerZValueAxis->setLinePenColor(magnetomerZSeries->pen().color());
    magnetomerZSeries->attachAxis(timeAxis);
    magnetomerZSeries->attachAxis(magnetomerZValueAxis);
    magnetomerZSeries->setName("magZ");
    magnetomerZSeries->setUseOpenGL(true);

    //accXValueAxis->setTitleText("Accelerometer X");
    accXValueAxis->setTickCount(3);
    accXValueAxis->setLabelsAngle(-90);
    accXValueAxis->setLinePenColor(accXSeries->pen().color());
    accXSeries->attachAxis(timeAxis);
    accXSeries->attachAxis(accXValueAxis);
    accXSeries->setName("accX");
    accXSeries->setUseOpenGL(true);

    //accYValueAxis->setTitleText("Accelerometer Y");
    accYValueAxis->setTickCount(3);
    accYValueAxis->setLabelsAngle(-90);
    accYValueAxis->setLinePenColor(accYSeries->pen().color());
    accYSeries->attachAxis(timeAxis);
    accYSeries->attachAxis(accYValueAxis);
    accYSeries->setName("accY");
    accYSeries->setUseOpenGL(true);

    QPen accZSeriesPen = accZSeries->pen();
    accZSeriesPen.setWidth(3);
    accZSeriesPen.setBrush(QBrush("orange"));
    accZSeries->setPen(accZSeriesPen);
    //accZValueAxis->setTitleText("Accelerometer Z");
    accZValueAxis->setTickCount(3);
    accZValueAxis->setLabelsAngle(-90);
    accZValueAxis->setLinePenColor(accZSeries->pen().color());
    accZSeries->attachAxis(timeAxis);
    accZSeries->attachAxis(accZValueAxis);
    accZSeries->setName("accZ");
    accZSeries->setUseOpenGL(true);
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

                   /*
                    * Barometer
                    */
                   float barometerReading = messageParts[2].toFloat();
                   barometerReadings.append(QPointF(messageTime, barometerReading));
                   double maxBarometerReading = 0;
                   double minBarometerReading = 0;
                   foreach (QPointF p, barometerReadings) {
                       if (p.y() > maxBarometerReading) {
                           maxBarometerReading = p.y();
                       }
                       if (p.y() < minBarometerReading) {
                           minBarometerReading = p.y();
                       }
                   }
                   barometerValueAxis->setRange(minBarometerReading, maxBarometerReading);
                   barometerSeries->replace(barometerReadings);

                   /*
                    * Magnetometer
                    */
                   int magXReading = messageParts[3].toInt();
                   int magYReading = messageParts[4].toInt();
                   int magZReading = messageParts[5].toInt();

                   magnetomerXReadings.append(QPointF(messageTime, magXReading));
                   double maxMagnetomerXReading = 0;
                   double minMagnetomerXReading = 0;
                   foreach (QPointF p, magnetomerXReadings) {
                       if (p.y() > maxMagnetomerXReading) {
                           maxMagnetomerXReading = p.y();
                       }
                       if (p.y() < minMagnetomerXReading) {
                           minMagnetomerXReading = p.y();
                       }
                   }
                   magnetomerXValueAxis->setRange(minMagnetomerXReading, maxMagnetomerXReading);
                   magnetomerXSeries->replace(magnetomerXReadings);

                   magnetomerYReadings.append(QPointF(messageTime, magYReading));
                   double maxMagnetomerYReading = 0;
                   double minMagnetomerYReading = 0;
                   foreach (QPointF p, magnetomerYReadings) {
                       if (p.y() > maxMagnetomerYReading) {
                           maxMagnetomerYReading = p.y();
                       }
                       if (p.y() < minMagnetomerYReading) {
                           minMagnetomerYReading = p.y();
                       }
                   }
                   magnetomerYValueAxis->setRange(minMagnetomerYReading, maxMagnetomerYReading);
                   magnetomerYSeries->replace(magnetomerYReadings);

                   magnetomerZReadings.append(QPointF(messageTime, magZReading));
                   double maxMagnetomerZReading = 0;
                   double minMagnetomerZReading = 0;
                   foreach (QPointF p, magnetomerZReadings) {
                       if (p.y() > maxMagnetomerZReading) {
                           maxMagnetomerZReading = p.y();
                       }
                       if (p.y() < minMagnetomerZReading) {
                           minMagnetomerZReading = p.y();
                       }
                   }
                   magnetomerZValueAxis->setRange(minMagnetomerZReading, maxMagnetomerZReading);
                   magnetomerZSeries->replace(magnetomerZReadings);

                   /*
                    * Accelerometer
                    */
                   int accXReading = messageParts[6].toInt();
                   int accYReading = messageParts[7].toInt();
                   int accZReading = messageParts[8].toInt();

                   accXReadings.append(QPointF(messageTime, accXReading));
                   double maxAccXReading = 0;
                   double minAccXReading = 0;
                   foreach (QPointF p, accXReadings) {
                       if (p.y() > maxAccXReading) {
                           maxAccXReading = p.y();
                       }
                       if (p.y() < minAccXReading) {
                           minAccXReading = p.y();
                       }
                   }
                   accXValueAxis->setRange(minAccXReading, maxAccXReading);
                   accXSeries->replace(accXReadings);

                   accYReadings.append(QPointF(messageTime, accYReading));
                   double maxAccYReading = 0;
                   double minAccYReading = 0;
                   foreach (QPointF p, accYReadings) {
                       if (p.y() > maxAccYReading) {
                           maxAccYReading = p.y();
                       }
                       if (p.y() < minAccYReading) {
                           minAccYReading = p.y();
                       }
                   }
                   accYValueAxis->setRange(minAccYReading, maxAccYReading);
                   accYSeries->replace(accYReadings);

                   accZReadings.append(QPointF(messageTime, accZReading));
                   double maxAccZReading = 0;
                   double minAccZReading = 0;
                   foreach (QPointF p, accZReadings) {
                       if (p.y() > maxAccZReading) {
                           maxAccZReading = p.y();
                       }
                       if (p.y() < minAccZReading) {
                           minAccZReading = p.y();
                       }
                   }
                   accZValueAxis->setRange(minAccZReading, maxAccZReading);
                   accZSeries->replace(accZReadings);


                   /*
                    * Time asix
                    */
                   timeAxis->setRange(barometerReadings.first().x(), barometerReadings.last().x());
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

