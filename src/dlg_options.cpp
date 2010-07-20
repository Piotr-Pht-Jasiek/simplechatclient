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

#include "dlg_options.h"

DlgOptions::DlgOptions(QWidget *parent, QSettings *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    settings = param1;

    Config *pConfig = new Config();
    QString strNick = pConfig->get_value("login-nick");
    QString strPass = pConfig->get_value("login-pass");
    QString strAutoBusy = pConfig->get_value("auto_busy");
    QString strDebugAll = pConfig->get_value("debug_all");
    QString strShowZuo = pConfig->get_value("show_zuo");
    QString strHideFormating = pConfig->get_value("hide_formating");
    QString strHideJoinPart = pConfig->get_value("hide_join_part");
    QString strDisableAvatars = pConfig->get_value("disable_avatars");
    QString strDisableLogs = pConfig->get_value("disable_logs");
    QString strDisableSounds = pConfig->get_value("disable_sounds");
    QString strStyle = pConfig->get_value("style");
    QString strBackgroundColor = pConfig->get_value("background_color");
    delete pConfig;

    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

    ui.lineEditNick->setText(strNick);
    ui.lineEditPass->setText(strPass);
    ui.lineEditBackgroundColor->setText(strBackgroundColor);

    if (strAutoBusy == "on")
        ui.checkBox_5->setChecked(true);
    else
        ui.checkBox_5->setChecked(false);

    if (strDebugAll == "on")
        ui.checkBox->setChecked(true);
    else
        ui.checkBox->setChecked(false);

    if (strShowZuo == "on")
        ui.checkBox_2->setChecked(true);
    else
        ui.checkBox_2->setChecked(false);

    if (strHideFormating == "on")
        ui.checkBox_3->setChecked(true);
    else
        ui.checkBox_3->setChecked(false);

    if (strHideJoinPart == "on")
        ui.checkBox_4->setChecked(true);
    else
        ui.checkBox_4->setChecked(false);

    if (strDisableAvatars == "on")
        ui.checkBox_6->setChecked(true);
    else
        ui.checkBox_6->setChecked(false);

    if (strDisableLogs == "on")
        ui.checkBox_7->setChecked(true);
    else
        ui.checkBox_7->setChecked(false);

    if (strDisableSounds == "on")
        ui.checkBox_8->setChecked(true);
    else
        ui.checkBox_8->setChecked(false);

    if (strStyle == "modern")
        ui.radioButton->setChecked(true);
    else
        ui.radioButton_2->setChecked(true);

    QObject::connect(ui.checkBox_5, SIGNAL(clicked()), this, SLOT(auto_busy()));
    QObject::connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(debug_all()));
    QObject::connect(ui.checkBox_2, SIGNAL(clicked()), this, SLOT(show_zuo()));
    QObject::connect(ui.checkBox_3, SIGNAL(clicked()), this, SLOT(hide_formating()));
    QObject::connect(ui.checkBox_4, SIGNAL(clicked()), this, SLOT(hide_join_part()));
    QObject::connect(ui.checkBox_6, SIGNAL(clicked()), this, SLOT(disable_avatars()));
    QObject::connect(ui.checkBox_7, SIGNAL(clicked()), this, SLOT(disable_logs()));
    QObject::connect(ui.checkBox_8, SIGNAL(clicked()), this, SLOT(disable_sounds()));
    QObject::connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(set_modern_style()));
    QObject::connect(ui.radioButton_2, SIGNAL(clicked()), this, SLOT(set_classic_style()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgOptions::auto_busy()
{
    Config *pConfig = new Config();
    if (ui.checkBox_5->isChecked() == true)
    {
        pConfig->set_value("auto_busy", "on");
        settings->setValue("auto_busy", "on");
    }
    else
    {
        pConfig->set_value("auto_busy", "off");
        settings->setValue("auto_busy", "off");
    }
    delete pConfig;
}

void DlgOptions::debug_all()
{
    Config *pConfig = new Config();
    if (ui.checkBox->isChecked() == true)
    {
        pConfig->set_value("debug_all", "on");
        settings->setValue("debug_all", "on");
    }
    else
    {
        pConfig->set_value("debug_all", "off");
        settings->setValue("debug_all", "off");
    }
    delete pConfig;
}

void DlgOptions::show_zuo()
{
    Config *pConfig = new Config();
    if (ui.checkBox_2->isChecked() == true)
    {
        pConfig->set_value("show_zuo", "on");
        settings->setValue("show_zuo", "on");
    }
    else
    {
        pConfig->set_value("show_zuo", "off");
        settings->setValue("show_zuo", "off");
    }
    delete pConfig;
}

void DlgOptions::hide_formating()
{
    Config *pConfig = new Config();
    if (ui.checkBox_3->isChecked() == true)
    {
        pConfig->set_value("hide_formating", "on");
        settings->setValue("hide_formating", "on");
    }
    else
    {
        pConfig->set_value("hide_formating", "off");
        settings->setValue("hide_formating", "off");
    }
    delete pConfig;
}

void DlgOptions::hide_join_part()
{
    Config *pConfig = new Config();
    if (ui.checkBox_4->isChecked() == true)
    {
        pConfig->set_value("hide_join_part", "on");
        settings->setValue("hide_join_part", "on");
    }
    else
    {
        pConfig->set_value("hide_join_part", "off");
        settings->setValue("hide_join_part", "off");
    }
    delete pConfig;
}

void DlgOptions::disable_avatars()
{
    Config *pConfig = new Config();
    if (ui.checkBox_6->isChecked() == true)
    {
        pConfig->set_value("disable_avatars", "on");
        settings->setValue("disable_avatars", "on");
    }
    else
    {
        pConfig->set_value("disable_avatars", "off");
        settings->setValue("disable_avatars", "off");
    }
    delete pConfig;
}

void DlgOptions::disable_logs()
{
    Config *pConfig = new Config();
    if (ui.checkBox_7->isChecked() == true)
    {
        pConfig->set_value("disable_logs", "on");
        settings->setValue("disable_logs", "on");
    }
    else
    {
        pConfig->set_value("disable_logs", "off");
        settings->setValue("disable_logs", "off");
    }
    delete pConfig;
}

void DlgOptions::disable_sounds()
{
    Config *pConfig = new Config();
    if (ui.checkBox_8->isChecked() == true)
    {
        pConfig->set_value("disable_sounds", "on");
        settings->setValue("disable_sounds", "on");
    }
    else
    {
        pConfig->set_value("disable_sounds", "off");
        settings->setValue("disable_sounds", "off");
    }
    delete pConfig;
}

void DlgOptions::set_modern_style()
{
    Config *pConfig = new Config();
    if (ui.radioButton->isChecked() == true)
    {
        pConfig->set_value("style", "modern");
        settings->setValue("style", "modern");
    }
    delete pConfig;

    this->close(); // fix stay on top

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString(tr("Restart program to apply the changes.")));
    msgBox.exec();
}

void DlgOptions::set_classic_style()
{
    Config *pConfig = new Config();
    if (ui.radioButton_2->isChecked() == true)
    {
        pConfig->set_value("style", "classic");
        settings->setValue("style", "classic");
    }
    delete pConfig;

    this->close(); // fix stay on top

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString(tr("Restart program to apply the changes.")));
    msgBox.exec();
}

