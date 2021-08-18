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
    chart->addAxis(gyrXValueAxis, Qt::AlignLeft);
    chart->addAxis(gyrYValueAxis, Qt::AlignLeft);
    chart->addAxis(gyrZValueAxis, Qt::AlignLeft);

    chart->addSeries(barometerSeries);
    chart->addSeries(magnetomerXSeries);
    chart->addSeries(magnetomerYSeries);
    chart->addSeries(magnetomerZSeries);
    chart->addSeries(accXSeries);
    chart->addSeries(accYSeries);
    chart->addSeries(accZSeries);
    chart->addSeries(gyrXSeries);
    chart->addSeries(gyrYSeries);
    chart->addSeries(gyrZSeries);

//    QPen barometerSeriesPen = barometerSeries->pen();
//    barometerSeriesPen.setWidth(3);
//    barometerSeriesPen.setBrush(QBrush("red"));
//    barometerSeries->setPen(barometerSeriesPen);
//    barometerValueAxis->setTitleText("barometer");
    barometerValueAxis->setTickCount(3);
    barometerValueAxis->setLabelsAngle(-90);
    barometerValueAxis->setLinePenColor(barometerSeries->pen().color());
    barometerSeries->attachAxis(timeAxis);
    barometerSeries->attachAxis(barometerValueAxis);
    barometerSeries->setName("barometer, m");
    barometerSeries->setUseOpenGL(true);

    //magnetomerXValueAxis->setTitleText("magnetometer X");
    magnetomerXValueAxis->setTickCount(3);
    magnetomerXValueAxis->setLabelsAngle(-90);
    magnetomerXValueAxis->setLinePenColor(magnetomerXSeries->pen().color());
    magnetomerXSeries->attachAxis(timeAxis);
    magnetomerXSeries->attachAxis(magnetomerXValueAxis);
    magnetomerXSeries->setName("magX");
    magnetomerXSeries->setUseOpenGL(true);
    if (!ui->magXCheckBox->isChecked()) {
        magnetomerXSeries->hide();
        magnetomerXValueAxis->hide();
    }

    //magnetomerYValueAxis->setTitleText("magnetometer Y");
    magnetomerYValueAxis->setTickCount(3);
    magnetomerYValueAxis->setLabelsAngle(-90);
    magnetomerYValueAxis->setLinePenColor(magnetomerYSeries->pen().color());
    magnetomerYSeries->attachAxis(timeAxis);
    magnetomerYSeries->attachAxis(magnetomerYValueAxis);
    magnetomerYSeries->setName("magY");
    magnetomerYSeries->setUseOpenGL(true);
    if (!ui->magYCheckBox->isChecked()) {
        magnetomerYSeries->hide();
        magnetomerYValueAxis->hide();
    }

//    QPen magnetomerZSeriesPen = magnetomerZSeries->pen();
//    magnetomerZSeriesPen.setWidth(3);
//    magnetomerZSeriesPen.setBrush(QBrush("blue"));
//    magnetomerZSeries->setPen(magnetomerZSeriesPen);
    //magnetomerZValueAxis->setTitleText("magnetometer Z");
    magnetomerZValueAxis->setTickCount(3);
    magnetomerZValueAxis->setLabelsAngle(-90);
    magnetomerZValueAxis->setLinePenColor(magnetomerZSeries->pen().color());
    magnetomerZSeries->attachAxis(timeAxis);
    magnetomerZSeries->attachAxis(magnetomerZValueAxis);
    magnetomerZSeries->setName("magZ");
    magnetomerZSeries->setUseOpenGL(true);
    if (!ui->magZCheckBox->isChecked()) {
        magnetomerZSeries->hide();
        magnetomerZValueAxis->hide();
    }

    //accXValueAxis->setTitleText("Accelerometer X");
    accXValueAxis->setTickCount(3);
    accXValueAxis->setLabelsAngle(-90);
    accXValueAxis->setLinePenColor(accXSeries->pen().color());
    accXSeries->attachAxis(timeAxis);
    accXSeries->attachAxis(accXValueAxis);
    accXSeries->setName("accX, g");
    accXSeries->setUseOpenGL(true);
    if (!ui->accXCheckBox->isChecked()) {
        accXSeries->hide();
        accXValueAxis->hide();
    }

    //accYValueAxis->setTitleText("Accelerometer Y");
    accYValueAxis->setTickCount(3);
    accYValueAxis->setLabelsAngle(-90);
    accYValueAxis->setLinePenColor(accYSeries->pen().color());
    accYSeries->attachAxis(timeAxis);
    accYSeries->attachAxis(accYValueAxis);
    accYSeries->setName("accY, g");
    accYSeries->setUseOpenGL(true);
    if (!ui->accYCheckBox->isChecked()) {
        accYSeries->hide();
        accYValueAxis->hide();
    }

