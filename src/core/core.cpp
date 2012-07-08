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

#include <QAction>
#include <QDir>
#include <QSettings>
#include <QTcpSocket>
#include "config.h"
#include "log.h"
#include "mainwindow.h"
#include "updates.h"
#include "core.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

Core * Core::Instance = 0;

Core * Core::instance()
{
    if (!Instance)
    {
        Instance = new Core();
        Instance->init();
    }

    return Instance;
}

Core::Core() : window(0)
{
}

Core::~Core()
{
    delete window;
    window = 0;

    kamerzystaSocket->deleteLater();

    // close network
    settings["reconnect"] = "false";
    pNetwork->disconnect();
    pNetwork->quit();
    pNetwork->wait();
    pNetwork->deleteLater();
    pNetwork->QObject::disconnect();
    delete pNetwork;
}

void Core::init()
{
    // clear old settings
    QSettings oldSettings;
    oldSettings.clear();

    // remove old config
    removeOldConfig();

    // convert old profiles
    convertOldProfiles();

    // create settings
    createSettings();

    // kamerzysta
    kamerzystaSocket = new QTcpSocket();
    kamerzystaSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    kamerzystaSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    // network
    QString strServer = "czat-app.onet.pl";
    int iPort = 5015;
    pNetwork = new Network(strServer, iPort);
    pNetwork->start(QThread::InheritPriority);
}

void Core::createAndShowGui()
{
    window = new MainWindow(0);
    window->show();

    // updates
    Updates::instance()->checkUpdate();
}

void Core::createSettings()
{
    // default settings
    settings["version"] = "1.5.0.0";
    settings["available_version"] = "";
    settings["whats_new"] = "";
    settings["motd"] = "";
    settings["version_status"] = "unknown";
    settings["update_url"] = "";
    settings["logged"] = "false";
    settings["busy"] = "false";
    settings["away"] = "false";
    settings["override"] = "false";
    settings["ignore_raw_141"] = "false";
    settings["age_check"] = "true";
    settings["first_run"] = "true";
    settings["uokey"] = "";
    settings["uo_nick"] = "";
    settings["onet_ubi"] = "";
    settings["onet_cid"] = "";
    settings["onet_sid"] = "";
    settings["onet_uid"] = "";
    settings["onetzuo_ticket"] = "";
    settings["last_active"] = "0";
    settings["reconnect"] = "true";

    // read config
    configValues();

    // convert old config profile
    convertOldConfig();

    // read config profile
    configProfileValues();

    // check settings
    fixSettings();
}

void Core::removeOldConfig()
{
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // remove file
    if (QFile::exists(path+"/scc.conf"))
        QFile::remove(path+"/scc.conf");
}

void Core::convertOldProfiles()
{
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    path += "/profiles";

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QStringList("*.xml"), QDir::Files | QDir::NoSymLinks);
    foreach (QFileInfo info, list)
    {
        QString profile = info.fileName().remove(".xml");
        if (!QDir().exists(path+"/"+profile))
        {
            QDir().mkpath(path+"/"+profile);
            QFile::rename(path+"/"+profile+".xml", path+"/"+profile+"/profile.xml");
        }
        else
            QFile::remove(path+"/"+profile+".xml");
    }
}

void Core::convertOldConfig()
{
    Config *pConfig = new Config();

    // change on off -> true false
    QHash<QString,QString> lConfigValues = pConfig->read();

    QHashIterator <QString, QString> i(lConfigValues);
    while (i.hasNext())
    {
        i.next();
        QString strKey = i.key();
        QString strValue = i.value();

        if (strValue == "on")
            pConfig->set(strKey, "true");
        else if (strValue == "off")
            pConfig->set(strKey, "false");
   }

    delete pConfig;
}

void Core::configValues()
{
    // config values
    Config *pConfig = new Config(false);
    QHash<QString,QString> lConfigValues = pConfig->read();
    delete pConfig;

    // set settings
    QHashIterator <QString, QString> i(lConfigValues);
    while (i.hasNext())
    {
        i.next();
        settings[i.key()] = i.value();
    }
}

void Core::configProfileValues()
{
    // config profile values
    Config *pConfigProfile = new Config();
    QHash<QString,QString> lConfigProfileValues = pConfigProfile->read();
    delete pConfigProfile;

    // set profile settings
    QHashIterator <QString, QString> ip(lConfigProfileValues);
    while (ip.hasNext())
    {
        ip.next();
        settings[ip.key()] = ip.value();
    }
}

void Core::fixSettings()
{
    Config *pConfig = new Config();

    if (!QFile::exists(settings.value("sound_beep")))
    {
        pConfig->set("sound_beep", "");
        settings["sound_beep"] = "";
    }
    if (!QFile::exists(settings.value("sound_query")))
    {
        pConfig->set("sound_query", "");
        settings["sound_query"] = "";
    }
    if (!QFile::exists(settings.value("background_image")))
    {
        pConfig->set("background_image", "");
        settings["background_image"] = "";
    }
    if ((settings.value("themes") != "Standard") && (settings.value("themes") != "Origin") && (settings.value("themes") != "Alhena") && (settings.value("themes") != "Adara"))
    {
        pConfig->set("themes", "Standard");
        settings["themes"] = "Standard";
    }

    delete pConfig;
}

QString Core::version()
{
    return settings.value("version");
}

void Core::setDebug(bool b)
{
    if (b == true)
        settings["debug"] = "true";
    else
        settings["debug"] = "false";
}

MainWindow *Core::mainWindow()
{
    return window;
}

void Core::quit()
{
    if (!Instance)
        return;

    delete Instance;
    Instance = 0;
}

QString Core::getChannelNameFromIndex(int index)
{
    if (index < lOpenChannels.size())
        return lOpenChannels.at(index);
    else
        return QString::null;
}

int Core::getIndexFromChannelName(const QString &strChannel)
{
    for (int i = 0; i < lOpenChannels.size(); i++)
    {
        QString strOpenChannel = lOpenChannels.at(i);
        if (strOpenChannel == strChannel)
            return i;
    }

    return -1;
}

QString Core::getCurrentChannelName()
{
    int index = window->getCurrentTabIndex();
    return getChannelNameFromIndex(index);
}

QString Core::convertPrivName(const QString &strChannel)
{
    if (mPrivNames.contains(strChannel))
        return mPrivNames.value(strChannel);
    else
        return strChannel;
}

bool Core::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName))
    {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir())
                result = removeDir(info.absoluteFilePath());
            else
                result = QFile::remove(info.absoluteFilePath());

            if (!result)
                return result;
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
