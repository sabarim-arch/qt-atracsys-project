#pragma once

#include <QObject>
#include <QTimer>
#include "AtracsysAPI.h"

class TrackingWorker : public QObject
{
    Q_OBJECT

public:
    explicit TrackingWorker(QObject *parent = nullptr);

public slots:
    void startTracking();
    void stopTracking();

signals:
    void newData(QString line);
    void error(QString msg);

private:
    QTimer *m_timer;
    bool m_running = false;
};
