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

#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QLocale>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>
#include "config.h"
#include "debug.h"
#include "mainwindow.h"
#include "singleapplication.h"

#ifdef Q_WS_X11
#include <QProcess>
#include <signal.h>
#endif

#ifdef Q_WS_X11
#include <QDebug>
#endif

#ifdef Q_WS_X11
void crashHandler()
{
    QString path;
#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = settings.value("Personal").toString();
    path += "/scc";
#endif

    // create dir if not exist
    QDir d(path);
    if (d.exists(path) == false)
        d.mkdir(path);

    QDir d1(path);
    if (d1.exists(path+"/log") == false)
        d1.mkdir(path+"/log");

    QProcess pProcess;

    int pid = QCoreApplication::applicationPid();
    QString strPid = QString::number(pid);
    QString strRandom;

    while(strRandom.length() < 6)
    {
        int cat = qrand() % 3;

        // 0-9
        if (cat == 0)
        {
            int i = qrand() % 9;
            QChar c = i+=48;
            strRandom += c.toAscii();
        }
        // A-Z
        else if (cat == 1)
        {
            int i = qrand() % 25;
            QChar c = i+=65;
            strRandom += c.toAscii();
        }
        // a-z
        else if (cat == 2)
        {
            int i = qrand() % 25;
            QChar c = i+=97;
            strRandom += c.toAscii();
        }
    }

    QString strCommand = "gdb --pid "+ strPid +" -ex \"set logging overwrite on\" -ex \"set logging file "+ path +"/log/crash-"+ strRandom +".txt\" -ex \"set logging on\" -ex \"backtrace\" -ex \"info registers\" -ex \"x/16i $pc\" -ex \"thread apply all backtrace\" -ex \"up\" -ex \"list\" -ex \"quit\"";

    pProcess.start(strCommand);
    pProcess.waitForFinished();

    exit(1);
}

static void got_bus(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGBUS: BUS ERROR -- CRASHING!");
    crashHandler();
}

static void got_segv(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGSEGV: SEGMENT VIOLATION -- CRASHING!");
    crashHandler();
}

static void got_fpe(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGFPE: FLOATING POINT ERROR -- CRASHING!");
    crashHandler();
}

static void got_term(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGTERM: TERMINATE SIGNAL -- SIGNING OFF!");
    crashHandler();
}

static void got_hup(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGHUP: HANGUP SIGNAL -- SIGNING OFF!");
    crashHandler();
}

static void got_ill(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGILL: ILL SIGNAL -- SIGNING OFF!");
    crashHandler();
}

static void got_abrt(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGABRT: ABRT SIGNAL -- SIGNING OFF!");
    crashHandler();
}

#endif

int main(int argc, char *argv[])
{
    bool bDebug = false;

    if (argc > 1)
    {
        for (int i = 0; i < argc; i++)
        {
            // debug
            if (strcmp(argv[i], "-debug") == 0)
            {
#ifdef Q_WS_X11
                qDebug() << "Debug: on";
#endif
                bDebug = true;
            }
        }
    }

    Q_INIT_RESOURCE(scc);

    // linux crash handler
#ifdef Q_WS_X11
    struct sigaction sv;

    // sigbus
    sv.sa_handler = got_bus;
    sigemptyset(&sv.sa_mask);
    sv.sa_flags = 0;
    sigaction(SIGBUS, &sv, NULL);
    // sigsegv
    sv.sa_handler = got_segv;
    sigaction(SIGSEGV, &sv, NULL);
    // sigfpe
    sv.sa_handler = got_fpe;
    sigaction(SIGFPE, &sv, NULL);
    // sigterm
    sv.sa_handler = got_term;
    sigaction(SIGTERM, &sv, NULL);
    // sighup
    sv.sa_handler = got_hup;
    sigaction(SIGHUP, &sv, NULL);
    // sigpipe
    sv.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sv, NULL);
    // sigill
    sv.sa_handler = got_ill;
    sigaction(SIGILL, &sv, NULL);
    // sigabrt
    sv.sa_handler = got_abrt;
    sigaction(SIGABRT, &sv, NULL);
#endif


    qInstallMsgHandler(messageHandler);
    SingleApplication app(argc, argv, "scc");

    // detect already running
    if (app.isRunning())
    {
        QMessageBox::critical(0, "Error", "Simple Chat Client is already running");
        return 0;
    }

    // set codec
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO-8859-2"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // set geometry
    const int x = QApplication::desktop()->availableGeometry().width();
    const int y = QApplication::desktop()->availableGeometry().height();
    const int xo = 800;
    const int yo = 600;

    // set organization
    QCoreApplication::setOrganizationName("scc");
    QCoreApplication::setOrganizationDomain("simplechatclien.sourceforge.net");
    QCoreApplication::setApplicationName("scc");

    // get translate
    Config *pConfig = new Config(false);
    QString strLanguage = pConfig->get_value("language");
    delete pConfig;

    // set system locale if not found config language
    if (strLanguage.isEmpty() == true)
        strLanguage = QLocale::system().name();

    // set translate
    QString strPath = QCoreApplication::applicationDirPath();

    QTranslator qtTranslator;
    qtTranslator.load(QString("%1/i18n/qt_%2").arg(strPath).arg(strLanguage));
    app.installTranslator(&qtTranslator);

    QTranslator sccTranslator;
    sccTranslator.load(QString("%1/i18n/scc_%2").arg(strPath).arg(strLanguage));
    app.installTranslator(&sccTranslator);

    // set locale
    //QLocale::setDefault(QLocale::system());

    // main window
    MainWindow mainWin;
    mainWin.set_debug(bDebug);
    mainWin.resize(xo,yo);
    mainWin.setGeometry((x-xo)/2,(y-yo)/2,xo,yo);
    mainWin.show();

    return app.exec();
}
