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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsDropShadowEffect *startGlow = new QGraphicsDropShadowEffect(this);
    startGlow->setBlurRadius(25);          // glow strength
    startGlow->setColor(QColor(0, 255, 0)); // green glow
    startGlow->setOffset(0);               // centered glow

    QGraphicsDropShadowEffect *endGlow = new QGraphicsDropShadowEffect(this);
    endGlow->setBlurRadius(25);
    endGlow->setColor(QColor(255, 0, 0));   // red glow
    endGlow->setOffset(0);

    ui->endButton->setGraphicsEffect(endGlow);

    ui->startButton->setGraphicsEffect(startGlow);
    // ❌ REMOVE THIS (timer not needed anymore)
    // connect(&timer, &QTimer::timeout, this, &MainWindow::generateData);

    const char* geomFile = "C:\\Program Files\\Atracsys\\simulator SDK x64\\data\\geometry011.ini";

    int initStatus = Initialize(nullptr, geomFile);

    if (initStatus != 0) {
        qDebug() << "❌ Initialize failed:" << initStatus;
    } else {
        char deviceInfo[256];
        GetDeviceInfo(deviceInfo, sizeof(deviceInfo));
        qDebug() << "✅ Device connected:" << deviceInfo;
    }

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
    // ❌ REMOVE TIMER
    // timer.stop();

    // ❌ REMOVE THIS
    // StopTracking();

    // 🆕 STOP WORKER
    emit stopWorker();

    sessionManager.endSession();
}


// ❌ KEEP THIS FUNCTION BUT NOT USED (safe to keep)
void MainWindow::generateData()
{
    // Not used anymore (logic moved to worker thread)
}
