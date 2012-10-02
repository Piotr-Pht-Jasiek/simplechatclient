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

#ifndef AVATAR_H
#define AVATAR_H

class QNetworkAccessManager;
class QNetworkReply;
#include <QObject>

class Avatar : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Avatar)
    static Avatar *Instance;
public:
    static Avatar *instance();

    Avatar();
    virtual ~Avatar();
    void get(const QString &strNickOrChannel, const QString &strCategory, const QString &strUrl);
    QString getAvatarPath(const QString &strAvatar);

private:
    QNetworkAccessManager *accessManager;

    void saveAvatar(const QString &strAvatar, const QByteArray &bAvatar);

public slots:
    void httpFinished(QNetworkReply *reply);
};

#endif // AVATAR_H