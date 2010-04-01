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

#ifndef DLG_CAM_H
#define DLG_CAM_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QHideEvent>
#include <QSettings>
#include <QShowEvent>
#include <QTcpSocket>
#include <QTimer>
#include "ui_cam.h"

class dlg_cam : public QDialog
{
    Q_OBJECT
public:
    dlg_cam(QSettings *, QTcpSocket *);
    ~dlg_cam();
    void set_nick(QString);

private:
    Ui::uiCam ui;
    QSettings *settings;
    QTcpSocket *irc_socket;
    QString strNick;
    QTcpSocket *socket;
    QString strDataRecv;
    QTimer *timer;

    void show_img(QByteArray);
    void network_connect();
    void network_send(QString);
    void network_disconnect();
    void send(QString);

private slots:
    void button_ok();
    void network_read();
    void network_connected();
    void network_keepalive();
    void network_disconnected();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

};

#endif // DLG_CAM_H
