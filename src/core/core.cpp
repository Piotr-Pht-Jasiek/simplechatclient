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

#include <QSettings>
#include "config.h"
#include "mainwindow.h"
#include "update.h"
#include "core.h"

Core * Core::Instance = 0;

Core * Core::instance()
{
    if (!Instance)
    {
        Instance = new Core();
        Instance->init();
    }

    return Instance;
}

Core::Core() : window(0)
{
    pUpdate = new Update();

    // lag
    lagAct = new QAction("Lag: ?", this);
}

Core::~Core()
{
    delete pUpdate;
    delete window;
    window = 0;
}

void Core::init()
{
    createSettings();
}

void Core::createGui()
{
    window = new MainWindow(0);
}

void Core::createSettings()
{
    // default settings
    QSettings settings;
    settings.setValue("version", "1.0.11.830");
    settings.setValue("logged", "off");
    settings.setValue("busy", "off");
    settings.setValue("away", "off");
    settings.setValue("override", "off");
    settings.setValue("ignore_raw_141", "off");
    settings.setValue("age_check", "on");
    settings.setValue("first_run", "true");
    settings.setValue("uokey", "");
    settings.setValue("uo_nick", "");
    settings.setValue("onet_ubi", "");
    settings.setValue("onet_cid", "");
    settings.setValue("onet_sid", "");
    settings.setValue("onet_uid", "");
    settings.setValue("onetzuo_ticket", "");

    // config values
    Config *pConfig = new Config();
    QMap <QString, QString> mConfigValues = pConfig->read_config();
    delete pConfig;

    // set settings
    QMapIterator <QString, QString> i(mConfigValues);
    while (i.hasNext())
    {
        i.next();
        settings.setValue(i.key(), i.value());
    }

    // fix config values
    if (settings.value("style").toString() == "classic")
        settings.setValue("disable_avatars", "on");
}

QString Core::version()
{
    QSettings settings;
    return settings.value("version").toString();
}

void Core::enableDebug()
{
    QSettings settings;
    settings.setValue("debug", "on");
}

void Core::showSccWindow()
{
    window->show();
}

MainWindow *Core::sccWindow()
{
    return window;
}

void Core::quit()
{
    if (!Instance)
        return;

    delete Instance;
    Instance = 0;
}

// from options
void Core::refresh_colors()
{
    window->refresh_colors();
}

// from options
void Core::refresh_background_image()
{
    window->refresh_background_image();
}

// update
void Core::check_update()
{
    pUpdate->check_update();
}

// for avatars (if nick not in any channels -> remove avatar)
int Core::get_nick_channels(QString strNick)
{
    int iResult = 0;

    for (int i = 0; i < lUsers.size(); i++)
    {
        if (lUsers.at(i).nick == strNick)
            iResult++;
    }

    return iResult;
}

QList<QString> Core::get_nicks_from_channel(QString strChannel)
{
    QList<QString> usersList;
    for (int i = 0; i < lUsers.size(); i++)
    {
        if (lUsers.at(i).channel == strChannel)
        {
            QString strUser = lUsers.at(i).nick;
            usersList.append(strUser);
        }
    }
    return usersList;
}
