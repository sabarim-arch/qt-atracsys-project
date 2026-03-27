#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QVector>
#include <QDateTime>

struct SessionData {
    QDateTime timestamp;
    QString line;
};

class SessionManager : public QObject
{
    Q_OBJECT

public:
    explicit SessionManager(QObject *parent = nullptr);

    void startSession();
    void addData(const QString &line);
    void endSession();

private:
    QVector<SessionData> m_data;
    bool m_isRunning;
};

#endif
