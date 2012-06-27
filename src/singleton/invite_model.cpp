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

#include "invite_model.h"

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
    inviteAction = new QAction(QIcon() , tr("Invite"), this);
    inviteAction->setVisible(false);
}

void Invite::init()
{
    lInvite.clear();
}

void Invite::add(const QString &nick, const QString &channel)
{
    lInvite.insert(nick, channel);
}

void Invite::remove(const QString &nick, const QString &channel)
{
    lInvite.remove(nick, channel);
}

void Invite::clear()
{
    lInvite.clear();
}

bool Invite::isEmpty()
{
    return lInvite.isEmpty();
}

int Invite::count()
{
    return lInvite.size();
}

QMultiHash<QString,QString> Invite::get()
{
    return lInvite;
}
