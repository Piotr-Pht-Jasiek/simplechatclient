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

#ifndef AWAYLOG_MODEL_H
#define AWAYLOG_MODEL_H

class QAction;
#include <QObject>

class Awaylog : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Awaylog)
    static Awaylog *Instance;
public:
    static Awaylog *instance();

    Awaylog();
    void init();
    void add(int iTime, const QString &strChannel, const QString &strAwayData);
    QList<QString> get();
    void clear();
    bool isEmpty();
    int count();

    QAction *awaylogAction;

private:
    QList<QString> lAwaylog;

};

#endif // AWAYLOG_MODEL_H
