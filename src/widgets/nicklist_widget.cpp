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

#include <QContextMenuEvent>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMenu>
#include "core.h"
#include "defines.h"
#include "user_profile.h"
#include "nicklist_widget.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "webcam.h"
#endif

NickListWidget::NickListWidget(QString _strChannel) : strChannel(_strChannel), strSelectedNick(QString::null)
{
    setFocusPolicy(Qt::NoFocus);
    setSortingEnabled(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void NickListWidget::addUser(QString strNick, QString strModes)
{
    // if owner remove op
    if (strModes.contains("`")) strModes.remove("@");

    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setText(strNick);
    item->setData(Qt::UserRole+10, true); // is nicklist
    item->setData(Qt::UserRole+11, Core::instance()->getUserMaxModes(strModes));
    item->setData(Qt::UserRole+12, strModes);
    item->setData(Qt::UserRole+13, QByteArray()); // avatar
    item->setData(Qt::UserRole+14, QString()); // avatar link

    this->addItem(item);
}

void NickListWidget::delUser(QString strNick)
{
    QList<QListWidgetItem*> items = this->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        this->takeItem(this->row(item));
}

bool NickListWidget::existUser(QString strNick)
{
    if (this->findItems(strNick, Qt::MatchExactly).isEmpty())
        return false;
    else
        return true;
}

void NickListWidget::updateUserAvatar(QString strNick, QByteArray bData)
{
    QList<QListWidgetItem*> items = this->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        item->setData(Qt::UserRole+13, bData);
}

void NickListWidget::updateUserAvatarLink(QString strNick, QString strValue)
{
    QList<QListWidgetItem*> items = this->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        item->setData(Qt::UserRole+14, strValue);
}

QString NickListWidget::getUserAvatarLink(QString strNick)
{
    QList<QListWidgetItem*> items = this->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        return item->data(Qt::UserRole+14).toString();
    return QString::null;
}

void NickListWidget::changeUserFlag(QString strNick, QString strFlag)
{
    QList<QListWidgetItem*> items = this->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
    {
        QString strModes = item->data(Qt::UserRole+12).toString();
        QString plusminus = strFlag.at(0);
        strFlag.remove(0, 1);

        strFlag = convertFlag(strFlag);

        if (plusminus == "+")
        {
            // fix webcam flags
            if ((strFlag == "W") && (strModes.contains("V"))) strModes.remove("V");
            if ((strFlag == "V") && (strModes.contains("W"))) strModes.remove("W");

            if (!strModes.contains(strFlag))
                strModes.append(strFlag);
        }
        else
        {
            if (strModes.contains(strFlag))
                strModes.remove(strFlag);
        }

        // if owner remove op
        if ((strModes.contains("`")) && (strModes.contains("@"))) strModes.remove("@");

        item->setData(Qt::UserRole+11, Core::instance()->getUserMaxModes(strModes));
        item->setData(Qt::UserRole+12, strModes);
    }
}

QString NickListWidget::getUserModes(QString strNick)
{
    QList<QListWidgetItem*> items = this->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        return item->data(Qt::UserRole+12).toString();
    return QString::null;
}

QList<QString> NickListWidget::getUserList()
{
    QList<QString> userList;

    QList<QListWidgetItem*> items = this->findItems("*", Qt::MatchWrap | Qt::MatchWildcard);
    foreach (QListWidgetItem *item, items)
        userList.append(item->text());

    return userList;
}

QString NickListWidget::convertFlag(QString strFlag)
{
    QString strConvertFrom = "qaohXYv";
    QString strConvertTo = "`&@%!=+";

    for (int i = 0; i < strConvertFrom.size(); i++)
        strFlag.replace(strConvertFrom.at(i), strConvertTo.at(i));

    return strFlag;
}

void NickListWidget::priv()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("PRIV %1").arg(strSelectedNick));
}

void NickListWidget::whois()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("WHOIS %1 :%1").arg(strSelectedNick));
}

void NickListWidget::profile()
{
    if (strSelectedNick.isEmpty()) return;

    if (strSelectedNick[0] != '~')
        DlgUserProfile(this, strSelectedNick).exec();
}

void NickListWidget::cam()
{
    if (strSelectedNick.isEmpty()) return;

#ifdef Q_WS_WIN
    (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strSelectedNick);
#else
    new DlgWebcam(strSelectedNick, true);
#endif
}

void NickListWidget::friendsAdd()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("NS FRIENDS ADD %1").arg(strSelectedNick));
}

void NickListWidget::friendsDel()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("NS FRIENDS DEL %1").arg(strSelectedNick));
}

void NickListWidget::ignoreAdd()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("NS IGNORE ADD %1").arg(strSelectedNick));
}

void NickListWidget::ignoreDel()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("NS IGNORE DEL %1").arg(strSelectedNick));
}

void NickListWidget::kick()
{
    if (strSelectedNick.isEmpty()) return;

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
        Core::instance()->pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strSelectedNick).arg(strText));
}

void NickListWidget::ban()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::kban()
{
    if (strSelectedNick.isEmpty()) return;

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strSelectedNick));
        Core::instance()->pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strSelectedNick).arg(strText));
    }
}

