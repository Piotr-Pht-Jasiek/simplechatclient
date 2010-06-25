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

#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
// init
    toolBar = new QToolBar();
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/images/logo_64.png"));

// action
    showAct = new QAction(QIcon(":/images/logo_64.png"),("P&oka�"),this);
    connectAct = new QAction(QIcon(":/3rdparty/images/connect.png"),("&Po��cz"),this);
    closeAct = new QAction(QIcon(":/3rdparty/images/exit.png"),("Za&ko�cz"),this);
    optionsAct = new QAction(QIcon(":/3rdparty/images/options.png"),("O&pcje programu"),this);
    channel_listAct = new QAction(QIcon(":/3rdparty/images/list.png"),("&Lista kana��w"),this);
    channel_homesAct = new QAction(QIcon(":/3rdparty/images/list.png"),("U&stawienia kana��w"),this);
    channel_favouritesAct = new QAction(QIcon(":/3rdparty/images/list.png"),("Ul&ubione kana�y"),this);
    friendsAct = new QAction(QIcon(":/3rdparty/images/people.png"),("Przy&jaciele"),this);
    ignoreAct = new QAction(QIcon(":/3rdparty/images/people.png"),("Ignorowa&ni"),this);
    aboutAct = new QAction(QIcon(":/images/logo_64.png"),("&O programie SCC..."),this);

// shortcut
    connectAct->setShortcuts(QKeySequence::New);
    closeAct->setShortcut(tr("Ctrl+E"));
    optionsAct->setShortcut(tr("Ctrl+O"));
    channel_listAct->setShortcut(tr("Ctrl+L"));
    channel_homesAct->setShortcut(tr("Ctrl+M"));
    channel_favouritesAct->setShortcut(tr("Ctrl+U"));
    friendsAct->setShortcut(tr("Ctrl+P"));
    ignoreAct->setShortcut(tr("Ctrl+I"));

// show options if config not exist
    QString path = QCoreApplication::applicationDirPath();
    QString strConfigFile = path+"/scc.conf";
    if (QFile::exists(strConfigFile) == false)
        QTimer::singleShot(1000, this, SLOT(options_dlg()));

// init all
    Config *pConfig = new Config();
    QString strAutoBusy = pConfig->get_value("auto_busy");
    QString strDebugAll = pConfig->get_value("debug_all");
    QString strShowZuo = pConfig->get_value("show_zuo");
    QString strHideFormating = pConfig->get_value("hide_formating");
    QString strHideJoinPart = pConfig->get_value("hide_join_part");
    delete pConfig;

    settings.clear();
    settings.setValue("version", "1.0.5.260");
    settings.setValue("debug", "off");
    settings.setValue("logged", "off");
    settings.setValue("busy", "off");
    settings.setValue("override", "off");
    settings.setValue("autojoin_favourites", "on");
    settings.setValue("auto_busy", strAutoBusy);
    settings.setValue("debug_all", strDebugAll);
    settings.setValue("show_zuo", strShowZuo);
    settings.setValue("hide_formating", strHideFormating);
    settings.setValue("hide_join_part", strHideJoinPart);
    settings.setValue("uokey", "");
    settings.setValue("onet_ubi", "");
    settings.setValue("onet_cid", "");
    settings.setValue("onet_sid", "");
    settings.setValue("onet_uid", "");

    pTabM = new TabManager(this, &settings);
    setCentralWidget(pTabM);

    pNetwork = new Network(this, connectAct, &settings);
    pNotify = new Notify();
    pTabC = new TabContainer(pNetwork, &settings, pTabM, this, pNotify);

    pDlg_channel_settings = new DlgChannelSettings(pNetwork, &settings);
    pDlg_moderation = new DlgModeration(pNetwork, &settings);
    pDlg_channel_list = new DlgChannelList(pNetwork, &settings, pTabC);
    pDlg_channel_homes = new DlgChannelHomes(pNetwork, &settings, pTabC, pDlg_channel_settings);
    pDlg_channel_favourites = new DlgChannelFavourites(pNetwork, &settings, pTabC);
    pDlg_friends = new DlgFriends(pNetwork, &settings, pTabC);
    pDlg_ignore = new DlgIgnore(pNetwork, &settings, pTabC);
    pIrc_kernel = new IrcKernel(pNetwork, &settings, pTabC, pDlg_channel_settings, pDlg_channel_homes, pDlg_channel_list, pDlg_channel_favourites, pDlg_friends, pDlg_ignore, pDlg_moderation);
    pIrc_auth = new IrcAuth(pNetwork, &settings, pTabC);

    pTabC->set_dlg(pDlg_channel_settings, pDlg_moderation);

    pTabC->show_msg("Status", "%Fi:courier%Witaj w programie Simple Chat Client %Ixhehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%Oficjalna strona SCC%C000000%: http://simplechatclien.sourceforge.net/ %Ixluzak%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%Cff0000%Lista b��d�w%C000000%: http://tinyurl.com/yg3fjb4 %Ixmm%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%C8800ab%Zg�aszanie b��d�w%C000000%: http://tinyurl.com/yfcseeh %Ixco%", 0);