//    QPen accZSeriesPen = accZSeries->pen();
//    accZSeriesPen.setWidth(3);
//    accZSeriesPen.setBrush(QBrush("orange"));
//    accZSeries->setPen(accZSeriesPen);
    //accZValueAxis->setTitleText("Accelerometer Z");
    accZValueAxis->setTickCount(3);
    accZValueAxis->setLabelsAngle(-90);
    accZValueAxis->setLinePenColor(accZSeries->pen().color());
    accZSeries->attachAxis(timeAxis);
    accZSeries->attachAxis(accZValueAxis);
    accZSeries->setName("accZ, g");
    accZSeries->setUseOpenGL(true);
    if (!ui->accZCheckBox->isChecked()) {
        accZSeries->hide();
        accZValueAxis->hide();
    }

    //gyrXValueAxis->setTitleText("Gyroscope X");
    gyrXValueAxis->setTickCount(3);
    gyrXValueAxis->setLabelsAngle(-90);
    gyrXValueAxis->setLinePenColor(gyrXSeries->pen().color());
    gyrXSeries->attachAxis(timeAxis);
    gyrXSeries->attachAxis(gyrXValueAxis);
    gyrXSeries->setName("gyrX, °/s");
    gyrXSeries->setUseOpenGL(true);
    if (!ui->gyrXCheckBox->isChecked()) {
        gyrXSeries->hide();
        gyrXValueAxis->hide();
    }

    //gyrYValueAxis->setTitleText("Gyroscope Y");
    gyrYValueAxis->setTickCount(3);
    gyrYValueAxis->setLabelsAngle(-90);
    gyrYValueAxis->setLinePenColor(gyrYSeries->pen().color());
    gyrYSeries->attachAxis(timeAxis);
    gyrYSeries->attachAxis(gyrYValueAxis);
    gyrYSeries->setName("gyrY, °/s");
    gyrYSeries->setUseOpenGL(true);
    if (!ui->gyrYCheckBox->isChecked()) {
        gyrYSeries->hide();
        gyrYValueAxis->hide();
    }

