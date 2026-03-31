#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include "AtracsysAPI.h"
#include <QDateTime>
#include <QDebug>

// 🆕 ADD THESE
#include <QThread>
#include "trackingworker.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setStyleSheet(
        "QMainWindow {"
        "background: qlineargradient("
        "x1:0, y1:0, x2:0, y2:1,"
        "stop:0 #111827,"
        "stop:1 #334155"
        ");"
        "}"
        );

    ui->statusLabel->setStyleSheet(
        "QLabel {"
        "color: #87CEFA;"
        "font-size: 22px;"
        "font-weight: bold;"
        "padding: 8px;"
        "border-radius: 8px;"
        "}"
        "QLabel:hover {"
        "color: #00FFFF;"
        "background-color: rgba(0, 255, 255, 30);"
        "}"
        );

    ui->startButton->setStyleSheet(
        "QPushButton {"
        "background-color: #4CAF50;"
        "border-radius: 12px;"
        "color: white;"
        "font-size: 14px;"
        "font-weight: bold;"
        "padding: 10px 20px;"
        "min-width: 160px;"
        "min-height: 45px;"
        "border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "background-color: #66BB6A;"
        "border: 2px solid #A5D6A7;"
        "padding: 11px 21px;"
        "}"
        "QPushButton:pressed {"
        "background-color: #388E3C;"
        "border: 2px solid #C8E6C9;"
        "}"
        );

    ui->initButton->setStyleSheet(
        "QPushButton {"
        "background-color: #2196F3;"
        "border-radius: 12px;"
        "color: white;"
        "font-size: 14px;"
        "font-weight: bold;"
        "padding: 10px 20px;"
        "min-width: 160px;"
        "min-height: 45px;"
        "border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "background-color: #42A5F5;"
        "border: 2px solid #90CAF9;"
        "padding: 11px 21px;"
        "}"
        "QPushButton:pressed {"
        "background-color: #1976D2;"
        "border: 2px solid #BBDEFB;"
        "}"
        );

    ui->endButton->setStyleSheet(
        "QPushButton {"
        "background-color: #F44336;"
        "border-radius: 12px;"
        "color: white;"
        "font-size: 14px;"
        "font-weight: bold;"
        "padding: 10px 20px;"
        "min-width: 160px;"
        "min-height: 45px;"
        "border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "background-color: #EF5350;"
        "border: 2px solid #EF9A9A;"
        "padding: 11px 21px;"
        "}"
        "QPushButton:pressed {"
        "background-color: #D32F2F;"
        "border: 2px solid #FFCDD2;"
        "}"
        );


    auto titleGlow = new QGraphicsDropShadowEffect(this);
    titleGlow->setBlurRadius(20);
    titleGlow->setOffset(0, 0);
    titleGlow->setColor(QColor(0, 255, 255, 180));
    ui->statusLabel->setGraphicsEffect(titleGlow);


    auto blueGlow = new QGraphicsDropShadowEffect(this);
    blueGlow->setBlurRadius(25);
    blueGlow->setOffset(0, 0);
    blueGlow->setColor(QColor(0, 180, 255, 180));
    ui->initButton->setGraphicsEffect(blueGlow);

    // Green
    auto greenGlow = new QGraphicsDropShadowEffect(this);
    greenGlow->setBlurRadius(25);
    greenGlow->setOffset(0, 0);
    greenGlow->setColor(QColor(0, 255, 100, 180));
    ui->startButton->setGraphicsEffect(greenGlow);

    // Red
    auto redGlow = new QGraphicsDropShadowEffect(this);
    redGlow->setBlurRadius(25);
    redGlow->setOffset(0, 0);
    redGlow->setColor(QColor(255, 60, 60, 180));
    ui->endButton->setGraphicsEffect(redGlow);
    // ❌ REMOVE THIS (timer not needed anymore)
    // connect(&timer, &QTimer::timeout, this, &MainWindow::generateData);
    // ==============================
    // ✨ STARTUP ANIMATION
    // ==============================

    // Fade title
    auto titleEffect = new QGraphicsOpacityEffect(this);
    ui->statusLabel->setGraphicsEffect(titleEffect);

    auto titleFade = new QPropertyAnimation(titleEffect, "opacity");
    titleFade->setDuration(800);
    titleFade->setStartValue(0);
    titleFade->setEndValue(1);

    // Slide buttons
    auto initAnim = new QPropertyAnimation(ui->initButton, "pos");
    initAnim->setDuration(600);
    initAnim->setStartValue(ui->initButton->pos() + QPoint(0, 30));
    initAnim->setEndValue(ui->initButton->pos());

    auto startAnim = new QPropertyAnimation(ui->startButton, "pos");
    startAnim->setDuration(700);
    startAnim->setStartValue(ui->startButton->pos() + QPoint(0, 30));
    startAnim->setEndValue(ui->startButton->pos());

    auto endAnim = new QPropertyAnimation(ui->endButton, "pos");
    endAnim->setDuration(800);
    endAnim->setStartValue(ui->endButton->pos() + QPoint(0, 30));
    endAnim->setEndValue(ui->endButton->pos());

    // Run together
    auto group = new QParallelAnimationGroup(this);
    group->addAnimation(titleFade);
    group->addAnimation(initAnim);
    group->addAnimation(startAnim);
    group->addAnimation(endAnim);

    group->start();
    // ==============================
    // 🆕 THREAD SETUP START
    // ==============================

    workerThread = new QThread(this);
    worker = new TrackingWorker();

    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // connect signals
    connect(this, &MainWindow::startWorker, worker, &TrackingWorker::startTracking);
    connect(this, &MainWindow::stopWorker, worker, &TrackingWorker::stopTracking);

    // receive data from worker
    connect(worker, &TrackingWorker::newData, this, [this](QString line) {
        qDebug() << line;
        sessionManager.addData(line);
    });

    // error handling
    connect(worker, &TrackingWorker::error, this, [](QString msg) {
        qDebug() << msg;
    });

    workerThread->start();

    // ==============================
    // 🆕 THREAD SETUP END
    // ==============================
}

