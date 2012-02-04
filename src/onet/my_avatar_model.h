/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
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

#ifndef MY_AVATAR_MODEL_H
#define MY_AVATAR_MODEL_H

#include <QRect>
#include <QString>

class MyAvatarModel
{
public:
    MyAvatarModel();
    ~MyAvatarModel();

    QString getUrl() const;
    QString getRawUrl() const;
    QString getRawUrl(int angle) const;
    int rotateLeft();
    int rotateRight();
    static QString scaledCropToString(QRect crop, QSize scaled, QSize original, int angle, bool rotate = false);
    static QRect stringToScaledCrop(QString s, QSize scaled, QSize original, int angle, bool rotate = false);
    QSize size() const;
    void debug(QString title = 0) const;

    int angle() const;
    int mApp() const;
    QString crop() const;
    QString desc() const;
    int height() const;
    QString img() const;
    int imgId() const;
    QString mSrv() const;
    int width() const;

    void setAngle(int angle);
    void setMApp(int mApp);
    void setCrop(QString crop);
    void setDesc(QString desc);
    void setHeight(int height);
    void setImg(QString img);
    void setImgId(int imgId);
    void setMSrv(QString mSrv);
    void setWidth(int width);

private:
    enum Rotate
    {
        LEFT = -1, RIGHT = 1
    };

    enum Angle
    {
        A_0 = 0, A_90 = 1, A_180 = 2, A_270 = 3
    };

    int rotate(Rotate i);

    int _angle;
    QString _crop;
    int _height;
    QString _img;
    int _imgId;
    int _width;
    QString _desc;
    int _mApp;
    QString _mSrv;

};

#endif /* MY_AVATAR_MODEL_H */
