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

    void repeatCommandTimerTimeout();

    void serialReadData();

    void on_launchButton_clicked();

    void on_stopButton_clicked();

    void on_magXCheckBox_clicked(bool checked);

    void on_magYCheckBox_clicked(bool checked);

    void on_accXCheckBox_clicked(bool checked);

    void on_accYCheckBox_clicked(bool checked);

    void on_gyrXCheckBox_clicked(bool checked);

    void on_gyrYCheckBox_clicked(bool checked);

    void on_barometerCheckBox_clicked(bool checked);

    void on_magZCheckBox_clicked(bool checked);

    void on_accZCheckBox_clicked(bool checked);

    void on_gyrZCheckBox_clicked(bool checked);

    void on_openLogPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QMessageBox msgBox;
    QMessageBox startConfirmBox;

    QString selectedPort;
    int selectedPortBaudRate;
    bool portOpened = false;
    QByteArray charBuffer;
    QString strBuffer;

    QString logFilename;
    QFile logFile;
    QTextStream logFileStream;

    void sendCommand(QString command);

    /**
     * @brief messageRegExp
     * Spear send telemetry messages in <m>.*</m> format
     */
    QRegularExpression messageRegExp = QRegularExpression("<m>(.*?)</m>", QRegularExpression::MultilineOption);

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

    QLineSeries *magnetomerYSeries = new QLineSeries;
    QVector<QPointF> magnetomerYReadings;
    QValueAxis *magnetomerYValueAxis = new QValueAxis;

    QLineSeries *magnetomerZSeries = new QLineSeries;
    QVector<QPointF> magnetomerZReadings;
    QValueAxis *magnetomerZValueAxis = new QValueAxis;

    QLineSeries *accXSeries = new QLineSeries;
    QVector<QPointF> accXReadings;
    QValueAxis *accXValueAxis = new QValueAxis;

    QLineSeries *accYSeries = new QLineSeries;
    QVector<QPointF> accYReadings;
    QValueAxis *accYValueAxis = new QValueAxis;

    QLineSeries *accZSeries = new QLineSeries;
    QVector<QPointF> accZReadings;
    QValueAxis *accZValueAxis = new QValueAxis;


    QLineSeries *gyrXSeries = new QLineSeries;
    QVector<QPointF> gyrXReadings;
    QValueAxis *gyrXValueAxis = new QValueAxis;

    QLineSeries *gyrYSeries = new QLineSeries;
    QVector<QPointF> gyrYReadings;
    QValueAxis *gyrYValueAxis = new QValueAxis;

    QLineSeries *gyrZSeries = new QLineSeries;
    QVector<QPointF> gyrZReadings;
    QValueAxis *gyrZValueAxis = new QValueAxis;
    /*
     * Commands
    */
    QTimer* repeatCommandTimer = new QTimer(this);
    QString repeatCommand = "";
    int repeatCommandNum = 0;
    int repeatCommandCnt = 0;

    /*
     * Messages
     */
    unsigned long lastMessageTime = 0;
    bool processMessage(QString message);
};
#endif // MAINWINDOW_H
