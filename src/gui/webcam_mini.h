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

#ifndef WEBCAM_MINI_H
#define WEBCAM_MINI_H

#include <QDialog>
#include "ui_webcam_mini.h"

class DlgWebcamMini : public QDialog
{
    Q_OBJECT
public:
    DlgWebcamMini(QString nick);

public slots:
    void updateImage(const QByteArray &data);
    void updateText(const QString &text);
    void error(const QString &s);

private:
    Ui::uiWebcamMini ui;

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void closeCam();
};

#endif // WEBCAM_MINI_H
