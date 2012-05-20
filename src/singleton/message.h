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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "defines.h"
#include <QObject>

class Message : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Message)
    static Message *Instance;
public:
    static Message *instance();

    Message();
    void init();

    void showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strTime = QString::null, QString strNick = QString::null);
    void showMessageAll(const QString &strData, MessageCategory eMessageCategory);
    void showMessageActive(const QString &strData, MessageCategory eMessageCategory);

private:
    void saveMessage(const QString &strChannel, const QString &strData, QString strTime = QString::null, QString strNick = QString::null);
    bool hideJoinPart(const QString &strChannel, MessageCategory eMessageCategory);
    bool isHighlightMessage(const QString &strMessage);

};

#endif // MESSAGE_H