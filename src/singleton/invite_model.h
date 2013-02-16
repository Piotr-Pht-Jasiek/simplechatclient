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

#ifndef INVITE_MODEL_H
#define INVITE_MODEL_H

#include <QMultiHash>
#include <QObject>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class Invite : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Invite)
    static Invite *Instance;
public:
    static Invite *instance();

    Invite();
    void add(const QString &nick, const QString &channel);
    void remove(const QString &nick, const QString &channel);
    void clear();
    bool isEmpty();
    int count();
    QMultiHash<QString,QString> get();

    QAction *inviteAction;

private:
    QMultiHash<QString,QString> lInvite;

    void init();
};

#endif // INVITE_MODEL_H
