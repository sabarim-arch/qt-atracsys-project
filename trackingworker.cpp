#include "trackingworker.h"
#include <QDateTime>
#include <QDebug>

TrackingWorker::TrackingWorker(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, [this]() {

        const int MAX_MARKERS = 10;
        MarkerData markers[MAX_MARKERS];
        int markersRead = 0;

        int status = GetTrackingData(markers, MAX_MARKERS, &markersRead);

        if (status != 0) {
            emit error(QString("Tracking error: %1").arg(status));
            return;
        }

        if (markersRead == 0)
            return;

        for (int i = 0; i < markersRead; ++i) {

            QString line = QString(
                               "%1 | ID:%2\n"
                               "Pos  → X:%3  Y:%4  Z:%5\n"
                               "Rot  → [%6 %7 %8 | %9 %10 %11 | %12 %13 %14]\n"
                               "Err  → %15\n"
                               "-----------------------------"
                               )
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                               .arg(markers[i].geometryId)

                               .arg(markers[i].tx)
                               .arg(markers[i].ty)
                               .arg(markers[i].tz)

                               .arg(markers[i].r00)
                               .arg(markers[i].r01)
                               .arg(markers[i].r02)

                               .arg(markers[i].r10)
                               .arg(markers[i].r11)
                               .arg(markers[i].r12)

                               .arg(markers[i].r20)
                               .arg(markers[i].r21)
                               .arg(markers[i].r22)

                               .arg(markers[i].registrationError);

            emit newData(line);
        }
    });
}

void TrackingWorker::startTracking()
{
    if (m_running)
        return;

    int status = StartTracking();

    if (status != 0) {
        emit error(QString("StartTracking failed: %1").arg(status));
        return;
    }

    m_running = true;
    m_timer->start(33);

    qDebug() << "Worker: Tracking started";
}

void TrackingWorker::stopTracking()
{
    if (!m_running)
        return;

    m_timer->stop();
    StopTracking();
    m_running = false;

    qDebug() << "Worker: Tracking stopped";
}
