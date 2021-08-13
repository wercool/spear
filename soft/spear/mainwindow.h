#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>
#include <QCloseEvent>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace QtCharts;
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

    void on_refreshPortListpushButton_clicked();

    void on_checkLinkButton_clicked();

    void closeEvent(QCloseEvent *event);

    void serialReadData();

    void checkLinkTimeout();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QMessageBox msgBox;

    QString selectedPort;
    int selectedPortBaudRate;
    bool portOpened = false;
    QByteArray charBuffer;
    QString strBuffer;

    void sendCommand(QString command);

    /**
     * @brief messageRegExp
     * Spear send telemetry messages in <s>.*</s> format
     */
    QRegularExpression messageRegExp = QRegularExpression("<c>(.*?)</c>", QRegularExpression::MultilineOption);

    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QChart *chart = new QChart;
    QChartView *chartView = new QChartView(chart);
    QValueAxis *timeAxis = new QValueAxis;

    QLineSeries *barometerSeries = new QLineSeries;
    QVector<QPointF> barometerReadings;
    QValueAxis *barometerValueAxis = new QValueAxis;

    QLineSeries *magnetomerXSeries = new QLineSeries;
    QVector<QPointF> magnetomerXReadings;
    QValueAxis *magnetomerXValueAxis = new QValueAxis;

    /*
     * Commands
    */
    QTimer* checkLinkTimer = new QTimer(this);

    /*
     * Messages
     */
    int lastMessageTime = -1;
};
#endif // MAINWINDOW_H
