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

#include <QAction>
#include "notification.h"
#include "invite.h"

Invite * Invite::Instance = 0;

Invite * Invite::instance()
{
    if (!Instance)
    {
        Instance = new Invite();
        Instance->init();
    }

    return Instance;
}

Invite::Invite()
{
    inviteAction = new QAction(QIcon(":/images/oxygen/16x16/legalmoves.png"), tr("Invite"), this);
    inviteAction->setVisible(false);
}

void Invite::init()
{
    lInvite.clear();
}

void Invite::add(const QString &id, qint64 datetime, const QString &nick, const QString &channel)
{
    OnetInvite add;
    add.id = id;
    add.datetime = datetime;
    add.nick = nick;
    add.channel = channel;

    lInvite.append(add);
    Notification::instance()->refreshInvite();
}

void Invite::remove(const QString &id)
{
    QMutableListIterator<OnetInvite> i(lInvite);
    while (i.hasNext())
    {
        OnetInvite oinvite = i.next();
        if (oinvite.id == id)
            i.remove();
    }

    Notification::instance()->refreshInvite();
}

void Invite::clear()
{
    lInvite.clear();
    Notification::instance()->refreshInvite();
}

bool Invite::isEmpty()
{
    return lInvite.isEmpty();
}

int Invite::count()
{
    return lInvite.size();
}

QList<OnetInvite> Invite::get()
{
    return lInvite;
}