void NickListWidget::ipban()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS BANIP %1 ADD %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::opAdd()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::opDel()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::halfopAdd()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::halfopDel()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::moderatorAdd()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::moderatorDel()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::voiceAdd()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::voiceDel()
{
    if (strSelectedNick.isEmpty()) return;

    Core::instance()->pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strSelectedNick));
}

void NickListWidget::invite()
{
    if (strSelectedNick.isEmpty()) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        Core::instance()->pNetwork->send(QString("INVITE %1 %2").arg(strSelectedNick).arg(strInviteChannel));
    }
}

void NickListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if (this->selectedItems().size() == 0) return;

    strSelectedNick = this->selectedItems().at(0)->text();

    QString strMe = Core::instance()->settings.value("nick");
    QString strSelfModes = Core::instance()->getUserModes(strMe, strChannel);
    int iSelfMaxModes = Core::instance()->getUserMaxModes(strSelfModes);
    QString strNickModes = Core::instance()->getUserModes(strSelectedNick, strChannel);

    QMenu *minvite = new QMenu(tr("Invite"));
    minvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));

    for (int i = 0; i < maxOpenChannels; ++i)
    {
        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
     }

    for (int i = 0; i < maxOpenChannels; ++i)
        minvite->addAction(openChannelsActs[i]);

    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    for (int i = 0; i < lOpenChannels.size(); ++i)
    {
        QString strOpenChannel = lOpenChannels[i];
        if (strOpenChannel[0] == '^')
            strOpenChannel = Core::instance()->convertPrivName(strOpenChannel);

        openChannelsActs[i]->setText(strOpenChannel);
        openChannelsActs[i]->setData(lOpenChannels[i]);
        openChannelsActs[i]->setVisible(true);
    }
    for (int j = lOpenChannels.size(); j < maxOpenChannels; ++j)
        openChannelsActs[j]->setVisible(false);

    QMenu *friends = new QMenu(tr("Friends list"));
    friends->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));
    if (Core::instance()->mFriends.contains(strSelectedNick))
        friends->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from friends"), this, SLOT(friendsDel()));
    else
        friends->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to friends"), this, SLOT(friendsAdd()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));
    if (Core::instance()->lIgnore.contains(strSelectedNick))
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from Ignore list"), this, SLOT(ignoreDel()));
    else
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to Ignore list"), this, SLOT(ignoreAdd()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));

    if ((strNickModes.contains("@")) && ((iSelfMaxModes >= 16) || (strSelectedNick == strMe)))
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(opDel()));
    else if ((!strNickModes.contains("@")) && (iSelfMaxModes >= 16))
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(opAdd()));

    if ((strNickModes.contains("%")) && ((iSelfMaxModes >= 8) || (strSelectedNick == strMe)))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfopDel()));
    else if ((!strNickModes.contains("%")) && (iSelfMaxModes >= 8))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfopAdd()));

    if ((strNickModes.contains("!")) && ((iSelfMaxModes >= 4) || (strSelectedNick == strMe)))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderatorDel()));
    else if ((!strNickModes.contains("!")) && (iSelfMaxModes >= 4))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderatorAdd()));

    if ((strNickModes.contains("+")) && ((iSelfMaxModes >= 4) || (strSelectedNick == strMe)))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voiceDel()));
    else if ((!strNickModes.contains("+")) && (iSelfMaxModes >= 4))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voiceAdd()));

    QAction *nickAct = new QAction(strSelectedNick, this);
    nickAct->setIcon(QIcon(":/images/oxygen/16x16/user-identity.png"));
    nickAct->setDisabled(true);

    QMenu *menu = new QMenu(strSelectedNick);
    menu->addAction(nickAct);
    menu->addSeparator();
    menu->addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    menu->addAction(QIcon(":/images/oxygen/16x16/text-field.png"), tr("Whois"), this, SLOT(whois()));
    if (strSelectedNick[0] != '~')
    {
        menu->addAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"), tr("Profile"), this, SLOT(profile()));
        if ((strNickModes.contains("W")) || (strNickModes.contains("V")))
            menu->addAction(QIcon(":/images/pubcam.png"), tr("Webcam"), this, SLOT(cam()));
    }
    menu->addMenu(minvite);
    if (strSelfModes.contains("r"))
    {
        menu->addMenu(friends);
        menu->addMenu(ignore);
    }
    if (iSelfMaxModes >= 4)
    {
        menu->addSeparator();
        menu->addAction(QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Kick From Channel"), this, SLOT(kick()));
        menu->addAction(QIcon(":/images/oxygen/16x16/im-ban-user.png"), tr("Ban From Channel"), this, SLOT(ban()));
        menu->addAction(QIcon(":/images/oxygen/16x16/im-ban-kick-user.png"), tr("Kick & Ban"), this, SLOT(kban()));
        menu->addAction(QIcon(":/images/oxygen/16x16/im-user-busy.png"), tr("IP Ban"), this, SLOT(ipban()));
    }
    if (!privilege->isEmpty())
    {
        menu->addSeparator();
        menu->addMenu(privilege);
    }

    menu->popup(e->globalPos());
}
