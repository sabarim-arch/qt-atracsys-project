#include "sessionmanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

SessionManager::SessionManager(QObject *parent)
    : QObject(parent), m_isRunning(false)
{
}

void SessionManager::startSession()
{
    m_data.clear();
    m_isRunning = true;
    qDebug() << "Session started";
}

void SessionManager::addData(const QString &line)
{
    if (!m_isRunning)
        return;

    SessionData data;
    data.timestamp = QDateTime::currentDateTime();
    data.line = line;

    m_data.append(data);
}

void SessionManager::endSession()
{
    if (!m_isRunning)
        return;

    m_isRunning = false;

    // 📁 Create folder
    QString folderPath = QDir::currentPath() + "/Saved Video";
    QDir().mkpath(folderPath);

    // 📄 File name
    QString fileName = folderPath + "/session_" +
                       QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";

    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        for (const auto &d : m_data) {
            out << d.line << "\n";
        }

        file.close();
        qDebug() << "Saved file at:" << fileName;
    }

    qDebug() << "Session ended and saved";
}
