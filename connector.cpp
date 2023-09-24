#include "connector.h"

Connector::Connector(QString host, int port)
{
    this->host = host;
    this->port = port;

    p_socket = new QTcpSocket(this);
    connect(p_socket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(p_socket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(p_socket, SIGNAL(readyRead()), this, SLOT(dataReady()));
    connect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)));

    p_reconnectTimer = new QTimer(this);
    connect(p_reconnectTimer,SIGNAL(timeout()), this, SLOT(onReconnect()));
    p_reconnectTimer->setInterval(1000);
    p_reconnectTimer->start();

    p_socket->connectToHost(host, port);
}

void Connector::dataReady()
{
    tcpData = p_socket->readAll();
    if(tcpData.startsWith("OK MPD"))
    {
        connectionStatus = true;
        emit connected();
    }
    else
        emit newData();
}
void Connector::onConnected()
{
    qDebug("Server connected");
}
void Connector::onDisconnected()
{
    connectionStatus = false;
    emit disconnected();

    qDebug("Server disconnected");
}
void Connector::onReconnect()
{
    if(!p_socket->isOpen())
        p_socket->connectToHost(host, port);
}

void Connector::onSocketError(QAbstractSocket::SocketError)
{
    qDebug("Socket error");
    p_socket->close();
}

bool Connector::isConnected()
{
    return connectionStatus;
}

void Connector::write(QByteArray data)
{
    p_socket->write(data);
}

QByteArray Connector::read()
{
    return tcpData;
}
