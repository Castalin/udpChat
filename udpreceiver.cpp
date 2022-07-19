#include "udpreceiver.h"
#include <QtNetwork>

UdpReceiver::UdpReceiver(QObject *parent) : QThread(parent)
{
    m_mutex = new QMutex;
}

void UdpReceiver::run()
{
    if (m_workingThreadEnable) {
        emit poolStatus(true);
    }

    QByteArray receivedMessage;

    m_receivingSocket = new QUdpSocket;
    m_receivingSocket->abort();
    m_receivingSocket->bind(QHostAddress(m_address), m_port);
    m_receivingSocket->open(QUdpSocket::ReadOnly);

    while (m_workingThreadEnable) {
        receivedMessage.clear();
        if (m_receivingSocket->state() == QUdpSocket::BoundState) {
            receivedMessage = m_receivingSocket->receiveDatagram().data();
        }
        if (!receivedMessage.isEmpty()) {
            QString message(receivedMessage);
            emit signalMessageReceived(message);
        }
        usleep(10);
    }

    delete m_receivingSocket;

    m_mutex->lock();
    m_workingThreadEnable = false;
    emit poolStatus(false);
    m_mutex->unlock();
}

bool UdpReceiver::isWorking()
{
    bool workingStatus;
    m_mutex->lock();
    workingStatus = m_workingThreadEnable;
    m_mutex->unlock();
    return workingStatus;
}

void UdpReceiver::startWorkingThread(const QString &address, const int &port)
{
    if (!isWorking()) {
        m_address = address;
        m_port = port;
        m_workingThreadEnable = true;
        start();
    }
}

void UdpReceiver::stopWorkingThread()
{
    if (isWorking()) {
        m_mutex->lock();
        m_workingThreadEnable = false;
        m_mutex->unlock();
    }
}
