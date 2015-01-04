/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2015 Piotr Łuczko <piotr.luczko@gmail.com>
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

#ifndef WEBCAM_DELEGATE_H
#define WEBCAM_DELEGATE_H

#include <QAbstractItemDelegate>

class WebcamDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    WebcamDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    enum WebCamRoles
    {
        WebCamSpectatorsRole = Qt::UserRole+1,
        WebCamRankRole = Qt::UserRole+2,
        WebCamUdgetRole = Qt::UserRole+3,
        WebCamUserChannelsRole = Qt::UserRole+4
    };
};

#endif // WEBCAM_DELEGATE_H