// update
    uThreadList.append(new UpdateThread(&settings, pTabC));
    QObject::connect(uThreadList.at(uThreadList.size()-1), SIGNAL(do_remove_uthread(UpdateThread*)), this, SLOT(remove_uthread(UpdateThread*)));

#ifdef Q_WS_X11
    //if (settings.value("debug").toString() == "on")
        qDebug() << "Update thread +1 (size: " << uThreadList.size() << ")";
#endif

// main menu
    fileMenu = menuBar()->addMenu("&Plik");
    fileMenu->addAction(connectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);

    optionsMenu = menuBar()->addMenu("&Ustawienia");
    optionsMenu->addAction(optionsAct);

    chatMenu = menuBar()->addMenu("&Czat");
    chatMenu->addAction(channel_listAct);
    chatMenu->addAction(channel_homesAct);
    chatMenu->addAction(channel_favouritesAct);
    chatMenu->addAction(friendsAct);
    chatMenu->addAction(ignoreAct);

    helpMenu = menuBar()->addMenu("Pomo&c");
    helpMenu->addAction(aboutAct);

// toolbar
    toolBar = addToolBar("Pasek nawigacji");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAct);
    toolBar->addAction(optionsAct);
    toolBar->addAction(channel_listAct);
    toolBar->addAction(channel_homesAct);
    toolBar->addAction(friendsAct);
    toolBar->addAction(channel_favouritesAct);

// statusbar
    QLabel *label_status = new QLabel();
    label_status->setText("v"+settings.value("version").toString());
    statusBar()->addWidget(label_status);

// signals
    QObject::connect(connectAct, SIGNAL(triggered()), this, SLOT(button_connect()));
    QObject::connect(optionsAct, SIGNAL(triggered()), this, SLOT(options_dlg()));
    QObject::connect(channel_listAct, SIGNAL(triggered()), this, SLOT(channel_list_dlg()));
    QObject::connect(channel_homesAct, SIGNAL(triggered()), this, SLOT(channel_homes_dlg()));
    QObject::connect(channel_favouritesAct, SIGNAL(triggered()), this, SLOT(channel_favourites_dlg()));
    QObject::connect(friendsAct, SIGNAL(triggered()), this, SLOT(friends_dlg()));
    QObject::connect(ignoreAct, SIGNAL(triggered()), this, SLOT(ignore_dlg()));
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(about_dlg()));
    QObject::connect(showAct, SIGNAL(triggered()), this, SLOT(button_show()));
    QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(button_close()));
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pDlg_moderation, SIGNAL(display_msg(QString,QString,int)), pTabC, SLOT(sshow_msg(QString,QString,int)));

    QObject::connect(this, SIGNAL(do_kernel(QString)), pIrc_kernel, SLOT(kernel(QString)));
    QObject::connect(this, SIGNAL(do_request_uo(QString, QString)), pIrc_auth, SLOT(request_uo(QString, QString)));

