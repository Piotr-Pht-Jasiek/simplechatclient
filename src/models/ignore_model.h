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

#ifndef IGNORE_MODEL_H
#define IGNORE_MODEL_H

#include <QObject>

class IgnoreModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(IgnoreModel)
    static IgnoreModel *Instance;
public:
    static IgnoreModel *instance();

    IgnoreModel();
    void clear();
    QList<QString> getAll();
    void add(const QString &key);
    void remove(const QString &key);
    bool contains(const QString &key);

private:
    QList<QString> ignore;
};

#endif // IGNORE_MODEL_H