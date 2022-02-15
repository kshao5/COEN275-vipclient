#include "TcpClient.h"
#include <iostream>
TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{
    connect(&_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(&_socket, &QTcpSocket::errorOccurred, this, &TcpClient::onErrorOccurred);
    // when server sends client some data
    connect(&_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}


void TcpClient::connectToHost(const QString &ip, const QString &port)
{
    _socket.connectToHost(ip, port.toUInt());
}

void TcpClient::sendMessage(const QString &message)
{
    _socket.write(message.toUtf8());
    need_cryption_ = false;//initialize cryption state
    _socket.flush();
    emit newMessage("YOU: "+message.toUtf8());
}

void TcpClient::onConnected()
{
    qInfo() << "Connected to host";
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qWarning() << "Error:" << error;
}

QString TcpClient::decrypt(const QString &message) {
    std::string text = message.toUtf8().constData();
    for (size_t i = 0; i != text.size(); i++) {
        text[i] -= 2;
    }
    return QString::fromUtf8(text.c_str());
}

void TcpClient::onReadyRead()
{
    const auto message = _socket.readAll();
    // if (!need_cryption_) {//the message that client send to server don't need to be decrypted
    //     emit newMessage(message);
    //     need_cryption_ = true;
    //     return;
    // }
    QString msg = QString(message);
    QString output = decrypt(msg);//only decrypt the message server send to client
    qInfo() << "54";
    emit newMessage(output.toUtf8());
}
