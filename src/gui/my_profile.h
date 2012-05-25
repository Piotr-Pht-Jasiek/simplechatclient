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

#ifndef MY_PROFILE_H
#define MY_PROFILE_H

class QNetworkAccessManager;
class QNetworkReply;
#include <QDialog>
#include "ui_my_profile.h"

class DlgMyProfile : public QDialog
{
    Q_OBJECT
public:
    DlgMyProfile(QWidget *parent = 0);
    virtual ~DlgMyProfile();

private:
    Ui::uiMyProfile ui;
    QNetworkAccessManager *accessManager;

    void createGui();
    void setDefaultValues();
    void createSignals();

    void refresh();
    int getIndex(QComboBox *comboBox, const QString &strFindText);
    QString convertTextToDesc(QString strContent);
    QString convertDescToText(QString strContent);
    QString convertCodeToCountry(const QString &strCountryCode);
    QString convertCountryToCode(const QString &strCountry);
    QString convertIntToMonth(const QString &strConvertMonth);
    QString convertMonthToInt(const QString &strMonth);
    QString convertType(const QString &strType);

private slots:
    void avatarFinished(QNetworkReply *reply);
    void buttonOk();
};

#endif // MY_PROFILE_H
