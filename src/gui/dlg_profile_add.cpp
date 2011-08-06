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

#include <QDesktopWidget>
#include <QMessageBox>
#include "core.h"
#include "config.h"
#include "crypt.h"
#include "mainwindow.h"
#include "dlg_register_nick.h"
#include "dlg_profile_manager.h"
#include "dlg_profile_add.h"

DlgProfileAdd::DlgProfileAdd(MainWindow *parent, DlgProfileManager *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Add profile"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    profileManager = param1;

    createGui();
    setDefaultValues();
    createSignals();

    ui.lineEdit_nick->setFocus();
}

void DlgProfileAdd::createGui()
{
    ui.pushButton_register_nick->setIcon(QIcon(":/images/oxygen/16x16/list-add-user.png"));
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.radioButton_unregistered_nick->setText(tr("Unregistered nick"));
    ui.radioButton_registered_nick->setText(tr("Registered nick"));
    ui.pushButton_register_nick->setText(tr("Register nick"));
    ui.label_nick->setText(tr("Nick:"));
    ui.label_password->setText(tr("Password:"));
}

void DlgProfileAdd::setDefaultValues()
{
    ui.radioButton_unregistered_nick->setChecked(true);
    hidePass();
}

void DlgProfileAdd::createSignals()
{
    QObject::connect(ui.radioButton_unregistered_nick, SIGNAL(clicked()), this, SLOT(hidePass()));
    QObject::connect(ui.radioButton_registered_nick, SIGNAL(clicked()), this, SLOT(showPass()));
    QObject::connect(ui.pushButton_register_nick, SIGNAL(clicked()), this, SLOT(buttonRegisterNick()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(buttonOk()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgProfileAdd::setNickPass(QString n, QString p)
{
    ui.radioButton_registered_nick->setChecked(true);
    showPass();

    ui.lineEdit_nick->setText(n);
    ui.lineEdit_password->setText(p);
}

void DlgProfileAdd::hidePass()
{
    ui.label_password->hide();
    ui.lineEdit_password->hide();

    QString strNick = ui.lineEdit_nick->text();

    // fix nick
    if ((!strNick.isEmpty()) && (strNick[0] != '~'))
        ui.lineEdit_nick->setText("~"+strNick);

    ui.lineEdit_nick->setFocus();
}

void DlgProfileAdd::showPass()
{
    ui.label_password->show();
    ui.lineEdit_password->show();

    QString strNick = ui.lineEdit_nick->text();

    // fix nick
    if ((!strNick.isEmpty()) && (strNick[0] == '~'))
        ui.lineEdit_nick->setText(strNick.remove(0,1));

    ui.lineEdit_nick->setFocus();
}

void DlgProfileAdd::buttonRegisterNick()
{
    DlgRegisterNick(Core::instance()->sccWindow(), this).exec();
}

void DlgProfileAdd::buttonOk()
{
    QString strNick = ui.lineEdit_nick->text();
    QString strPassword = ui.lineEdit_password->text();

    // empty
    if (strNick.isEmpty())
    {
        QMessageBox::critical(0, "", tr("Nick is empty!"));
        return;
    }

    // check nick
    QString strPass;
    if (ui.radioButton_unregistered_nick->isChecked())
    {
        // fix nick
        if (strNick[0] != '~')
            strNick = "~"+strNick;

        strPass = "";
    }
    else if (ui.radioButton_registered_nick->isChecked())
    {
        // fix nick
        if (strNick[0] == '~')
            strNick.remove(0,1);

        strPass = ui.lineEdit_password->text();
    }

    // fix nick
    if ((strPass.isEmpty()) && (strNick[0] != '~'))
        strNick = '~'+strNick;

    // fix too long nick
    if ((strPass.isEmpty()) && (strNick.size() > 32))
        strNick = strNick.left(32);

    // exist profile
    if (profileManager->existProfile(strNick))
    {
        QMessageBox::critical(0, "", tr("Profile already exists!"));
        return;
    }

    // encrypt pass
    if (!strPass.isEmpty())
    {
        Crypt *pCrypt = new Crypt();
        strPassword = pCrypt->encrypt(strNick, strPassword);
        delete pCrypt;
    }

    // save
    Config *pConfig = new Config(true, strNick);
    pConfig->setValue("nick", strNick);
    pConfig->setValue("pass", strPassword);
    delete pConfig;

    // refresh
    profileManager->refreshAllLists();

    // close
    this->close();
}