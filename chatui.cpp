#include "chatui.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

ChatUI::ChatUI(QWidget *parent) : QWidget(parent)
{
    QString IpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression IpRegex ("^" + IpRange
                                    + "(\\." + IpRange + ")"
                                    + "(\\." + IpRange + ")"
                                    + "(\\." + IpRange + ")$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(IpRegex, this);
    w_sendingAddress = new QLineEdit;
    w_sendingAddress->setValidator(ipValidator);

    w_sendingPort = new QSpinBox;
    w_sendingPort->setMinimum(0);
    w_sendingPort->setMaximum(65535);
    w_setSendingAddress = new QPushButton(QString("Set"));
    w_setSendingAddress->setEnabled(false);

    w_receivingAddress = new QLineEdit;
    w_receivingAddress->setValidator(ipValidator);

    w_receivingPort = new QSpinBox;
    w_receivingPort->setMinimum(0);
    w_receivingPort->setMaximum(65535);
    w_startReceiving = new QPushButton(QString("Start"));
    w_stopReceiving = new QPushButton(QString("Stop"));
    w_stopReceiving->setEnabled(false);

    w_receivedMessagesEdit = new QTextEdit;
    w_receivedMessagesEdit->setReadOnly(true);

    w_messageEdit = new QLineEdit;
    w_sendButton = new QPushButton(QString("Send"));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *topLayout = new QHBoxLayout;
    QHBoxLayout *midLayout = new QHBoxLayout;
    QHBoxLayout *botLayout = new QHBoxLayout;

    topLayout->addWidget(new QLabel(QString("IP Address")));
    topLayout->addWidget(w_sendingAddress);
    topLayout->addWidget(new QLabel(QString("Port")));
    topLayout->addWidget(w_sendingPort);
    topLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    topLayout->addWidget(w_setSendingAddress);

    midLayout->addWidget(new QLabel(QString("IP Address")));
    midLayout->addWidget(w_receivingAddress);
    midLayout->addWidget(new QLabel(QString("Port")));
    midLayout->addWidget(w_receivingPort);
    midLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    midLayout->addWidget(w_startReceiving);
    midLayout->addWidget(w_stopReceiving);

    botLayout->addWidget(new QLabel(QString("Message")));
    botLayout->addWidget(w_messageEdit);
    botLayout->addWidget(w_sendButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(midLayout);
    mainLayout->addWidget(w_receivedMessagesEdit);
    mainLayout->addLayout(botLayout);

    m_sendingUdpSocket = new QUdpSocket;

    connect(w_setSendingAddress, &QPushButton::clicked, this, &ChatUI::slotUiSetSendingSocketParameters);
    connect(w_sendButton, &QPushButton::clicked, this, &ChatUI::slotUiSendMessage);
    connect(w_sendingPort, QOverload<int> :: of(&QSpinBox :: valueChanged), this, [this]() -> void {w_setSendingAddress->setEnabled(true);});
    connect(w_sendingAddress, &QLineEdit :: textEdited, this, [this]()->void {w_setSendingAddress->setEnabled(true);});

    this->setLayout(mainLayout);

    m_udpReceiver = new UdpReceiver;
    connect(w_startReceiving, &QPushButton::clicked, this, &ChatUI::slotUiStartReceiving);
    connect(w_startReceiving, &QPushButton :: clicked, this, [this]()->void {w_startReceiving->setEnabled(false); w_stopReceiving->setEnabled(true);});
    connect(w_stopReceiving, &QPushButton::clicked, m_udpReceiver, &UdpReceiver::stopWorkingThread);
    connect(w_stopReceiving, &QPushButton :: clicked, this, [this]()->void {w_startReceiving->setEnabled(true); w_stopReceiving->setEnabled(false);});
    connect(m_udpReceiver, &UdpReceiver::signalMessageReceived, this, &ChatUI::slotUiReceivedMessage);

}

void ChatUI::slotUiSetSendingSocketParameters()
{
    m_sendingUdpSocket->abort();
    m_sendingUdpSocket->bind(QHostAddress(w_sendingAddress->text()), w_sendingPort->value());
    m_sendingUdpSocket->open(QUdpSocket::ReadWrite);
    w_setSendingAddress->setEnabled(false);
}

void ChatUI::slotUiSendMessage()
{
    QByteArray sendingDatagram;
    sendingDatagram.append(w_messageEdit->text().toLocal8Bit());
    m_sendingUdpSocket->writeDatagram(sendingDatagram, QHostAddress(w_sendingAddress->text()), w_sendingPort->value());
}

void ChatUI::slotUiStartReceiving()
{
    m_udpReceiver->startWorkingThread(w_receivingAddress->text(), w_receivingPort->value());
}

void ChatUI::slotUiReceivedMessage(const QString &message)
{
    w_receivedMessagesEdit->append(message);
}