void DlgOptions::button_cancel()
{
    ui.lineEditNick->clear();
    ui.lineEditPass->clear();
    ui.lineEditBackgroundColor->clear();
    ui.buttonBox->QObject::disconnect();
    ui.checkBox->QObject::disconnect();
    ui.checkBox_2->QObject::disconnect();
    ui.checkBox_3->QObject::disconnect();
    ui.checkBox_4->QObject::disconnect();
    ui.checkBox_5->QObject::disconnect();
    ui.checkBox_6->QObject::disconnect();
    ui.checkBox_7->QObject::disconnect();
    ui.checkBox_8->QObject::disconnect();
    ui.radioButton->QObject::disconnect();
    ui.radioButton_2->QObject::disconnect();
    this->close();
}

void DlgOptions::button_ok()
{
    QString strNick = ui.lineEditNick->text();
    QString strPass = ui.lineEditPass->text();
    QString strBackgroundColor = ui.lineEditBackgroundColor->text();

    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->encrypt(strNick, strPass);
        delete pCrypt;
    }

    Config *pConfig = new Config();
    pConfig->set_value("login-nick", strNick);
    pConfig->set_value("login-pass", strPass);
    pConfig->set_value("background_color", strBackgroundColor);
    delete pConfig;

    settings->setValue("background_color", strBackgroundColor);

    ui.lineEditNick->clear();
    ui.lineEditPass->clear();
    ui.lineEditBackgroundColor->clear();
    ui.buttonBox->QObject::disconnect();
    ui.checkBox->QObject::disconnect();
    ui.checkBox_2->QObject::disconnect();
    ui.checkBox_3->QObject::disconnect();
    ui.checkBox_4->QObject::disconnect();
    ui.checkBox_5->QObject::disconnect();
    ui.checkBox_6->QObject::disconnect();
    ui.checkBox_7->QObject::disconnect();
    ui.checkBox_8->QObject::disconnect();
    ui.radioButton->QObject::disconnect();
    ui.radioButton_2->QObject::disconnect();
    this->close();
}
