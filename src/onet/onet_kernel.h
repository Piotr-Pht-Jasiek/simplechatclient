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

#ifndef ONET_KERNEL_H
#define ONET_KERNEL_H

class Avatar;
class DlgChannelSettings;
class DlgModeration;
class TabContainer;
#include <QMap>
#include <QObject>
#include <QStringList>

/**
 * Irc messages parser
 */
class OnetKernel : public QObject
{
    Q_OBJECT
public:
    OnetKernel(TabContainer *, DlgChannelSettings *, DlgModeration *);
    virtual ~OnetKernel();

public slots:
    void kernel(QString);

private:
    TabContainer *pTabC;
    DlgChannelSettings *pDlgChannelSettings;
    DlgModeration *pDlgModeration;

    Avatar *avatar;
    QString strData;
    QStringList strDataList;
    QMap<QString,QString> mOldNameNewName; // rename channels

    void raw_error();
    void raw_ping();
    void raw_pong();
    void raw_join();
    void raw_part();
    void raw_quit();
    void raw_kick();
    void raw_mode();
    void raw_privmsg();
    void raw_notice();
    void raw_invite();
    void raw_topic();
    void raw_invreject();
    void raw_invignore();
    void raw_modermsg();
    void raw_modernotice();
    void raw_moderate();
    void raw_kill();
    void raw_001();
    void raw_002();
    void raw_003();
    void raw_004();
    void raw_005();
    void raw_100n();
    void raw_109n();
    void raw_111n();
    void raw_112n();
    void raw_121n();
    void raw_122n();
    void raw_123n();
    void raw_131n();
    void raw_132n();
    void raw_133n();
    void raw_141n();
    void raw_142n();
    void raw_151n();
    void raw_152n();
    void raw_160n();
    void raw_161n();
    void raw_162n();
    void raw_163n();
    void raw_164n();
    void raw_165n();
    void raw_170n();
    void raw_171n();
    void raw_175n();
    void raw_176n();
    void raw_210n();
    void raw_211n();
    void raw_220n();
    void raw_221n();
    void raw_230n();
    void raw_231n();
    void raw_240n();
    void raw_241n();
    void raw_250n();
    void raw_251();
    void raw_251n();
    void raw_252();
    void raw_252n();
    void raw_253();
    void raw_253n();
    void raw_254();
    void raw_254n();
    void raw_255();
    void raw_255n();
    void raw_256();
    void raw_256n();
    void raw_257();
    void raw_257n();
    void raw_258();
    void raw_258n();
    void raw_259();
    void raw_259n();
    void raw_260n();
    void raw_261n();
    void raw_262n();
    void raw_263n();
    void raw_265();
    void raw_266();
    void raw_271();
    void raw_272();
    void raw_301();
    void raw_302();
    void raw_303();
    void raw_304();
    void raw_305();
    void raw_306();
    void raw_307();
    void raw_311();
    void raw_312();
    void raw_313();
    void raw_314();
    void raw_315();
    void raw_316();
    void raw_317();
    void raw_318();
    void raw_319();
    void raw_332();
    void raw_333();
    void raw_341();
    void raw_352();
    void raw_353();
    void raw_355();
    void raw_366();
    void raw_369();
    void raw_371();
    void raw_372();
    void raw_374();
    void raw_375();
    void raw_376();
    void raw_378();
    void raw_391();
    void raw_396();
    void raw_400n();
    void raw_401();
    void raw_401n();
    void raw_402();
    void raw_402n();
    void raw_403();
    void raw_403n();
    void raw_404n();
    void raw_405();
    void raw_406();
    void raw_406n();
    void raw_407n();
    void raw_408n();
    void raw_409n();
    void raw_412n();
    void raw_413n();
    void raw_414n();
    void raw_415n();
    void raw_416n();
    void raw_420n();
    void raw_421();
    void raw_421n();
    void raw_404();
    void raw_430n();
    void raw_431n();
    void raw_432();
    void raw_433();
    void raw_440n();
    void raw_441();
    void raw_441n();
    void raw_442();
    void raw_443();
    void raw_445();
    void raw_446();
    void raw_451();
    void raw_452n();
    void raw_454n();
    void raw_455n();
    void raw_456n();
    void raw_458n();
    void raw_459n();
    void raw_460n();
    void raw_461();
    void raw_461n();
    void raw_462();
    void raw_463n();
    void raw_464n();
    void raw_467n();
    void raw_468n();
    void raw_469n();
    void raw_470();
    void raw_471();
    void raw_472n();
    void raw_473();
    void raw_474();
    void raw_475();
    void raw_481();
    void raw_482();
    void raw_484();
    void raw_491();
    void raw_530();
    void raw_531();
    void raw_600();
    void raw_601();
    void raw_602();
    void raw_604();
    void raw_605();
    void raw_607();
    void raw_666();
    void raw_702();
    void raw_703();
    void raw_704();
    void raw_801();
    void raw_802();
    void raw_807();
    void raw_808();
    void raw_809();
    void raw_811();
    void raw_812();
    void raw_815();
    void raw_817();
    void raw_818();
    void raw_819();
    void raw_820();
    void raw_821();
    void raw_942();
    void raw_950();
    void raw_951();
    void raw_952();

private slots:
    void timerRenameChannel();

signals:
    void addUser(QString,QString,QString,bool);
    void delUser(QString,QString);
    void nicklistRefresh(QString);
    void quitUser(QString,QString);
    void changeFlag(QString,QString,QString);
    void changeFlag(QString,QString);
    void clearNicklist(QString);
    void clearChannelAllNickAvatars(QString);
};

#endif // ONET_KERNEL_H
