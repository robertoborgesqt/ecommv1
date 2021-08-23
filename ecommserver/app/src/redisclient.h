#ifndef REDISCLIENT_H
#define REDISCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QQueue>

#include "command.h"

using namespace Redis;

class RedisClient : public QObject
{
    Q_OBJECT

public:
    explicit RedisClient(QObject *parent = 0);

    void connectToServer(QString hostName, quint16 port = 6379);
    void disconnectFromServer();
    void sendCommand(Command *command);

private:
    QTcpSocket connection;
    QQueue<Command *> sentCommands;
    bool subscribing;

    void initConnectionHandler();
    void sendRequest(QVariantList lines);

private slots:
    void handleConnectionError(QAbstractSocket::SocketError socketError);
    void clearState();
    void parseReply();

signals:
    void connected();
    void disconnected();
    void messageReceived(QString channel, QVariant payload);
};

#endif // REDISCLIENT_H