MainWindow::~MainWindow()
{
    // ❌ REMOVE TIMER
    // timer.stop();

    // 🆕 STOP WORKER SAFELY
    emit stopWorker();

    workerThread->quit();
    workerThread->wait();

    StopTracking();
    Cleanup();

    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    sessionManager.startSession();

    // ❌ REMOVE THIS
    // int status = StartTracking();

    // ❌ REMOVE TIMER
    // timer.start(33);

    // 🆕 START WORKER
    emit startWorker();
}

void MainWindow::on_endButton_clicked()
{
    emit stopWorker();

    sessionManager.endSession();

    StopTracking();
    Cleanup();   // 🔥 proper shutdown
    // 🔁 Allow re-initialization
    ui->initButton->setEnabled(true);
    ui->startButton->setEnabled(false);
}


// ❌ KEEP THIS FUNCTION BUT NOT USED (safe to keep)
void MainWindow::generateData()
{
    // Not used anymore (logic moved to worker thread)
}
void MainWindow::setupAtracsys()
{
    const char* geomFile = "C:\\Program Files\\Atracsys\\simulator SDK x64\\data\\geometry011.ini";

    int initStatus = Initialize(nullptr, geomFile);

    if (initStatus != 0) {
        qDebug() << "❌ Initialize failed:" << initStatus;
    } else {
        char deviceInfo[256];
        GetDeviceInfo(deviceInfo, sizeof(deviceInfo));
        qDebug() << "✅ Device connected:" << deviceInfo;
    }
}
void MainWindow::on_initButton_clicked()
{
    setupAtracsys();
    // 🔥 Disable after init
    ui->initButton->setEnabled(false);

    // Optional: enable start button
    ui->startButton->setEnabled(true);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    int centerX = width() / 2;
    int startY = height() / 3;

    // Title
    ui->statusLabel->move(centerX - ui->statusLabel->width() / 2, startY - 100);

    // Initialize button
    ui->initButton->move(centerX - ui->initButton->width() / 2, startY);

    // Start button
    ui->startButton->move(centerX - ui->startButton->width() / 2, startY + 70);

    // End button
    ui->endButton->move(centerX - ui->endButton->width() / 2, startY + 140);
}