//    QPen gyrZSeriesPen = accZSeries->pen();
//    gyrZSeriesPen.setWidth(3);
//    gyrZSeriesPen.setBrush(QBrush("pink"));
//    gyrZSeries->setPen(gyrZSeriesPen);
    //gyrZValueAxis->setTitleText("Gyroscope Z");
    gyrZValueAxis->setTickCount(3);
    gyrZValueAxis->setLabelsAngle(-90);
    gyrZValueAxis->setLinePenColor(gyrZSeries->pen().color());
    gyrZSeries->attachAxis(timeAxis);
    gyrZSeries->attachAxis(gyrZValueAxis);
    gyrZSeries->setName("gyrZ, °/s");
    gyrZSeries->setUseOpenGL(true);
    if (!ui->gyrZCheckBox->isChecked()) {
        gyrZSeries->hide();
        gyrZValueAxis->hide();
    }
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
        QDateTime date = QDateTime::currentDateTime();
        QString formattedTime = date.toString("dd-MM-yyyy-hh-mm-ss");
        logFilename = QString("/home/maska/Documents/spear-log-%1.txt").arg(formattedTime);
        logFile.setFileName(logFilename);
        if (!logFile.open(QIODevice::ReadWrite) )
        {
            qDebug() << "Error openeing log file";
            logFile.close();
        } else {
            logFileStream.setDevice(&logFile);
        }
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
        if (logFile.isOpen()) {
            logFileStream.flush();
            logFile.close();
        }
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

           if (logFile.isOpen()) {
               logFileStream << message + "\n";
           }

           QStringList messageParts = message.split(',');

           unsigned long messageTime = messageParts[0].toULong();
           QString curMode;
           if (messageParts.length() > 1) {
               curMode = messageParts[1];
           }

           /*
            * Process message
            */
           if (curMode == "F" || curMode == "C") {
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
                   float accXReading = messageParts[6].toFloat() / 4096;
                   float accYReading = messageParts[7].toFloat() / 4096;
                   float accZReading = messageParts[8].toFloat() / 4096;

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
                    * Gyroscope
                    */
                   float gyrXReading = messageParts[9].toFloat() / 32.8;
                   float gyrYReading = messageParts[10].toFloat() / 32.8;
                   float gyrZReading = messageParts[11].toFloat() / 32.8;

                   gyrXReadings.append(QPointF(messageTime, gyrXReading));
                   double maxGyrXReading = 0;
                   double minGyrXReading = 0;
                   foreach (QPointF p, gyrXReadings) {
                       if (p.y() > maxGyrXReading) {
                           maxGyrXReading = p.y();
                       }
                       if (p.y() < minGyrXReading) {
                           minGyrXReading = p.y();
                       }
                   }
                   gyrXValueAxis->setRange(minGyrXReading, maxGyrXReading);
                   gyrXSeries->replace(gyrXReadings);

                   gyrYReadings.append(QPointF(messageTime, gyrYReading));
                   double maxGyrYReading = 0;
                   double minGyrYReading = 0;
                   foreach (QPointF p, gyrYReadings) {
                       if (p.y() > maxGyrYReading) {
                           maxGyrYReading = p.y();
                       }
                       if (p.y() < minGyrYReading) {
                           minGyrYReading = p.y();
                       }
                   }
                   gyrYValueAxis->setRange(minGyrYReading, maxGyrYReading);
                   gyrYSeries->replace(gyrYReadings);

                   gyrZReadings.append(QPointF(messageTime, gyrZReading));
                   double maxGyrZReading = 0;
                   double minGyrZReading = 0;
                   foreach (QPointF p, gyrZReadings) {
                       if (p.y() > maxGyrZReading) {
                           maxGyrZReading = p.y();
                       }
                       if (p.y() < minGyrZReading) {
                           minGyrZReading = p.y();
                       }
                   }
                   gyrZValueAxis->setRange(minGyrZReading, maxGyrZReading);
                   gyrZSeries->replace(gyrZReadings);

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


   if (charBuffer.length() > 1024) {
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


void MainWindow::on_magXCheckBox_clicked(bool checked)
{
    if (checked ) {
        magnetomerXSeries->show();
        magnetomerXValueAxis->show();
    } else {
        magnetomerXSeries->hide();
        magnetomerXValueAxis->hide();
    }
}


void MainWindow::on_magYCheckBox_clicked(bool checked)
{
    if (checked) {
        magnetomerYSeries->show();
        magnetomerYValueAxis->show();
    } else {
        magnetomerYSeries->hide();
        magnetomerYValueAxis->hide();
    }
}


void MainWindow::on_magZCheckBox_clicked(bool checked)
{
    if (checked) {
        magnetomerZSeries->show();
        magnetomerZValueAxis->show();
    } else {
        magnetomerZSeries->hide();
        magnetomerZValueAxis->hide();
    }
}


void MainWindow::on_accXCheckBox_clicked(bool checked)
{
    if (checked) {
        accXSeries->show();
        accXValueAxis->show();
    } else {
        accXSeries->hide();
        accXValueAxis->hide();
    }
}


void MainWindow::on_accYCheckBox_clicked(bool checked)
{
    if (checked) {
        accYSeries->show();
        accYValueAxis->show();
    } else {
        accYSeries->hide();
        accYValueAxis->hide();
    }
}


void MainWindow::on_accZCheckBox_clicked(bool checked)
{
    if (checked) {
        accZSeries->show();
        accZValueAxis->show();
    } else {
        accZSeries->hide();
        accZValueAxis->hide();
    }
}


void MainWindow::on_gyrXCheckBox_clicked(bool checked)
{
    if (checked) {
        gyrXSeries->show();
        gyrXValueAxis->show();
    } else {
        gyrXSeries->hide();
        gyrXValueAxis->hide();
    }
}


void MainWindow::on_gyrYCheckBox_clicked(bool checked)
{
    if (checked) {
        gyrYSeries->show();
        gyrYValueAxis->show();
    } else {
        gyrYSeries->hide();
        gyrYValueAxis->hide();
    }
}


void MainWindow::on_gyrZCheckBox_clicked(bool checked)
{
    if (checked) {
        gyrZSeries->show();
        gyrZValueAxis->show();
    } else {
        gyrZSeries->hide();
        gyrZValueAxis->hide();
    }
}


void MainWindow::on_barometerCheckBox_clicked(bool checked)
{
    if (checked) {
        barometerSeries->show();
        barometerValueAxis->show();
    } else {
        barometerSeries->hide();
        barometerValueAxis->hide();
    }
}

