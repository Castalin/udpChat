#ifndef CHATUI_H
#define CHATUI_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QUdpSocket>

#include "udpreceiver.h"

class ChatUI : public QWidget
{
    Q_OBJECT
public:
    explicit ChatUI(QWidget *parent = nullptr);

private:

    QLineEdit *w_sendingAddress;
    QSpinBox *w_sendingPort;
    QPushButton *w_setSendingAddress;

    QLineEdit *w_receivingAddress;
    QSpinBox *w_receivingPort;
    QPushButton *w_startReceiving;
    QPushButton *w_stopReceiving;

    QLineEdit *w_messageEdit;
    QPushButton *w_sendButton;

    QTextEdit *w_receivedMessagesEdit;

    QUdpSocket *m_sendingUdpSocket;

    UdpReceiver *m_udpReceiver;

private slots:

    void slotUiSetSendingSocketParameters();
    void slotUiSendMessage();
    void slotUiStartReceiving();
    void slotUiReceivedMessage(const QString &message);

signals:

};

#endif // CHATUI_H