// tray
    trayMenu = new QMenu();
    trayMenu->addAction(showAct);
    trayMenu->addAction(closeAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/logo_64.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
    trayIcon->hide();
    settings.setValue("reconnect", "false");
    delete trayIcon;
    delete trayMenu;
    delete pIrc_auth;
    delete pIrc_kernel;
    delete pDlg_ignore;
    delete pDlg_friends;
    delete pDlg_channel_favourites;
    delete pDlg_channel_homes;
    delete pDlg_channel_list;
    delete pDlg_moderation;
    delete pDlg_channel_settings;
    delete pNotify;
    delete pNetwork;
    delete pTabC;
    delete pTabM;
}

void MainWindow::remove_uthread(UpdateThread *thr)
{
    thr->QObject::disconnect();
    uThreadList.removeOne(thr);

#ifdef Q_WS_X11
    //if (settings.value("debug").toString() == "on")
        qDebug() << "Update thread -1 (size: " << uThreadList.size() << ")";
#endif
}

// slots

void MainWindow::kernel(QString param1) { emit do_kernel(param1); }
void MainWindow::request_uo(QString param1, QString param2) { emit do_request_uo(param1, param2); }
void MainWindow::show_msg_active(QString param1, int param2) { pTabC->show_msg_active(param1, param2); }
void MainWindow::show_msg_all(QString param1, int param2) { pTabC->show_msg_all(param1, param2); }
void MainWindow::update_nick(QString param1) { pTabC->update_nick(param1); }
void MainWindow::clear_nicklist(QString param1) { pTabC->clear_nicklist(param1); }
void MainWindow::clear_all_nicklist() { pTabC->clear_all_nicklist(); }

// args
void MainWindow::set_debug(bool param1)
{
    if (param1 == true)
        settings.setValue("debug", "on");
    else
        settings.setValue("debug", "off");
}

// buttons
void MainWindow::button_close()
{
    this->close();
}

void MainWindow::button_connect()
{
    if (pNetwork->is_connected() == false)
    {
        connectAct->setText("&Roz��cz");
        connectAct->setIconText("&Roz��cz");
        settings.setValue("reconnect", "true");
        pNetwork->connect();
    }
    else
    {
        settings.setValue("reconnect", "false");
        pNetwork->send("QUIT");
        connectAct->setText("&Po��cz");
        connectAct->setIconText("&Po��cz");
        pNetwork->close();
    }
}

void MainWindow::options_dlg()
{
    (new DlgOptions(this, &settings))->show();
}

void MainWindow::channel_list_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_list->show();
}

void MainWindow::channel_homes_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_homes->show();
}

void MainWindow::channel_favourites_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_favourites->show();
}

void MainWindow::friends_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_friends->show();
}

void MainWindow::ignore_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_ignore->show();
}

void MainWindow::about_dlg()
{
    (new DlgAbout(this, &settings))->show();
}

// tray
void MainWindow::tray_icon(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        if (this->isVisible() == true)
            this->hide();
        else
            this->showNormal();
    }
}

// tray button
void MainWindow::button_show()
{
    this->showNormal();
}

// close tab
void MainWindow::tab_close_requested(int index)
{
    QString strName = pTabM->tabText(index);
    if (strName != "Status")
    {
        if (strName[0] == '#')
        {
            pTabC->remove_tab(strName);
            if (pNetwork->is_connected() == true)
                pNetwork->send(QString("PART %1").arg(strName));
        }
        else
        {
            if (pTabC->exist_tab(strName) == false)
            {
                QString strNewName = settings.value("priv"+strName).toString();
                if (strNewName.isEmpty() == false)
                {
                    if (pNetwork->is_connected() == true)
                        pNetwork->send(QString("PART %1").arg(strNewName));
                }
            }
            else
            {
                if (pNetwork->is_connected() == true)
                    pNetwork->send(QString("PART %1").arg(strName));
            }
        }
    }
}
