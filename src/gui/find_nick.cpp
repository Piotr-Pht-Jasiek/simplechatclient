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

#include <QDesktopWidget>
#include <QMenu>
#include <QTimer>
#include "core.h"
#include "channel.h"
#include "defines.h"
#include "find_nick_model.h"
#include "find_nick.h"

DlgFindNick::DlgFindNick(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Find nick"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();
}

void DlgFindNick::createGui()
{
    ui.toolButton_options->setEnabled(false);

    ui.pushButton_search->setIcon(QIcon(":/images/oxygen/16x16/edit-find.png"));
    ui.pushButton_clear->setIcon(QIcon(":/images/oxygen/16x16/draw-eraser.png"));
    ui.toolButton_options->setIcon(QIcon(":/images/oxygen/16x16/applications-system.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.label_find_nick->setText(tr("Find nick:"));
    ui.pushButton_search->setText(tr("Search"));
    ui.pushButton_clear->setText(tr("Clear"));

    ui.toolButton_options->setText(tr("Options"));

    QMenu *optionsMenu = new QMenu(this);
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/user-properties.png"), tr("Whois"), this, SLOT(whois()));

    QMenu *mInvite = new QMenu(tr("Invite"));
    mInvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));

    QList<QString> lChannelsCleared = Channel::instance()->getCleared();
    for (int i = 0; i < lChannelsCleared.size(); ++i)
    {
        QString strOpenChannel = lChannelsCleared[i];
        if (strOpenChannel[0] == '^')
            strOpenChannel = Channel::instance()->getPriv(strOpenChannel);

        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        openChannelsActs[i]->setText(strOpenChannel);
        openChannelsActs[i]->setData(lChannelsCleared[i]);
        openChannelsActs[i]->setVisible(true);

        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
        mInvite->addAction(openChannelsActs[i]);
    }
    optionsMenu->addMenu(mInvite);

    ui.toolButton_options->setMenu(optionsMenu);

    //focus
    ui.lineEdit_search->setFocus();
}

void DlgFindNick::createSignals()
{
    connect(ui.listWidget_nicks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(ui.pushButton_search, SIGNAL(clicked()), this, SLOT(buttonFind()));
    connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(buttonClear()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgFindNick::buttonFind()
{
    ui.lineEdit_search->setFocus();

    FindNick::instance()->clear();

    QString search = ui.lineEdit_search->text();
    if (!search.isEmpty())
    {
        Core::instance()->network->send(QString("NS LIST %1 20").arg(search));
        refreshList();
    }
    else
        ui.listWidget_nicks->clear();
}

void DlgFindNick::buttonClear()
{
    ui.lineEdit_search->setFocus();

    ui.lineEdit_search->clear();
    ui.listWidget_nicks->clear();
    ui.listWidget_nicks->setEnabled(false);
    ui.toolButton_options->setEnabled(false);
}

void DlgFindNick::refreshList()
{
    // ready
    if (!FindNick::instance()->getReady())
    {
        QTimer::singleShot(200, this, SLOT(refreshList())); // 0.2 sec
        return;
    }

    ui.listWidget_nicks->clear();

    QList<QString> list = FindNick::instance()->get();
    foreach (const QString &nick, list)
    {
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setData(Qt::UserRole+10, false); // is nicklist
        item->setText(nick);

        ui.listWidget_nicks->addItem(item);
    }

    if (!list.isEmpty())
        ui.listWidget_nicks->setEnabled(true);
    else
        ui.listWidget_nicks->setEnabled(false);

    ui.toolButton_options->setEnabled(false);
}

void DlgFindNick::itemClicked(QListWidgetItem *)
{
    if (!ui.toolButton_options->isEnabled())
        ui.toolButton_options->setEnabled(true);
}

void DlgFindNick::priv()
{
    if (ui.listWidget_nicks->selectedItems().size() != 0)
    {
        QString strNick = ui.listWidget_nicks->selectedItems().at(0)->text();

        Core::instance()->network->send(QString("PRIV %1").arg(strNick));
    }
}

void DlgFindNick::whois()
{
    if (ui.listWidget_nicks->selectedItems().size() != 0)
    {
        QString strNick = ui.listWidget_nicks->selectedItems().at(0)->text();

        Core::instance()->network->send(QString("WHOIS %1 :%1").arg(strNick));
    }
}

void DlgFindNick::invite()
{
    if (ui.listWidget_nicks->selectedItems().size() != 0)
    {
        QString strNick = ui.listWidget_nicks->selectedItems().at(0)->text();

        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            QString strInviteChannel = action->data().toString();
            Core::instance()->network->send(QString("INVITE %1 %2").arg(strNick, strInviteChannel));
        }
    }
}
