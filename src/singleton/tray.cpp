/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "core.h"
#include "mainwindow.h"
#include "convert.h"
#include "tray.h"

Tray * Tray::Instance = 0;

Tray * Tray::instance()
{
    if (!Instance)
    {
        Instance = new Tray();
        Instance->init();
    }

    return Instance;
}

Tray::Tray()
{
}

void Tray::init()
{
    connect(Core::instance()->mainWindow()->getTrayIcon(), SIGNAL(messageClicked()), this, SLOT(messageClicked()));
}

void Tray::showMessage(const QString &strTitle, const QString &strMessage)
{
    if ((Core::instance()->mainWindow()->isVisible()) || (Core::instance()->settings.value("tray_message") == "false"))
        return;

    QString strTrayTitle = strTitle;
    Convert::simpleConvert(strTrayTitle);

    QString strTrayMessage = strMessage;
    Convert::simpleConvert(strTrayMessage);

    strLastMessageTitle = strTrayTitle;

    Core::instance()->mainWindow()->getTrayIcon()->showMessage(strTrayTitle, strTrayMessage);
}

void Tray::messageClicked()
{
    if (!Core::instance()->mainWindow()->isVisible())
        Core::instance()->mainWindow()->showNormal();

    Core::instance()->mainWindow()->changeCurrentTab(strLastMessageTitle);
}