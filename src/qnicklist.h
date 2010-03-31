/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr �uczko <piotr.luczko@gmail.com>               *
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

#ifndef QNICKLIST_H
#define QNICKLIST_H

#include <QContextMenuEvent>
#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QListWidget>
#include <QMenu>
#include <QSettings>
#include <QTcpSocket>

class qnicklist : public QListWidget
{
    Q_OBJECT
public:
    qnicklist(QTcpSocket *, QSettings *, QString);
    void set_open_channels(QStringList);

private:
    QTcpSocket *socket;
    QSettings *settings;
    QString strChannel;
    QStringList strOpenChannels;
    enum { MaxOpenChannels = 50 };
    QAction *openChannelsActs[MaxOpenChannels];

    void send(QString);

private slots:
    void priv();
    void whois();
    void friends_add();
    void friends_del();
    void ignore_add();
    void ignore_del();
    void kick();
    void ban();
    void kban();
    void op_add();
    void op_del();
    void halfop_add();
    void halfop_del();
    void moderator_add();
    void moderator_del();
    void voice_add();
    void voice_del();
    void invite();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);

};

#endif // QNICKLIST_H
