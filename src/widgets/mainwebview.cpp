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

#include "mainwebview.h"

MainWebView::MainWebView(QWidget *parent, Network *param1, QString param2, QTcpSocket *param3, sChannelNickStatus *param4, DlgUserProfile *param5)
{
    myparent = parent;
    pNetwork = param1;
    strChannel = param2;
    camSocket = param3;
    mChannelNickStatus = param4;
    pDlg_user_profile = param5;

    strNick = QString::null;
}

void MainWebView::join_channel()
{
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

MainWebView::~MainWebView()
{
    strOpenChannels.clear();
}

void MainWebView::set_open_channels(QStringList param1)
{
    strOpenChannels = param1;
}

void MainWebView::priv()
{
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void MainWebView::whois()
{
    pNetwork->send(QString("WHOIS %1 :%1").arg(strNick));
}

void MainWebView::profile()
{
    if (strNick[0] != '~')
    {
        pDlg_user_profile->set_nick(strNick);
        pDlg_user_profile->show();
    }
}

void MainWebView::cam()
{
#ifdef Q_WS_WIN
    QSettings settings;
    QString strUOKey = settings.value("uokey").toString();
    (new Kamerzysta(camSocket))->show(strNick, strUOKey);
#else
    (new DlgCam(myparent, pNetwork, strNick))->show();
#endif
}

void MainWebView::friends_add()
{
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void MainWebView::friends_del()
{
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void MainWebView::ignore_add()
{
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void MainWebView::ignore_del()
{
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void MainWebView::kick()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
}

void MainWebView::ban()
{
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainWebView::kban()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok == true) && (strText.isEmpty() == false))
    {
        pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
    }
}

void MainWebView::ipban()
{
    pNetwork->send(QString("CS BANIP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainWebView::op_add()
{
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainWebView::op_del()
{
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainWebView::halfop_add()
{
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainWebView::halfop_del()
{
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainWebView::moderator_add()
{
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainWebView::moderator_del()
{
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainWebView::voice_add()
{
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainWebView::voice_del()
{
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainWebView::invite()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

void MainWebView::contextMenuEvent(QContextMenuEvent *event)
{
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    // link
    if (r.linkUrl().isEmpty() == false)
    {
        QString strLink = r.linkUrl().toString();
        QString strCat = strLink.left(4);
        strLink = strLink.right(strLink.length()-4);

        // channel
        if (strCat == "chan")
        {
            strChannel = strLink;

            QAction *nameAct = new QAction(strChannel, this);
            nameAct->setDisabled(true);

            QMenu *menu = new QMenu(strChannel);
            menu->addAction(nameAct);
            menu->addSeparator();
            menu->addAction(tr("Join channel"), this, SLOT(join_channel()));
            menu->exec(mapToGlobal(event->pos()));
        }
        // nick
        else
        {
            strNick = strLink;

            QString strPrefix;
            QString strSuffix;

            for (int i = 0; i < mChannelNickStatus->count(); i++)
            {
                if ((mChannelNickStatus->at(i).nick == strNick) && (mChannelNickStatus->at(i).channel == strChannel))
                {
                    strPrefix = mChannelNickStatus->at(i).prefix;
                    strSuffix = mChannelNickStatus->at(i).suffix;
                    break;
                }
            }

            QMenu *minvite = new QMenu(tr("Invite"));

            for (int i = 0; i < maxOpenChannels; ++i)
            {
                openChannelsActs[i] = new QAction(this);
                openChannelsActs[i]->setVisible(false);
                connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
             }

            for (int i = 0; i < maxOpenChannels; ++i)
                minvite->addAction(openChannelsActs[i]);

            for (int i = 0; i < strOpenChannels.count(); ++i)
            {
                openChannelsActs[i]->setText(strOpenChannels[i]);
                openChannelsActs[i]->setData(strOpenChannels[i]);
                openChannelsActs[i]->setVisible(true);
            }
            for (int j = strOpenChannels.count(); j < maxOpenChannels; ++j)
                openChannelsActs[j]->setVisible(false);

            QMenu *friends = new QMenu(tr("Friends list"));
            friends->addAction(tr("Add to friends"), this, SLOT(friends_add()));
            friends->addAction(tr("Remove from friends"), this, SLOT(friends_del()));

            QMenu *ignore = new QMenu(tr("Ignore list"));
            ignore->addAction(tr("Add to Ignore list"), this, SLOT(ignore_add()));
            ignore->addAction(tr("Remove from Ignore list"), this, SLOT(ignore_del()));

            QMenu *privilege = new QMenu(tr("Actions"));

            if (strPrefix.indexOf("@") == -1)
                privilege->addAction(tr("Give super operator status"), this, SLOT(op_add()));
            else
                privilege->addAction(tr("Take super operator status"), this, SLOT(op_del()));

            if (strPrefix.indexOf("%") == -1)
                privilege->addAction(tr("Give operator status"), this, SLOT(halfop_add()));
            else
                privilege->addAction(tr("Take operator status"), this, SLOT(halfop_del()));

            if (strPrefix.indexOf("!") == -1)
                privilege->addAction(tr("Give moderator status"), this, SLOT(moderator_add()));
            else
                privilege->addAction(tr("Take moderator status"), this, SLOT(moderator_del()));

            if (strPrefix.indexOf("+") == -1)
                privilege->addAction(tr("Give guest status"), this, SLOT(voice_add()));
            else
                privilege->addAction(tr("Take guest status"), this, SLOT(voice_del()));

            QAction *nameAct = new QAction(strNick, this);
            nameAct->setDisabled(true);

            QMenu *menu = new QMenu(strNick);
            menu->addAction(nameAct);
            menu->addSeparator();
            menu->addAction(tr("Priv"), this, SLOT(priv()));
            menu->addAction(tr("Whois"), this, SLOT(whois()));
            if (strNick[0] != '~')
            {
                menu->addAction(tr("Profile"), this, SLOT(profile()));
                menu->addAction(tr("Webcam"), this, SLOT(cam()));
            }
            menu->addMenu(minvite);
            menu->addMenu(friends);
            menu->addMenu(ignore);
            menu->addSeparator();
            menu->addAction(tr("Kick From Channel"), this, SLOT(kick()));
            menu->addAction(tr("Ban From Channel"), this, SLOT(ban()));
            menu->addAction(tr("Kick & Ban"), this, SLOT(kban()));
            menu->addAction(tr("IP Ban"), this, SLOT(ipban()));
            menu->addSeparator();
            menu->addMenu(privilege);

            menu->exec(mapToGlobal(event->pos()));
        }
        return;
    }
    // selected text
    else if (r.isContentSelected() == true)
    {
        QAction *copyAct = pageAction(QWebPage::Copy);
        copyAct->setShortcut(QKeySequence::Copy);

        QMenu menu("copy");
        menu.addAction(copyAct);
        menu.exec(mapToGlobal(event->pos()));
        return;
    }
    // not selected text
    else if (r.isContentSelected() == false)
        return;
    // other
    else
        QWebView::contextMenuEvent(event);
}
