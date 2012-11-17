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

#include <QSplitter>
#include "core.h"
#include "channel.h"
#include "log.h"
#include "nicklist_widget.h"
#include "settings.h"
#include "tab_manager.h"
#include "tab_container.h"

TabContainer::TabContainer(TabManager *_pTabM) : pTabM(_pTabM)
{
}

TabContainer::~TabContainer()
{
    Channel::instance()->removeAll();
}

void TabContainer::addTab(const QString &strChannel)
{
    if (Channel::instance()->contains(strChannel))
        return;

    Channel::instance()->add(strChannel);

    // create tab
    pTabM->addTab(Channel::instance()->getTw(strChannel), strChannel);
    pTabM->setCurrentIndex(pTabM->count()-1);

    // if priv
    if (strChannel[0] == '^')
    {
        QString strAlternativeName = Channel::instance()->getAlternativeName(strChannel);

        if (!strAlternativeName.isEmpty())
        {
            pTabM->setTabText(pTabM->count()-1, strAlternativeName);
        }
    }
}

void TabContainer::removeTab(const QString &strChannel)
{
    if ((!Channel::instance()->contains(strChannel)) || (strChannel == DEBUG_WINDOW) || (strChannel == STATUS_WINDOW))
        return;

    Channel::instance()->remove(strChannel);
}

void TabContainer::renameTab(const QString &strChannel, const QString &strNewName)
{
    int index = Channel::instance()->getIndexFromName(strChannel);

    if (index >= 0 && index <= pTabM->count())
    {
        if (pTabM->tabText(index)[0] == '^')
        {
            pTabM->setTabText(index, strNewName);

            // log
            Log::logOpened(strChannel);
        }
    }
}

void TabContainer::partTab(int index)
{
    QString strChannel = Channel::instance()->getNameFromIndex(index);

    if (!strChannel.isEmpty())
    {
        if (Core::instance()->network->isConnected())
            Core::instance()->network->send(QString("PART %1").arg(strChannel));
        else
            removeTab(strChannel);
    }
}

void TabContainer::refreshColors()
{
    QList<QString> lChannels = Channel::instance()->getList();

    foreach (QString strChannel, lChannels)
    {
        // update tab name color
        int index = Channel::instance()->getIndexFromName(strChannel);
        pTabM->setColor(index, QColor(Settings::instance()->get("default_color")));
    }
}

void TabContainer::refreshCSS()
{
    QList<QString> lChannels = Channel::instance()->getList();

    foreach (QString strChannel, lChannels)
    {
        Channel::instance()->getChatView(strChannel)->refreshCSS();
    }
}

void TabContainer::resizeMainWindow(QSize s)
{
    QList<QString> lChannels = Channel::instance()->getList();

    foreach (QString strChannel, lChannels)
    {
        if (!Channel::instance()->getNickListWidget(strChannel)->isHidden())
        {
            int width = s.width();
            if (width > 250)
            {
                QList<int> currentSizes = Channel::instance()->getSplitter(strChannel)->sizes();

                currentSizes[0] = width-200;
                currentSizes[1] = 200;

                Channel::instance()->getSplitter(strChannel)->setSizes(currentSizes);
            }
        }
    }
}
