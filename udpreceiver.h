#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QWidget>
#include <QUdpSocket>
#include <QThread>
#include <QMutex>

class UdpReceiver : public QThread
{
    Q_OBJECT

    QMutex *m_mutex;
    bool m_workingThreadEnable;

    QUdpSocket *m_receivingSocket;
    QString m_address;
    int m_port;

public:
    explicit UdpReceiver(QObject *parent = nullptr);

private:

    void run() override;
    bool isWorking();

public slots:
    void startWorkingThread(const QString &address, const int &port);
    void stopWorkingThread();

signals:

    void poolStatus(const bool &threadStart);
    void signalMessageReceived(const QString &message);
};

#endif // UDPRECEIVER_H
