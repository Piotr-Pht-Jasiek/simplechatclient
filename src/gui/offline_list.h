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

#ifndef OFFLINE_LIST_H
#define OFFLINE_LIST_H

#include <QDialog>
#include "ui_offline_list.h"

class DlgOfflineList : public QDialog
{
    Q_OBJECT
public:
    DlgOfflineList(QWidget *parent = 0);

private:
    Ui::uiOfflineList ui;
    enum OfflineMessageRoles
    {
        OfflineMessageNickRole = Qt::UserRole
    };

    void createGui();
    void createSignals();
    void refresh();

private slots:
    void itemClicked(QListWidgetItem *);
    void buttonRead();
    void buttonReject();
};

#endif // OFFLINE_LIST_H
