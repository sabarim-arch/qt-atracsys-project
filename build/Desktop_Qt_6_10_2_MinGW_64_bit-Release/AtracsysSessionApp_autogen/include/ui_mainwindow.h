/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startButton;
    QPushButton *endButton;
    QLabel *statusLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(773, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: #2c3e50;"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(270, 190, 161, 61));
        startButton->setStyleSheet(QString::fromUtf8("#startButton {\n"
"    background-color: #4CAF50;\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 12px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* \360\237\224\245 Glow on hover */\n"
"#startButton:hover {\n"
"    background-color: #45a049;\n"
"    border: 2px solid #66ff66;\n"
"}"));
        endButton = new QPushButton(centralwidget);
        endButton->setObjectName("endButton");
        endButton->setGeometry(QRect(270, 270, 161, 61));
        endButton->setStyleSheet(QString::fromUtf8("#endButton {\n"
"    background-color: #e53935;\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 12px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* \360\237\224\245 Glow on hover */\n"
"#endButton:hover {\n"
"    background-color: #c62828;\n"
"    border: 2px solid #ff6666;\n"
"}"));
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(290, 20, 231, 41));
        statusLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #e2e8f0;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    background: transparent;\n"
"}"));
        statusLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 773, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start Session", nullptr));
        endButton->setText(QCoreApplication::translate("MainWindow", "End Session", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Atracsys Tracking", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
