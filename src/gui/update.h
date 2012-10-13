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

#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QNetworkReply>
#include "ui_update.h"

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class DlgUpdate : public QDialog
{
    Q_OBJECT
public:
    DlgUpdate(QWidget *parent = 0);
    virtual ~DlgUpdate();

private:
    Ui::uiUpdate ui;
    QString strVersion;
    QNetworkAccessManager *accessManager;

    void createGui();
    void setDefaultValues();
    void createSignals();
    void gotSFSite(QString site);
    void gotFile(const QByteArray &bData);
    void showError(const QString &strError);

private slots:
    void buttonDownload();
    void networkFinished(QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived,qint64 bytesTotal);
    void downloadError(QNetworkReply::NetworkError);
};

#endif // UPDATE_H
