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
#include "ThreeDWindow.h"
#include <QDir>
#include <QStandardPaths>
#include <QScreen>
#include <QApplication>
#include <QDateTime>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusLabel->hide();
    ui->threeDViewLabel->hide();
    ui->backButton->hide();
    ui->captureButton->hide();
    ui->init3DButton->hide();
    ui->captureButton->setEnabled(false);
    ui->init3DButton->setFixedSize(180, 50);
    ui->captureButton->setFixedSize(180, 50);

    ui->init3DButton->setStyleSheet(
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

    ui->captureButton->setStyleSheet(
        "QPushButton {"
        "background-color: #F59E0B;"
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
        "background-color: #FBBF24;"
        "border: 2px solid #FCD34D;"
        "padding: 11px 21px;"
        "}"
        "QPushButton:pressed {"
        "background-color: #D97706;"
        "border: 2px solid #FDE68A;"
        "}"
        );

    auto blueGlow3D = new QGraphicsDropShadowEffect(this);
    blueGlow3D->setBlurRadius(25);
    blueGlow3D->setOffset(0, 0);
    blueGlow3D->setColor(QColor(0, 180, 255, 180));
    ui->init3DButton->setGraphicsEffect(blueGlow3D);

    auto orangeGlow = new QGraphicsDropShadowEffect(this);
    orangeGlow->setBlurRadius(25);
    orangeGlow->setOffset(0, 0);
    orangeGlow->setColor(QColor(255, 180, 0, 180));
    ui->captureButton->setGraphicsEffect(orangeGlow);

    ui->captureButton->setStyleSheet(
        "QPushButton {"
        "background-color: #F59E0B;"
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
        "background-color: #FBBF24;"
        "border: 2px solid #FCD34D;"
        "}"
        "QPushButton:pressed {"
        "background-color: #D97706;"
        "}"
        );

    ui->backButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(255, 255, 255, 20);"
        "border-radius: 10px;"
        "color: white;"
        "font-size: 13px;"
        "font-weight: bold;"
        "padding: 8px 16px;"
        "border: 1px solid rgba(255,255,255,40);"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 255, 255, 40);"
        "border: 1px solid #00FFFF;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(255, 255, 255, 60);"
        "}"
        );

    ui->switch3DButton->setStyleSheet(
        "QPushButton {"
        "background-color: #6366F1;"
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
        "background-color: #818CF8;"
        "border: 2px solid #A5B4FC;"
        "padding: 11px 21px;"
        "}"
        "QPushButton:pressed {"
        "background-color: #4F46E5;"
        "border: 2px solid #C7D2FE;"
        "}"
        );

    auto purpleGlow = new QGraphicsDropShadowEffect(this);
    purpleGlow->setBlurRadius(25);
    purpleGlow->setOffset(0, 0);
    purpleGlow->setColor(QColor(120, 100, 255, 180));
    ui->switch3DButton->setGraphicsEffect(purpleGlow);


    ui->threeDViewLabel->setStyleSheet(
        "QLabel {"
        "color: white;"
        "font-size: 42px;"
        "font-weight: 700;"
        "font-family: 'Segoe UI';"
        "background: transparent;"
        "}"
        );

    ui->threeDViewLabel->setAlignment(Qt::AlignCenter);
    ui->threeDViewLabel->hide();


    auto bgAnimation = new QVariantAnimation(this);
    bgAnimation->setDuration(3000);
    bgAnimation->setStartValue(0);
    bgAnimation->setEndValue(100);
    bgAnimation->setLoopCount(-1);
    bgAnimation->setEasingCurve(QEasingCurve::InOutSine);

    connect(bgAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
        int glow = value.toInt();

        this->setStyleSheet(QString(
                                "QMainWindow {"
                                "background: qradialgradient("
                                "cx:0.5, cy:0.35, radius:0.9,"
                                "fx:0.5, fy:0.35,"
                                "stop:0 rgba(0, %1, 255, 120),"
                                "stop:1 #111827"
                                ");"
                                "}"
                                ).arg(80 + glow));
    });

    bgAnimation->start();

    QPixmap logo("C:/Users/sabar/OneDrive/Desktop/AtracsysSessionApp/images/atracsys_logo.png");

    ui->logoLabel->setPixmap(
        logo.scaled(520, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );

    ui->logoLabel->setFixedSize(520, 220);
    ui->logoLabel->setAlignment(Qt::AlignCenter);

    auto logoGlow = new QGraphicsDropShadowEffect(this);
    logoGlow->setBlurRadius(25);
    logoGlow->setOffset(0, 0);
    logoGlow->setColor(QColor(0, 255, 255, 150));

    ui->logoLabel->setGraphicsEffect(logoGlow);

    ui->initButton->hide();
    ui->startButton->hide();
    ui->endButton->hide();
    ui->switch3DButton->hide();

    QTimer::singleShot(400, this, [this]() {
        ui->initButton->show();
    });

    QTimer::singleShot(700, this, [this]() {
        ui->startButton->show();
    });

    QTimer::singleShot(1000, this, [this]() {
        ui->endButton->show();
    });

    QTimer::singleShot(1300, this, [this]() {
        ui->switch3DButton->show();
    });



    ui->statusLabel->setStyleSheet(
        "QLabel {"
        "color: #E2E8F0;"
        "font-size: 30px;"
        "font-weight: 700;"
        "font-family: 'Segoe UI';"
        "background: transparent;"
        "border: none;"
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

    auto glowPulse = new QPropertyAnimation(titleGlow, "blurRadius");
    glowPulse->setDuration(1500);
    glowPulse->setStartValue(15);
    glowPulse->setEndValue(30);
    glowPulse->setLoopCount(-1);
    glowPulse->setEasingCurve(QEasingCurve::InOutSine);
    glowPulse->start();


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
    titleFade->setDuration(1200);
    titleFade->setStartValue(0);
    titleFade->setEndValue(1);
    titleFade->setEasingCurve(QEasingCurve::OutCubic);

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
    // Button state control
    ui->startButton->setEnabled(false);
    ui->endButton->setEnabled(true);

    // Disable 3D switch while session is active
    ui->switch3DButton->setEnabled(false);
}

void MainWindow::on_endButton_clicked()
{
    emit stopWorker();

    sessionManager.endSession();

    StopTracking();

    // Allow start again
    ui->startButton->setEnabled(true);
    ui->endButton->setEnabled(false);

    // Allow switch to 3D again
    ui->switch3DButton->setEnabled(true);
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
    ui->startButton->setEnabled(true);
    ui->endButton->setEnabled(false);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // ===== Screen center =====
    int centerX = width() / 2;
    int centerY = height() / 2;

    // ===== Responsive scale =====
    double scale = width() / 900.0;
    if (scale < 1.0)
        scale = 1.0;

    // ===== Dynamic sizes =====
    int btnWidth  = 180 * scale;
    int btnHeight = 50 * scale;
    int gap       = 70 * scale;

    // ===== Apply button scaling =====
    ui->initButton->setFixedSize(btnWidth, btnHeight);
    ui->startButton->setFixedSize(btnWidth, btnHeight);
    ui->endButton->setFixedSize(btnWidth, btnHeight);
    ui->switch3DButton->setFixedSize(btnWidth, btnHeight);

    ui->init3DButton->setFixedSize(btnWidth, btnHeight);
    ui->captureButton->setFixedSize(btnWidth, btnHeight);

    // ===== Scale logo =====
    int logoW = 520 * scale;
    int logoH = 220 * scale;

    QPixmap logo("C:/Users/sabar/OneDrive/Desktop/AtracsysSessionApp/images/atracsys_logo.png");

    ui->logoLabel->setPixmap(
        logo.scaled(logoW, logoH,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation)
        );

    ui->logoLabel->setFixedSize(logoW, logoH);

    // ================= HOME PAGE =================
    int startY = height() / 3 + (20 * scale);

    ui->logoLabel->move(
        centerX - ui->logoLabel->width() / 2,
        startY - (240 * scale)
        );

    ui->initButton->move(
        centerX - btnWidth / 2,
        startY
        );

    ui->startButton->move(
        centerX - btnWidth / 2,
        startY + gap
        );

    ui->endButton->move(
        centerX - btnWidth / 2,
        startY + gap * 2
        );

    ui->switch3DButton->move(
        centerX - btnWidth / 2,
        startY + gap * 3
        );

    // ================= 3D PAGE =================
    ui->backButton->move(20, 20);

    ui->init3DButton->move(
        centerX - btnWidth / 2,
        160 * scale
        );

    ui->captureButton->move(
        centerX - btnWidth / 2,
        240 * scale
        );
}

void MainWindow::on_switch3DButton_clicked()
{
    // Hide home widgets
    ui->logoLabel->hide();
    ui->initButton->hide();
    ui->startButton->hide();
    ui->endButton->hide();
    ui->switch3DButton->hide();

    // Show 3D widgets
    // ui->threeDViewLabel->show();
    ui->backButton->show();
    ui->backButton->raise();
    ui->captureButton->show();
    ui->init3DButton->show();

    // Refresh clean layout
    resizeEvent(nullptr);
}

void MainWindow::on_backButton_clicked()
{
    // Hide 3D screen widgets
    // ui->threeDViewLabel->hide();
    ui->backButton->hide();

    // Show home page widgets
    ui->logoLabel->show();
    ui->initButton->show();
    ui->startButton->show();
    ui->endButton->show();
    ui->switch3DButton->show();
    ui->captureButton->hide();
    ui->init3DButton->hide();

    // Refresh positions
    resizeEvent(nullptr);
}

void MainWindow::on_captureButton_clicked()
{
    QString basePath =
        "C:/Users/sabar/OneDrive/Desktop/AtracsysSessionApp/build/Desktop_Qt_6_10_2_MinGW_64_bit-Release";

    QString mainFolder = basePath + "/3D Saved Video";
    QString imageFolder = mainFolder + "/Images";
    QString videoFolder = mainFolder + "/Videos";

    QDir().mkpath(imageFolder);
    QDir().mkpath(videoFolder);

    QString timeStamp = QDateTime::currentDateTime()
                            .toString("yyyyMMdd_hhmmss");

    QString fileName = imageFolder + "/capture_" + timeStamp + ".png";

    QPixmap screenshot = this->grab();

    bool saved = screenshot.save(fileName);

    if (saved)
        qDebug() << "Saved successfully:" << fileName;
    else
        qDebug() << "Save failed";
}
void MainWindow::on_init3DButton_clicked()
{
    qDebug() << "3D initialized successfully";

    // Disable after first click
    ui->init3DButton->setEnabled(false);

    // Allow capture forever
    ui->captureButton->setEnabled(true);
}
