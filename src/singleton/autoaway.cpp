/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QDateTime>
#include <QTimer>
#include "core.h"
#include "autoaway.h"

Autoaway * Autoaway::Instance = 0;

Autoaway * Autoaway::instance()
{
    if (!Instance)
    {
        Instance = new Autoaway();
        Instance->init();
    }

    return Instance;
}

Autoaway::Autoaway()
{
}

void Autoaway::init()
{
    autoAwayTimer = new QTimer();
    autoAwayTimer->setInterval(1000*60*1); // 1 min

    connect(autoAwayTimer, SIGNAL(timeout()), this, SLOT(timeoutAutoaway()));
}

void Autoaway::start()
{
    if (autoAwayTimer->isActive())
        autoAwayTimer->stop();

    autoAwayTimer->start();
}

void Autoaway::stop()
{
    if (autoAwayTimer->isActive())
        autoAwayTimer->stop();
}

void Autoaway::timeoutAutoaway()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
    {
        int iCurrentTime = QDateTime::currentDateTime().toTime_t();

        int iLastActive = Core::instance()->settings.value("last_active").toInt();

        bool bAway = Core::instance()->settings.value("away") == "true" ? true : false;

        if ((!bAway) && (iLastActive != 0) && (iCurrentTime-iLastActive > 300))
            Core::instance()->pNetwork->send(QString("AWAY :%1").arg(tr("Not here right now")));
    }
}
