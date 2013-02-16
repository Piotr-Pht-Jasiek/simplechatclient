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

#include <QTabBar>
#include "channel.h"
#include "defines.h"
#include "tab_manager.h"

TabManager::TabManager(QWidget *parent) : QTabWidget(parent)
{
    tab = tabBar();
    setTabsClosable(true);
    setMovable(true);
    setIconSize(QSize(22,22));

    cRed = QColor(255, 0, 0, 255);
    cGreen = QColor(0, 147, 0, 255);
    cHighlight = QColor(138, 0, 184, 255);

    connect(tab, SIGNAL(tabMoved(int,int)), this, SLOT(tabMovedSlot(int, int)));
}

void TabManager::setAlert(const QString &channel, ChannelColor c)
{
    QColor color;
    int colorPriority = 0;

    if (c == ChannelGreen)
    {
        color = cGreen;
        colorPriority = 1;
    }
    else if (c == ChannelRed)
    {
        color = cRed;
        colorPriority = 2;
    }
    else if (c == ChannelHighlight)
    {
        color = cHighlight;
        colorPriority = 3;
    }

    int index = Channel::instance()->getIndexFromName(channel);
    QColor currentColor = tab->tabTextColor(index);
    int currentPriority = 0;

    if (currentColor == cGreen)
        currentPriority = 1;
    else if (currentColor == cRed)
        currentPriority = 2;
    else if (currentColor == cHighlight)
        currentPriority = 3;

    if (colorPriority > currentPriority)
    {
        if ((index >= 0) && (index < count()) && (index != currentIndex()))
            tab->setTabTextColor(index, color);
    }
}

void TabManager::setColor(int index, QColor color)
{
    if ((index >= 0) && (index < count()))
        tab->setTabTextColor(index, color);
}

void TabManager::updateIcon(int index, const QString &avatar)
{
    if ((index >= 0) && (index < count()))
    {
        setTabIcon(index, QIcon(avatar));
    }
}

void TabManager::hideCloseButton(int index)
{
    if ((index >= 0) && (index < count()))
        tab->setTabButton(index, QTabBar::RightSide, 0);
}

void TabManager::tabInserted(int index)
{
    if (tabText(index) == STATUS_WINDOW) // hide close on status
        hideCloseButton(index);
    else if (tabText(index) == DEBUG_WINDOW) // hide close on debug
        hideCloseButton(index);
}

void TabManager::tabMovedSlot(int from, int to)
{
    Channel::instance()->move(from, to);
}
