/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef DLG_CHANNEL_HOMES_AD_H
#define DLG_CHANNEL_HOMES_AD_H

#include <QDialog>
#include <QSettings>
#include "network.h"
#include "ui_channel_homes_ad.h"

class DlgChannelHomesAd : public QDialog
{
    Q_OBJECT
public:
    DlgChannelHomesAd(QWidget *, Network *, QSettings *, QString);

private:
    Ui::uiChannelHomesAd ui;
    Network *pNetwork;
    QSettings *settings;
    QString strStatus;

private slots:
    void button_ok();
    void button_cancel();

};

#endif // DLG_CHANNEL_HOMES_AD_H
