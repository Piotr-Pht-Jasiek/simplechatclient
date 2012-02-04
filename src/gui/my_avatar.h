/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
 *   Copyright (C) 2012 Dariusz Markowicz <dmarkowicz@alari.pl>             *
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

#ifndef DLG_MY_AVATAR_H
#define DLG_MY_AVATAR_H

#include <QDialog>

#include "avatar_client.h"
#include "my_avatar_model.h"
#include "ui_my_avatar.h"

class DlgMyAvatar : public QDialog
{
    Q_OBJECT
public:
    DlgMyAvatar(QWidget *);
    virtual ~DlgMyAvatar();

public slots:
    void getCollectionsReady(const QString &strResult);
    void getCollectionAvatarsReady(const QString &strResult);
    void getMyAvatarsReady(const QString &strResult);
    void uploadImageReady(const QString &strResult, const QString &strFileName);
    void updatePhotoReady(const QString &strResult);
    void addPhotoReady(const QString &strResult);
    void deletePhotoReady(const QString &strResult);
    void getAvatarReady(const QString &strUrl, const QByteArray &bData, AvatarClient::AvatarType type);

private:
    Ui::uiMyAvatar ui;
    AvatarClient *pAvatarClient;
    QMap<QString, int> mCollectionId; // name, id

    void createGui();
    void setDefaultValues();
    void createSignals();

    void drawMyAvatar(const QString &strUrl, const QByteArray &bData);
    void drawAvatarFromCollection(const QString &strUrl, const QByteArray &bData);
    void drawCurrentAvatar(const QString &strUrl, const QByteArray &bData);

    class MyAvatarListWidgetItem : public QListWidgetItem
    {
    public:
        MyAvatarListWidgetItem(const MyAvatarModel &avatar) : avatar(avatar) {}
        MyAvatarModel avatar;
    };

private slots:
    void refreshAvatar();
    void tabChanged(int);
    void collectionChanged(QString);
    void addAvatarClicked();
    void editAvatarClicked();
    void applyAvatarClicked();
    void removeAvatarClicked();
    void setEmptyAvatarClicked();
    void applyCollectionAvatarClicked();

};

#endif // DLG_MY_AVATAR_H
