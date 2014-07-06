/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2014 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include "core.h"
#include "invite.h"
#include "invite_gui.h"

InviteGui::InviteGui(const QString &_strId, qint64 _iDateTime, const QString &_strNick, const QString &_strChannel, QWidget *parent) : QDialog(parent), strId(_strId), iDateTime(_iDateTime), strNick(_strNick), strChannel(_strChannel)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose); // require by show method - prevent hangup!
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Invitation"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    if (strChannel.at(0) == '^')
        ui.label_msg->setText(QString(tr("%1 invites you to priv")).arg(strNick));
    else
        ui.label_msg->setText(QString(tr("%1 invites you to channel %2")).arg(strNick, strChannel));

    // focus
    ui.buttonBox->setFocus();
    ui.buttonBox->setFocusPolicy(Qt::StrongFocus);
}

void InviteGui::createGui()
{
    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/user-online.png"));
    ui.pushButton_reject->setIcon(QIcon(":/images/oxygen/16x16/user-invisible.png"));
    ui.pushButton_ignore->setIcon(QIcon(":/images/oxygen/16x16/user-busy.png"));
    ui.toolButton_options->setIcon(QIcon(":/images/oxygen/16x16/applications-system.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_accept->setText(tr("Accept"));
    ui.pushButton_reject->setText(tr("Reject"));
    ui.pushButton_ignore->setText(tr("Ignore"));

    ui.toolButton_options->setText(tr("Options"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setText(tr("Close"));

    QMenu *optionsMenu = new QMenu(this);
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/user-properties.png"), tr("Whois"), this, SLOT(whois()));

    ui.toolButton_options->setMenu(optionsMenu);
}

void InviteGui::createSignals()
{
    connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(buttonReject()));
    connect(ui.pushButton_ignore, SIGNAL(clicked()), this, SLOT(buttonIgnore()));
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(buttonAccept()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void InviteGui::buttonAccept()
{
    Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
    Invite::instance()->remove(strId);
    this->close();
}

void InviteGui::buttonReject()
{
    Core::instance()->network->send(QString("INVREJECT %1 %2").arg(strNick, strChannel));
    Invite::instance()->remove(strId);
    this->close();
}

void InviteGui::buttonIgnore()
{
    Core::instance()->network->send(QString("INVIGNORE %1 %2").arg(strNick, strChannel));
    Invite::instance()->remove(strId);
    this->close();
}

void InviteGui::whois()
{
    Core::instance()->network->send(QString("WHOIS %1 %1").arg(strNick));
}
