#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

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

public slots:
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private slots:
    void onReadyRead();
};

#endif // TCPCLIENT_H
