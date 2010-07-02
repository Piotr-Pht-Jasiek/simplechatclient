/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr �uczko <piotr.luczko@gmail.com>               *
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

#include "dlg_channel_settings.h"

DlgChannelSettings::DlgChannelSettings(Network *param1, QSettings *param2)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(owner_changed()));
    QObject::connect(ui.pushButton_14, SIGNAL(clicked()), this, SLOT(remove_channel_clicked()));
    QObject::connect(ui.pushButton_19, SIGNAL(clicked()), this, SLOT(email_changed()));
    QObject::connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(www_changed()));
    QObject::connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(topic_changed()));
    QObject::connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(desc_changed()));
    QObject::connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(status_pub()));
    QObject::connect(ui.radioButton_2, SIGNAL(clicked()), this, SLOT(status_priv()));
    QObject::connect(ui.comboBox, SIGNAL(activated(int)), this, SLOT(category_changed(int)));
    QObject::connect(ui.radioButton_3, SIGNAL(clicked()), this, SLOT(guardian_inactive()));
    QObject::connect(ui.radioButton_4, SIGNAL(clicked()), this, SLOT(guardian_active()));
    QObject::connect(ui.comboBox_2, SIGNAL(activated(int)), this, SLOT(guardian_clicked(int)));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(password_changed()));
    QObject::connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(limit_changed(int)));
    QObject::connect(ui.radioButton_5, SIGNAL(clicked()), this, SLOT(moderated_inactive()));
    QObject::connect(ui.radioButton_6, SIGNAL(clicked()), this, SLOT(moderated_active()));
    QObject::connect(ui.radioButton_7, SIGNAL(clicked()), this, SLOT(auditorium_inactive()));
    QObject::connect(ui.radioButton_8, SIGNAL(clicked()), this, SLOT(auditorium_active()));

    QObject::connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(button_op_add()));
    QObject::connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(button_op_del()));
    QObject::connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(button_halfop_add()));
    QObject::connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(button_halfop_del()));
    QObject::connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(button_ban_add()));
    QObject::connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(button_ban_del()));
    QObject::connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(button_invite_add()));
    QObject::connect(ui.pushButton_13, SIGNAL(clicked()), this, SLOT(button_invite_del()));

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelSettings::set_channel(QString param1)
{
    strChannel = param1;
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::add_topic(QString strTopic)
{
    ui.plainTextEdit->clear();
    ui.plainTextEdit->insertPlainText(strTopic);
}

void DlgChannelSettings::add_pubpriv(int i)
{
    if (i == 1)
        ui.radioButton_2->setChecked(true);
    else if (i == 0)
        ui.radioButton->setChecked(true);
}

void DlgChannelSettings::add_owner(QString strNick)
{
    ui.label_13->clear();
    ui.label_13->setText(strNick);
}

void DlgChannelSettings::add_email(QString strEmail)
{
    ui.lineEdit->setText(strEmail);
}

void DlgChannelSettings::add_op(QString strNick)
{
    if (exist_item(strNick, ui.tableWidget) == false)
    {
        ui.tableWidget->insertRow(ui.tableWidget->rowCount());
        ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 0, new QTableWidgetItem(strNick));
    }
}

void DlgChannelSettings::add_halfop(QString strNick)
{
    if (exist_item(strNick, ui.tableWidget_2) == false)
    {
        ui.tableWidget_2->insertRow(ui.tableWidget_2->rowCount());
        ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount()-1, 0, new QTableWidgetItem(strNick));
    }
}

void DlgChannelSettings::add_ban(QString strNick, QString strWho, QString strDT)
{
    if (exist_item(strNick, ui.tableWidget_3) == false)
    {
        ui.tableWidget_3->insertRow(ui.tableWidget_3->rowCount());
        ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 0, new QTableWidgetItem(strNick));
        ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 1, new QTableWidgetItem(strWho));
        ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 2, new QTableWidgetItem(strDT));
    }
}

void DlgChannelSettings::add_invite(QString strNick, QString strWho, QString strDT)
{
    if (exist_item(strNick, ui.tableWidget_4) == false)
    {
        ui.tableWidget_4->insertRow(ui.tableWidget_4->rowCount());
        ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 0, new QTableWidgetItem(strNick));
        ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 1, new QTableWidgetItem(strWho));
        ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 2, new QTableWidgetItem(strDT));
    }
}

void DlgChannelSettings::add_cat(int iCatMajor, int iCatMinor)
{
    if (iCatMajor == 1) // teen
        ui.comboBox->setCurrentIndex(0);
    else if (iCatMajor == 2) // towarzyskie
        ui.comboBox->setCurrentIndex(1);
    else if (iCatMajor == 3) // erotyczne
        ui.comboBox->setCurrentIndex(2);
    else if (iCatMajor == 4) // tematyczne
        ui.comboBox->setCurrentIndex(3);
    else if (iCatMajor == 5) // regionalne
        ui.comboBox->setCurrentIndex(4);
    else
        ui.comboBox->setCurrentIndex(-1);
}

