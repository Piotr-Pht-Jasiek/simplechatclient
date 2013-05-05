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
#include "offline.h"

Offline * Offline::Instance = 0;

Offline * Offline::instance()
{
    if (!Instance)
    {
        Instance = new Offline();
        Instance->init();
    }

    return Instance;
}

Offline::Offline()
{
    offlineMessagesAction = new QAction(QIcon(":/images/oxygen/16x16/mail-mark-unread.png") , tr("Offline messages"), this);
    offlineMessagesAction->setVisible(false);
}

void Offline::init()
{
    lOfflineNicks.clear();
    lOfflineMessages.clear();
}

void Offline::addMessage(qint64 iTime, const QString &strType, const QString &strNick, const QString &strMessage)
{
    OnetOfflineMessage omessage;
    omessage.datetime = iTime;
    omessage.type = strType;
    omessage.message = strMessage;

    lOfflineMessages.insert(strNick, omessage);
}

void Offline::removeMessage(const QString &strNick)
{
    lOfflineMessages.remove(strNick);
}

void Offline::clearMessages()
{
    lOfflineMessages.clear();
}

QList<OnetOfflineMessage> Offline::getMessages(const QString &strNick)
{
    return lOfflineMessages.values(strNick);
}

QList<OnetOfflineMessage> Offline::getMessagesReverted(const QString &strNick)
{
    QList<OnetOfflineMessage> lMessages = lOfflineMessages.values(strNick);
    QList<OnetOfflineMessage> lMessagesReverted;

    foreach (OnetOfflineMessage message, lMessages)
        lMessagesReverted.prepend(message);

    return lMessagesReverted;
}

bool Offline::isEmptyMessages()
{
    return lOfflineMessages.isEmpty();
}

void Offline::addNick(const QString &nick)
{
    if (!lOfflineNicks.contains(nick))
    {
        lOfflineNicks.append(nick);
        Notification::instance()->refreshOffline();
    }
}

void Offline::removeNick(const QString &nick)
{
    if (lOfflineNicks.contains(nick))
    {
        lOfflineNicks.removeAll(nick);
        Notification::instance()->refreshOffline();
    }
}

void Offline::clearNicks()
{
    lOfflineNicks.clear();
    Notification::instance()->refreshOffline();
}

QList<QString> Offline::getNicks()
{
    return lOfflineNicks;
}

bool Offline::isEmptyNicks()
{
    return lOfflineNicks.isEmpty();
}

int Offline::countNicks()
{
    return lOfflineNicks.size();
}