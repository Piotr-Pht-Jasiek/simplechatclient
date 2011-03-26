/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef DLG_MY_STATS_H
#define DLG_MY_STATS_H

class SimpleStatsWidget;
#include <QDialog>
#include "ui_my_stats.h"

class DlgMyStats : public QDialog
{
    Q_OBJECT
public:
    DlgMyStats(QWidget *, QMap<QString,QString> *);

private:
    Ui::uiMyStats ui;
    QMap<QString,QString> *mMyStats;
    SimpleStatsWidget *simpleStatsWidget;

    void create_gui();
    void create_signals();

    int replace_value(QString);
    void refresh();

private slots:
    void button_close();
};

#endif // DLG_MY_STATS_H