void DlgChannelSettings::add_guardian(int iGuardianLevel)
{
    if (iGuardianLevel == 0)
    {
        ui.radioButton_3->setChecked(true);
        ui.comboBox_2->setCurrentIndex(-1);
    }
    else if (iGuardianLevel == 1)
    {
        ui.radioButton_4->setChecked(true);
        ui.comboBox_2->setCurrentIndex(0);
    }
    else if (iGuardianLevel == 2)
    {
        ui.radioButton_4->setChecked(true);
        ui.comboBox_2->setCurrentIndex(1);
    }
    else if (iGuardianLevel == 3)
    {
        ui.radioButton_4->setChecked(true);
        ui.comboBox_2->setCurrentIndex(2);
    }
}

void DlgChannelSettings::add_moderated(int iModerated)
{
    if (iModerated == 0)
        ui.radioButton_5->setChecked(true);
    else if (iModerated == 1)
        ui.radioButton_6->setChecked(true);
}

void DlgChannelSettings::add_www(QString strLink)
{
    ui.lineEdit_2->clear();
    ui.lineEdit_2->setText(strLink);
}

void DlgChannelSettings::add_created(QString strTime)
{
    QDateTime dt = QDateTime::fromTime_t(strTime.toInt());
    QString strDT = dt.toString("dd/MM/yyyy hh:mm:ss");
    ui.label_14->clear();
    ui.label_14->setText(strDT);
}

void DlgChannelSettings::add_password(QString strPassword)
{
    ui.lineEdit_3->clear();
    ui.lineEdit_3->setText(strPassword);
}

void DlgChannelSettings::add_limit(int iLimit)
{
    ui.spinBox->setValue(iLimit);
}

void DlgChannelSettings::add_auditorium(int iAuditorium)
{
    if (iAuditorium == 0)
        ui.radioButton_7->setChecked(true);
    else if (iAuditorium == 1)
        ui.radioButton_8->setChecked(true);
}

void DlgChannelSettings::add_description(QString strDescription)
{
    ui.plainTextEdit_2->clear();
    ui.plainTextEdit_2->insertPlainText(strDescription);
}

