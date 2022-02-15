#ifndef VIPCLIENT_H
#define VIPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "TcpClient.h"
#include <vector>
using namespace std;


class VipClient : public TcpClient
{
    Q_OBJECT
public:
    explicit VipClient(QObject *parent = nullptr);

signals:
    void newMessage(const QByteArray &ba);

public slots:
    void connectToHost(const QString &ip, const QString &port);
    void sendMessage(const QString &message);

private:
    QTcpSocket _socket;
    bool need_cryption_;


public:
    QString decrypt(const QString &message);
    vector<string> chatHistory;

public slots:
    void saveChat();

private slots:
    void onReadyRead();
};

#endif // VIPCLIENT_H
