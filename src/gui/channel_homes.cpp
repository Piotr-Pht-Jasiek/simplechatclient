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

#include <QDesktopWidget>
#include <QInputDialog>
#include <QMenu>
#include <QTimer>
#include "avatar.h"
#include "channel.h"
#include "core.h"
#include "channel_settings.h"
#include "channel_homes_model.h"
#include "channel_homes.h"

DlgChannelHomes::DlgChannelHomes(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("My channels"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgChannelHomes::createGui()
{
    ui.toolButton_options->setEnabled(false);

    ui.pushButton_create->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/irc-close-channel.png"));
    ui.toolButton_options->setIcon(QIcon(":/images/oxygen/16x16/applications-system.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_create->setText(tr("Create"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.toolButton_options->setText(tr("Options"));

    QMenu *optionsMenu = new QMenu(this);
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/legalmoves.png"), tr("Join"), this, SLOT(join()));
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("Settings"), this, SLOT(settings()));

    ui.toolButton_options->setMenu(optionsMenu);
}

void DlgChannelHomes::createSignals()
{
    connect(ui.listWidget_channels, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(ui.pushButton_create, SIGNAL(clicked()), this, SLOT(buttonCreate()));
    connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgChannelHomes::refresh()
{
    // ready
    if (ChannelHomes::instance()->getStatus() != StatusCompleted)
    {
        QTimer::singleShot(200, this, SLOT(refresh())); // 0.2 sec
        return;
    }

    ui.listWidget_channels->clear();

    QHashIterator<QString, OnetChannelHomes> i(ChannelHomes::instance()->get());
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();
        OnetChannelHomes onetChannelHomes = i.value();

        QString strAvatar = onetChannelHomes.avatar;
        if (!strAvatar.isEmpty())
        {
            strAvatar = Avatar::instance()->getAvatarPath(strAvatar);
        }
        else
        {
            strAvatar = ":/images/channel_avatar.png";
            Core::instance()->network->send(QString("CS INFO %1 i").arg(strChannel));
        }

        ui.listWidget_channels->addItem(new QListWidgetItem(QIcon(strAvatar), strChannel));
    }
}

void DlgChannelHomes::itemClicked(QListWidgetItem *)
{
    if (!ui.toolButton_options->isEnabled())
        ui.toolButton_options->setEnabled(true);
}

void DlgChannelHomes::buttonCreate()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), tr("Enter the name of the new channel:"), QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        ChannelHomes::instance()->clear();

        Core::instance()->network->send(QString("CS REGISTER %1").arg(strText));

        QTimer::singleShot(200, this, SLOT(refresh())); // 0.2 sec
    }
}

void DlgChannelHomes::buttonRemove()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), "<p><b>"+tr("The removal of the channel operation is irreversible!")+"</b></p><p>"+tr("Enter the name of the channel to remove:")+"</p>", QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        ChannelHomes::instance()->clear();

        Core::instance()->network->send(QString("CS DROP %1").arg(strText));

        QTimer::singleShot(200, this, SLOT(refresh())); // 0.2 sec
    }
}

void DlgChannelHomes::join()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();
        Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
    }
}

void DlgChannelHomes::settings()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();

        DlgChannelSettings(strChannel).exec();
    }
}
