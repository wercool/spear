/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *refreshPortListpushButton;
    QComboBox *serialPortsComboBox;
    QLabel *label_2;
    QComboBox *serialPortBaudRateComboBox;
    QPushButton *openPortButton;
    QPushButton *closePortButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *clearLogOutputPushButton;
    QSplitter *splitter;
    QTextEdit *logOutput;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QPushButton *checkLinkButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(870, 599);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8("spear.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setMaximumSize(QSize(16777215, 16777215));
        centralwidget->setLayoutDirection(Qt::LeftToRight);
        centralwidget->setAutoFillBackground(false);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        refreshPortListpushButton = new QPushButton(centralwidget);
        refreshPortListpushButton->setObjectName(QString::fromUtf8("refreshPortListpushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(refreshPortListpushButton->sizePolicy().hasHeightForWidth());
        refreshPortListpushButton->setSizePolicy(sizePolicy1);
        refreshPortListpushButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(refreshPortListpushButton);

        serialPortsComboBox = new QComboBox(centralwidget);
        serialPortsComboBox->setObjectName(QString::fromUtf8("serialPortsComboBox"));
        serialPortsComboBox->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(serialPortsComboBox);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        serialPortBaudRateComboBox = new QComboBox(centralwidget);
        serialPortBaudRateComboBox->setObjectName(QString::fromUtf8("serialPortBaudRateComboBox"));
        serialPortBaudRateComboBox->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(serialPortBaudRateComboBox);

        openPortButton = new QPushButton(centralwidget);
        openPortButton->setObjectName(QString::fromUtf8("openPortButton"));

        horizontalLayout->addWidget(openPortButton);

        closePortButton = new QPushButton(centralwidget);
        closePortButton->setObjectName(QString::fromUtf8("closePortButton"));

        horizontalLayout->addWidget(closePortButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        clearLogOutputPushButton = new QPushButton(centralwidget);
        clearLogOutputPushButton->setObjectName(QString::fromUtf8("clearLogOutputPushButton"));

        horizontalLayout->addWidget(clearLogOutputPushButton);


        verticalLayout->addLayout(horizontalLayout);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setMinimumSize(QSize(0, 0));
        splitter->setOrientation(Qt::Vertical);
        logOutput = new QTextEdit(splitter);
        logOutput->setObjectName(QString::fromUtf8("logOutput"));
        sizePolicy.setHeightForWidth(logOutput->sizePolicy().hasHeightForWidth());
        logOutput->setSizePolicy(sizePolicy);
        logOutput->setMinimumSize(QSize(0, 30));
        logOutput->setMaximumSize(QSize(16777215, 200));
        logOutput->setAutoFillBackground(false);
        logOutput->setStyleSheet(QString::fromUtf8(""));
        logOutput->setReadOnly(true);
        splitter->addWidget(logOutput);
        scrollArea = new QScrollArea(splitter);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setLineWidth(0);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setEnabled(true);
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 852, 343));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        scrollAreaWidgetContents->setMinimumSize(QSize(0, 0));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, -1, -1);
        checkLinkButton = new QPushButton(scrollAreaWidgetContents);
        checkLinkButton->setObjectName(QString::fromUtf8("checkLinkButton"));
        checkLinkButton->setMinimumSize(QSize(0, 0));
        checkLinkButton->setStyleSheet(QString::fromUtf8("font: 10pt \"Ubuntu\";"));
        checkLinkButton->setCheckable(false);
        checkLinkButton->setAutoRepeat(true);
        checkLinkButton->setAutoRepeatDelay(250);
        checkLinkButton->setFlat(false);

        verticalLayout_3->addWidget(checkLinkButton);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        scrollArea->setWidget(scrollAreaWidgetContents);
        splitter->addWidget(scrollArea);

        verticalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        checkLinkButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Spear Control", nullptr));
        label->setText(QApplication::translate("MainWindow", "Port", nullptr));
        refreshPortListpushButton->setText(QApplication::translate("MainWindow", "\342\237\263", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Baud Rate", nullptr));
        openPortButton->setText(QApplication::translate("MainWindow", "Open", nullptr));
        closePortButton->setText(QApplication::translate("MainWindow", "Close", nullptr));
        clearLogOutputPushButton->setText(QApplication::translate("MainWindow", "Clear", nullptr));
        checkLinkButton->setText(QApplication::translate("MainWindow", "\342\207\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
