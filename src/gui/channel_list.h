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

#ifndef CHANNEL_LIST_H
#define CHANNEL_LIST_H

#include <QDialog>
#include "ui_channel_list.h"

class DlgChannelList : public QDialog
{
    Q_OBJECT
public:
    DlgChannelList(QWidget *);

private:
    Ui::uiChannelList ui;

    void createGui();
    void setDefaultValues();
    void createSignals();

    bool isErotic(const QString&);
    void getOptions();
    QString channelTypeToString(int);
    QString channelCatToString(int, bool, bool);
    bool showChannel(const QString&, int, int, int, bool, bool);

    QString strSearch;
    bool bShowTeen;
    bool bShowCommon;
    bool bShowErotic;
    bool bShowThematic;
    bool bShowRegional;

    bool bShowWild;
    bool bShowTame;
    bool bShowWithClass;
    bool bShowCult;
    bool bShowModerated;
    bool bShowRecommended;

    bool bHideEmpty;

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void allCellDoubleClicked(int, int);
    void teenCellDoubleClicked(int, int);
    void commonCellDoubleClicked(int, int);
    void eroticCellDoubleClicked(int, int);
    void thematicCellDoubleClicked(int, int);
    void regionalCellDoubleClicked(int, int);
    void createList();
    void buttonClear();
    void showAdvOptions();
};

#endif // CHANNEL_LIST_H
