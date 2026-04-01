#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "sessionmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include "trackingworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_endButton_clicked();
    void generateData();  // simulate data
    void setupAtracsys();
    void on_initButton_clicked();
    void on_switch3DButton_clicked();
    void on_backButton_clicked();
    void on_captureButton_clicked();
    void on_init3DButton_clicked();


private:
    Ui::MainWindow *ui;
    SessionManager sessionManager;
    QTimer timer;
    QFile m_sessionFile;
    QTextStream m_stream;
    QString m_sessionFilePath;
    QThread *workerThread;
    TrackingWorker *worker;
    QTimer *bgAnimTimer;
    int glowAlpha = 60;
    bool glowIncreasing = true;

protected:
    void resizeEvent(QResizeEvent *event) override;
signals:
    void startWorker();
    void stopWorker();
};

#endif
