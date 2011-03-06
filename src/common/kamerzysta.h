/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 ****************************************************************************/

#ifndef KAMERZYSTA_H
#define KAMERZYSTA_H

class Config;
class Log;
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

/**
 * Cams support using Kamerzysta (http://programy.onet.pl/72,34,10195,,,Kamerzysta_3.00.159,programy.html)
 */
class Kamerzysta : public QObject
{
    Q_OBJECT
public:
    Kamerzysta(QTcpSocket *);
    void show(QString);
    void close();

private:
    QString strNick;
    QString strAppPath;
    QString strKamerzystaFile;
    int iPort;
    QTcpSocket *socket;
    int iTryGetPort;
    QTimer *timerGetPort;

    void log(QString);
    void get_path();
    void authorize();
    void kamerzysta_not_running();
    void kamerzysta_running();

private slots:
    void get_port();
    void network_connect();
    void network_connected();
    void network_disconnect();
    void network_disconnected();
    void network_send(QString);
    void network_read();
    void error(QAbstractSocket::SocketError);
};

#endif // KAMERZYSTA_H
