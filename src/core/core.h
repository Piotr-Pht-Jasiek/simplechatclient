/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef CORE_H
#define CORE_H

#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QTabWidget>
#include <QToolBar>
#include "dlg_channel_favourites.h"
#include "dlg_channel_homes.h"
#include "dlg_channel_list.h"
#include "dlg_channel_settings.h"
#include "dlg_friends.h"
#include "dlg_ignore.h"
#include "dlg_moderation.h"
#include "inputlinedockwidget.h"
#ifdef Q_WS_WIN
#include "kamerzysta.h"
#endif
#include "network.h"
#include "nicklistdelegate.h"
#include "nicklistwidget.h"
#include "nicklistdockwidget.h"
#include "onet_auth.h"
#include "onet_kernel.h"
#include "tab_container.h"
#include "tab_manager.h"

class Core : public QObject
{
    Q_OBJECT
public:
    Core(QMainWindow *, QString, int, Notify *, QAction *, QToolBar *);
    ~Core();
    void refresh_colors();
    // temp
    void network_connect();
    void network_disconnect();
    void network_send(QString);
    bool network_is_connected();
    // onet dialogs
    void open_channel_list();
    void open_channel_homes();
    void open_channel_favourites();
    void open_friends();
    void open_ignore();
    void open_cams();
    // nicklist
    bool nicklist_exist(QString, QString);

public slots:
    void set_lag(QString);
    void create_nicklist(QString);
    void remove_nicklist(QString);
    void add_user(QString, QString, QString, QString);
    void del_user(QString, QString);
    void nicklist_refresh(QString);
    void quit_user(QString,QString);
    void change_flag(QString, QString, QString);
    void change_flag(QString, QString);

/*
    void clear_nicklist();
    void set_user_info(QString, QString, QString);
    void update_nick_avatar();
    void update_channel_avatar();
    QStringList get_nicklist();
*/

private:
    // params
    QMainWindow *myparent;
    QString strServer;
    int iPort;
    Notify *pNotify;
    QAction *connectAct;
    QToolBar *toolBar;

    // require
    QTcpSocket *camSocket;
    TabManager *pTabM;
    InputLineDockWidget *inputLineDockWidget;
    NickListDockWidget *nickListDockWidget;
    TabContainer *pTabC;
    Network *pNetwork;
    OnetKernel *pOnet_kernel;
    OnetAuth *pOnet_auth;
    DlgChannelFavourites *pDlg_channel_favourites;
    DlgChannelHomes *pDlg_channel_homes;
    DlgChannelList *pDlg_channel_list;
    DlgChannelSettings *pDlg_channel_settings;
    DlgFriends *pDlg_friends;
    DlgIgnore *pDlg_ignore;
    DlgModeration *pDlg_moderation;
    QMap <QString, QByteArray> mNickAvatar;
    QMap <QString, QByteArray> mChannelAvatar;
    QDockWidget *bottomDockWidget;
    QDockWidget *rightDockWidget;
    QMap <QString, NickListWidget*> mChannelNickListWidget;
    sChannelNickStatus mChannelNickStatus;

    // lag
    QAction *lagAct;

private slots:
    void tab_close_requested(int);
    void current_tab_changed(int);

signals:
    void skernel(QString);
    void srequest_uo(QString, QString, QString);

};

#endif // CORE_H