void DlgChannelSettings::owner_changed()
{
    (new DlgPrivilege(pNetwork, settings, "owner", strChannel, "add", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
}

void DlgChannelSettings::remove_channel_clicked()
{
    (new DlgPrivilege(pNetwork, settings, "channel", strChannel, "del", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
}

void DlgChannelSettings::email_changed()
{
    (new DlgEmail(settings, strChannel, ui.lineEdit->text()))->show();
}

void DlgChannelSettings::www_changed()
{
    pNetwork->send(QString("CS SET %1 WWW %2").arg(strChannel).arg(ui.lineEdit_2->text()));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::topic_changed()
{
    pNetwork->send(QString("CS SET %1 TOPIC %2").arg(strChannel).arg(ui.plainTextEdit->toPlainText().replace(QRegExp("(\r|\n)"), "")));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::desc_changed()
{
    pNetwork->send(QString("CS SET %1 LONGDESC %2").arg(strChannel).arg(ui.plainTextEdit_2->toPlainText().replace(QRegExp("(\r|\n)"), "")));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::status_pub()
{
    pNetwork->send(QString("CS SET %1 PRIVATE OFF").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::status_priv()
{
    pNetwork->send(QString("CS SET %1 PRIVATE ON").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::category_changed(int index)
{
    pNetwork->send(QString("CS SET %1 CATMAJOR %2").arg(strChannel).arg(index+1));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::guardian_inactive()
{
    pNetwork->send(QString("CS SET %1 GUARDIAN 0").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::guardian_active()
{
    if (ui.comboBox_2->currentIndex() != -1)
        pNetwork->send(QString("CS SET %1 GUARDIAN %2").arg(strChannel).arg(ui.comboBox_2->currentIndex()));
    else
        pNetwork->send(QString("CS SET %1 GUARDIAN 1").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::guardian_clicked(int iLevel)
{
    if (ui.radioButton_4->isChecked() == true)
        pNetwork->send(QString("CS SET %1 GUARDIAN %2").arg(strChannel).arg(iLevel+1));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::password_changed()
{
    pNetwork->send(QString("CS SET %1 PASSWORD %2").arg(strChannel).arg(ui.lineEdit_3->text()));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::limit_changed(int iLimit)
{
    pNetwork->send(QString("CS SET %1 LIMIT %2").arg(strChannel).arg(iLimit));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::moderated_inactive()
{
    pNetwork->send(QString("CS SET %1 MODERATED OFF").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::moderated_active()
{
    pNetwork->send(QString("CS SET %1 MODERATED ON").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::auditorium_inactive()
{
    pNetwork->send(QString("CS SET %1 AUDITORIUM OFF").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::auditorium_active()
{
    pNetwork->send(QString("CS SET %1 AUDITORIUM ON").arg(strChannel));
    pNetwork->send(QString("CS INFO %1").arg(strChannel));
}

void DlgChannelSettings::button_op_add()
{
    (new DlgPrivilege(pNetwork, settings, "op", strChannel, "add", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
}

void DlgChannelSettings::button_op_del()
{
    if (ui.tableWidget->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget->selectedItems().at(0)->text();
        pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        (new DlgPrivilege(pNetwork, settings, "op", strChannel, "del", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
    }
}

void DlgChannelSettings::button_halfop_add()
{
    (new DlgPrivilege(pNetwork, settings, "halfop", strChannel, "add", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
}

void DlgChannelSettings::button_halfop_del()
{
    if (ui.tableWidget_2->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget_2->selectedItems().at(0)->text();
        pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        (new DlgPrivilege(pNetwork, settings, "halfop", strChannel, "del", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
    }
}

void DlgChannelSettings::button_ban_add()
{
    (new DlgPrivilege(pNetwork, settings, "ban", strChannel, "add", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
}

void DlgChannelSettings::button_ban_del()
{
    if (ui.tableWidget_3->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget_3->selectedItems().at(0)->text();
        pNetwork->send(QString("CS BAN %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        (new DlgPrivilege(pNetwork, settings, "ban", strChannel, "del", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
    }
}

void DlgChannelSettings::button_invite_add()
{
    (new DlgPrivilege(pNetwork, settings, "invite", strChannel, "add", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
}

void DlgChannelSettings::button_invite_del()
{
    if (ui.tableWidget_4->selectedItems().isEmpty() == false)
    {
        QString strRemoveNick = ui.tableWidget_4->selectedItems().at(0)->text();
        pNetwork->send(QString("CS INVITE %1 DEL %2").arg(strChannel).arg(strRemoveNick));
        pNetwork->send(QString("CS INFO %1").arg(strChannel));
        clear();
    }
    else
    {
        (new DlgPrivilege(pNetwork, settings, "invite", strChannel, "del", ui.tableWidget, ui.tableWidget_2, ui.tableWidget_3, ui.tableWidget_4))->show();
    }
}

void DlgChannelSettings::button_ok()
{
    strChannel.clear();
    clear();
    this->hide();
}

void DlgChannelSettings::button_cancel()
{
    strChannel.clear();
    clear();
    this->hide();
}

bool DlgChannelSettings::exist_item(QString strItem, QTableWidget *list)
{
    for (int i = 0; i < list->rowCount(); i++)
    {
        if (list->item(0, i)->text() == strItem)
            return true;
    }
    return false;
}

void DlgChannelSettings::clear()
{
    ui.tableWidget->clear();
    ui.tableWidget_2->clear();
    ui.tableWidget_3->clear();
    ui.tableWidget_4->clear();

    ui.tableWidget->setRowCount(0);
    ui.tableWidget_2->setRowCount(0);
    ui.tableWidget_3->setRowCount(0);
    ui.tableWidget_4->setRowCount(0);

    QStringList strlLabels;
    strlLabels << "Nick";

    QStringList strlLabels2;
    strlLabels2 << "Nick" << "Za�o�y�" << "Data/Czas";

    ui.tableWidget->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_2->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_3->setHorizontalHeaderLabels(strlLabels2);
    ui.tableWidget_4->setHorizontalHeaderLabels(strlLabels2);

    // prevents crash!
    ui.tableWidget->setSortingEnabled(false);
    ui.tableWidget_2->setSortingEnabled(false);
    ui.tableWidget_3->setSortingEnabled(false);
    ui.tableWidget_4->setSortingEnabled(false);

    ui.lineEdit->clear();
    ui.lineEdit_2->clear();
    ui.lineEdit_3->clear();
    ui.plainTextEdit->clear();
    ui.plainTextEdit_2->clear();
    ui.label_13->clear();
    ui.label_14->clear();
    ui.spinBox->setValue(0);
    ui.radioButton->setChecked(false);
    ui.radioButton_2->setChecked(false);
    ui.radioButton_3->setChecked(false);
    ui.radioButton_4->setChecked(false);
    ui.radioButton_5->setChecked(false);
    ui.radioButton_6->setChecked(false);
    ui.radioButton_7->setChecked(false);
    ui.radioButton_8->setChecked(false);
}

void DlgChannelSettings::showEvent(QShowEvent *event)
{
    event->accept();

    clear();
}

void DlgChannelSettings::hideEvent(QHideEvent *event)
{
    event->accept();
    strChannel.clear();
    clear();
}
