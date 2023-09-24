#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class Connector : public QObject
{
    Q_OBJECT

private:
    int port;
    QString host;
    QTcpSocket *p_socket;
    QTimer *p_reconnectTimer;
    bool connectionStatus;

    QByteArray tcpData;

private slots:
    void dataReady();
    void onConnected();
    void onDisconnected();
    void onReconnect();
    void onSocketError(QAbstractSocket::SocketError);

signals:
   void connected();
   void disconnected();
   void newData();

public:
    Connector(QString host, int port);
    bool isConnected();
    void write(QByteArray data);
    QByteArray read();
};

#endif // CONNECTOR_H
