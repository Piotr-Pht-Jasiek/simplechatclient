/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include "defines.h"
class QTimer;
class QTcpSocket;
#include <QAbstractSocket>
#include <QMutex>
#include <QThread>

/**
 * Network thread class
 */
class Network : public QThread
{
    Q_OBJECT
public:
    Network(const QString &_strServer, int _iPort);
    virtual ~Network();
    void run();
    bool isConnected();
    QTimer *timerReconnect;

public slots:
    void connect();
    void disconnect();
    void send(const QString &strData);

private:
    QString strServer;
    int iPort;
    QTcpSocket *socket;
    QTimer *timerPong;
    QTimer *timerPing;
    qint64 iActive;
    QList<QString> msgSendQueue;
    QHash<qint64, NetworkStats> msgSendHistory;
    QMutex mutex;
    bool bAuthorized;

    void authorize();
    void clearAll();
    void write(const QString &strData);

private slots:
    void connected();
    void disconnected();
    void reconnect();
    void recv();
    void error(QAbstractSocket::SocketError error);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void timeoutPong();
    void timeoutPing();
    void sendQueue();

signals:
    void socketStateChanged();
    void kernel(const QString &strData);
    void authorize(QString strNick, QString strPass);
    void updateNick(const QString &strNick);
};

#endif // NETWORK_H
