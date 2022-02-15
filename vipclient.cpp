#include "VipClient.h"
#include <fstream>
#include <iostream>
using namespace std;

VipClient::VipClient(QObject *parent)
    :TcpClient(parent)
{
    connect(&_socket, &QTcpSocket::connected, this, &VipClient::onConnected);
    connect(&_socket, &QTcpSocket::errorOccurred, this, &VipClient::onErrorOccurred);
    // when server sends client some data
    connect(&_socket, &QTcpSocket::readyRead, this, &VipClient::onReadyRead);
}

void VipClient::connectToHost(const QString &ip, const QString &port)
{
    _socket.connectToHost(ip, port.toUInt());
}

void VipClient::sendMessage(const QString &message)
{
    _socket.write(message.toUtf8());
    need_cryption_ = false;//initialize cryption state
    _socket.flush();
    chatHistory.push_back("YOU: " + message.toStdString());
    emit newMessage("YOU: "+message.toUtf8());
}


QString VipClient::decrypt(const QString &message) {
    std::string text = message.toUtf8().constData();
    for (size_t i = 0; i != text.size(); i++) {
        text[i] -= 2;
    }
    return QString::fromUtf8(text.c_str());
}

void VipClient::onReadyRead()
{
    const auto message = _socket.readAll();
    // if (!need_cryption_) {//the message that client send to server don't need to be decrypted
    //     emit newMessage(message);
    //     need_cryption_ = true;
    //     return;
    // }
    QString msg = QString(message);
    QString output = decrypt(msg);//only decrypt the message server send to client
    chatHistory.push_back(output.toStdString());
    qInfo() << "chat history has size of " << VipClient::chatHistory.size();
    emit newMessage(output.toUtf8());
}



void VipClient::saveChat(){
    ofstream output;
    output.open("../output.txt");
    for (int i = 0; i < chatHistory.size(); i++) {
        std::string currentString = chatHistory[i];
        output << chatHistory[i] + "\n";
    }
    output.close();
}


