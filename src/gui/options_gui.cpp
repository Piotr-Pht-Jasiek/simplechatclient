/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QColorDialog>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QUrl>
#include "config.h"
#include "core.h"
#include "highlight.h"
#include "mainwindow.h"
#include "profile_manager_gui.h"
#include "punish_reason.h"
#include "settings.h"
#include "sound_notify.h"
#include "themes_gui.h"
#include "options_gui.h"

OptionsGui::OptionsGui(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Options"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    refreshProfilesList();
    setDefaultValues();
    createSignals();
}

void OptionsGui::createGui()
{
    ui.pushButton_profiles->setIcon(QIcon(":/images/oxygen/16x16/preferences-activities.png"));
    ui.pushButton_themes->setIcon(QIcon(":/images/oxygen/16x16/view-presentation.png"));
    ui.pushButton_highlight_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_highlight_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_punish_reason_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_punish_reason_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_reverse_colors->setIcon(QIcon(":/images/oxygen/16x16/format-stroke-color.png"));
    ui.pushButton_restore_default_colors->setIcon(QIcon(":/images/oxygen/16x16/edit-undo.png"));
    ui.pushButton_play_beep->setIcon(QIcon(":/images/oxygen/16x16/media-playback-start.png"));
    ui.pushButton_play_query->setIcon(QIcon(":/images/oxygen/16x16/media-playback-start.png"));
    ui.pushButton_sound_beep_change->setIcon(QIcon(":/images/oxygen/16x16/document-edit.png"));
    ui.pushButton_sound_query_change->setIcon(QIcon(":/images/oxygen/16x16/document-edit.png"));
    ui.pushButton_logs_open_folder->setIcon(QIcon(":/images/oxygen/16x16/folder-txt.png"));
    ui.pushButton_set_background_image->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.pushButton_set_winamp->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_mpris_format->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    // page basic
    ui.groupBox_basic->setTitle(tr("Basic"));
    ui.label_profile->setText(tr("Current profile:"));
    ui.pushButton_profiles->setText(tr("Profiles"));
    ui.label_theme->setText(tr("Current theme:"));
    ui.pushButton_themes->setText(tr("Themes"));
    ui.label_language->setText(tr("Select language:"));

    // page highlight
    ui.groupBox_highlight->setTitle(tr("Highlight text"));
    ui.pushButton_highlight_add->setText(tr("Add"));
    ui.pushButton_highlight_remove->setText(tr("Remove"));

    // page punish reason
    ui.groupBox_punish_reason->setTitle(tr("Punish reason"));
    ui.pushButton_punish_reason_add->setText(tr("Add"));
    ui.pushButton_punish_reason_remove->setText(tr("Remove"));

    // page colors
    ui.pushButton_reverse_colors->setText(tr("Reverse colors"));
    ui.pushButton_restore_default_colors->setText(tr("Restore default"));
    ui.tabWidget->setTabText(0,tr("Main window"));
    ui.tabWidget->setTabText(1,tr("Nicklist"));

    ui.label_background_color->setText(tr("Background color:"));
    ui.label_default_color->setText(tr("Default font color:"));
    ui.label_time_color->setText(tr("Time color:"));
    ui.label_join_color->setText(tr("Join color:"));
    ui.label_part_color->setText(tr("Part color:"));
    ui.label_quit_color->setText(tr("Quit color:"));
    ui.label_kick_color->setText(tr("Kick color:"));
    ui.label_mode_color->setText(tr("Mode color:"));
    ui.label_notice_color->setText(tr("Notice color:"));
    ui.label_info_color->setText(tr("Info color:"));
    ui.label_me_color->setText(tr("Me color:"));
    ui.label_error_color->setText(tr("Error color:"));
    ui.label_channel_color->setText(tr("Channel color:"));

    ui.label_nicklist_nick_color->setText(tr("Nick color:"));
    ui.label_nicklist_selected_nick_color->setText(tr("Selected nick color:"));
    ui.label_nicklist_busy_nick_color->setText(tr("Busy nick color:"));
    ui.label_nicklist_gradient_1_color->setText(tr("Gradient 1:"));
    ui.label_nicklist_gradient_2_color->setText(tr("Gradient 2:"));

    // page sounds
    ui.groupBox_sounds->setTitle(tr("Sounds"));
    ui.label_sound_beep->setText(tr("Beep:"));
    ui.label_sound_query->setText(tr("Query:"));
    ui.pushButton_sound_beep_change->setText(tr("Change"));
    ui.pushButton_sound_query_change->setText(tr("Change"));
    ui.checkBox_sound->setText(tr("Sound"));

    // page logs
    ui.groupBox_logs->setTitle(tr("Logs"));
    ui.label_logs->setText(tr("Default logs folder:"));
    ui.pushButton_logs_open_folder->setText(tr("Open folder"));
    ui.checkBox_save_logs_by_date->setText(tr("Save logs by date"));
    ui.checkBox_logs->setText(tr("Save logs"));

    // page background image
    ui.groupBox_background_image->setTitle(tr("Background image"));
    ui.label_background_image->setText(tr("Default background image:"));
    ui.pushButton_set_background_image->setText(tr("Set"));
    ui.checkBox_disable_background_image->setText(tr("Disable background image"));

    // page view
    ui.groupBox_view->setTitle(tr("View"));
    ui.checkBox_hide_formating->setText(tr("Disable font size, color..."));
    ui.checkBox_hide_join_part->setText(tr("Hide join/part"));
    ui.checkBox_hide_join_part_200->setText(tr("Hide join/part on big channels"));
    ui.checkBox_hide_emoticons->setText(tr("Hide emoticons"));
    ui.checkBox_disable_replaces->setText(tr("Disable replaces"));
    ui.checkBox_hide_nicklist->setText(tr("Hide nicklist"));
    ui.checkBox_img_thumbs->setText(tr("Image thumbnails"));
    ui.label_time_format->setText(tr("Time format:"));

    // page winamp
    ui.groupBox_winamp->setTitle(tr("Winamp"));
    ui.label_winamp_text->setText(tr("Displayed text:"));
    ui.label_winamp_version->setText(tr("$version - Winamp version"));
    ui.label_winamp_song->setText(tr("$song - current song"));
    ui.label_winamp_position->setText(tr("$position - position"));
    ui.label_winamp_length->setText(tr("$length - length"));
    ui.pushButton_set_winamp->setText(tr("Set"));

    // page mpris
    ui.groupBox_mpris->setTitle(tr("MPRIS Player"));
    ui.label_mpris_format->setText(tr("Displayed text:"));
    ui.label_mpris_syntax->setText(tr("Syntax:\n%t - song title,\n%a - album,\n%r - artist,\n%f - file name,\n"
                                      "%l - song length (MM:SS),\n%c - current song position (MM:SS),\n"
                                      "%p - percents of played song,\n%n - player name,\n%v - player version\n"));
    ui.pushButton_set_mpris_format->setText(tr("Set"));
    ui.label_mpris_player->setText(tr("Select Player:"));

    // page notification
    ui.groupBox_notification->setTitle(tr("Notification"));
    ui.checkBox_tray_message->setText(tr("Show tray notification"));

    // page updates
    ui.groupBox_updates->setTitle(tr("Updates"));
    ui.checkBox_updates->setText(tr("Check updates on start up"));

    // page adv
    ui.groupBox_adv->setTitle(tr("Advanced"));
    ui.checkBox_auto_connect->setText(tr("Connect on start up"));
    ui.checkBox_auto_busy->setText(tr("Busy mode after you log in to chat"));
    ui.checkBox_auto_away->setText(tr("Auto away on idle"));
    ui.checkBox_autojoin_favourites->setText(tr("Autojoin favourite channels"));
    ui.checkBox_minimize_to_tray->setText(tr("Minimize to tray"));
    ui.checkBox_zuo_and_ip->setText(tr("Show ZUO and IP"));

    // options list
    QListWidgetItem *basic = new QListWidgetItem(ui.listWidget_options);
    basic->setIcon(QIcon(":/images/oxygen/16x16/view-media-artist.png"));
    basic->setText(tr("Basic"));
    basic->setToolTip(tr("Basic"));

    QListWidgetItem *highlight = new QListWidgetItem(ui.listWidget_options);
    highlight->setIcon(QIcon(":/images/oxygen/16x16/feed-subscribe.png"));
    highlight->setText(tr("Highlight"));
    highlight->setToolTip(tr("Highlight"));

    QListWidgetItem *punish_reason = new QListWidgetItem(ui.listWidget_options);
    punish_reason->setIcon(QIcon(":/images/oxygen/16x16/view-conversation-balloon.png"));
    punish_reason->setText(tr("Punish reason"));
    punish_reason->setToolTip(tr("Punish reason"));

    QListWidgetItem *colors = new QListWidgetItem(ui.listWidget_options);
    colors->setIcon(QIcon(":/images/oxygen/16x16/preferences-desktop-display-color.png"));
    colors->setText(tr("Colors"));
    colors->setToolTip(tr("Colors"));

    QListWidgetItem *sounds = new QListWidgetItem(ui.listWidget_options);
    sounds->setIcon(QIcon(":/images/oxygen/16x16/media-playback-start.png"));
    sounds->setText(tr("Sounds"));
    sounds->setToolTip(tr("Sounds"));

    QListWidgetItem *logs = new QListWidgetItem(ui.listWidget_options);
    logs->setIcon(QIcon(":/images/oxygen/16x16/text-field.png"));
    logs->setText(tr("Logs"));
    logs->setToolTip(tr("Logs"));

    QListWidgetItem *background_image = new QListWidgetItem(ui.listWidget_options);
    background_image->setIcon(QIcon(":/images/oxygen/16x16/games-config-background.png"));
    background_image->setText(tr("Background image"));
    background_image->setToolTip(tr("Background image"));

    QListWidgetItem *view = new QListWidgetItem(ui.listWidget_options);
    view->setIcon(QIcon(":/images/oxygen/16x16/view-time-schedule.png"));
    view->setText(tr("View"));
    view->setToolTip(tr("View"));

    QListWidgetItem *winamp = new QListWidgetItem(ui.listWidget_options);
    winamp->setIcon(QIcon(":/images/winamp.png"));
    winamp->setText(tr("Winamp"));
    winamp->setToolTip(tr("Winamp"));

    QListWidgetItem *mpris = new QListWidgetItem(ui.listWidget_options);
    mpris->setIcon(QIcon(":/images/mpris.png"));
    mpris->setText(tr("MPRIS"));
    mpris->setToolTip(tr("MPRIS"));

#ifdef Q_WS_WIN
    mpris->setHidden(true);
#else
    winamp->setHidden(true);
#endif

    QListWidgetItem *notification = new QListWidgetItem(ui.listWidget_options);
    notification->setIcon(QIcon(":/images/oxygen/16x16/help-hint.png"));
    notification->setText(tr("Notification"));
    notification->setToolTip(tr("Notification"));

    QListWidgetItem *updates = new QListWidgetItem(ui.listWidget_options);
    updates->setIcon(QIcon(":/images/oxygen/16x16/system-software-update.png"));
    updates->setText(tr("Updates"));
    updates->setToolTip(tr("Updates"));

    QListWidgetItem *adv = new QListWidgetItem(ui.listWidget_options);
    adv->setIcon(QIcon(":/images/oxygen/16x16/dialog-warning.png"));
    adv->setText(tr("Advanced"));
    adv->setToolTip(tr("Advanced"));
}

void OptionsGui::setDefaultValues()
{
    // current option
    ui.listWidget_options->setCurrentRow(0);

    // language
    QStringList lLanguage;
    lLanguage << tr("English") << tr("Polish");
    ui.comboBox_language->clear();
    ui.comboBox_language->addItems(lLanguage);

    // highlight
    Highlight::instance()->init();

    ui.listWidget_highlight->clear();
    QList<QString> highlightList = Highlight::instance()->get();
    foreach (const QString &strHighlight, highlightList)
        ui.listWidget_highlight->addItem(strHighlight);

    // punish reason
    PunishReason::instance()->init();

    ui.listWidget_punish_reason->clear();
    QList<QString> punishList = PunishReason::instance()->get();
    foreach (const QString &strPunishReason, punishList)
        ui.listWidget_punish_reason->addItem(strPunishReason);

    // sound beep
    ui.lineEdit_sound_beep->setText(QDir::toNativeSeparators(Settings::instance()->get("sound_beep")));
    ui.lineEdit_sound_query->setText(QDir::toNativeSeparators(Settings::instance()->get("sound_query")));

    // logs
    QString strCurrentProfile = Settings::instance()->get("current_profile");
    QString strLogsPath;
#ifdef Q_WS_WIN
    strLogsPath = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    strLogsPath += "/scc/";
#else
    strLogsPath = QDir::homePath();
    strLogsPath += "/.scc/";
#endif

    strLogsPath += "profiles/"+strCurrentProfile+"/log/";

    if (!QDir().exists(strLogsPath))
        QDir().mkpath(strLogsPath);

    ui.lineEdit_logs_folder->setText(QDir::toNativeSeparators(strLogsPath));

    // background image
    ui.lineEdit_background_image->setText(QDir::toNativeSeparators(Settings::instance()->get("background_image")));

    // time format
    ui.comboBox_time_format->addItem("hh:mm:ss");
    ui.comboBox_time_format->addItem("hh:mm");

    // mpris player
    ui.comboBox_mpris_player->addItem("Amarok", "org.mpris.MediaPlayer2.amarok");
    ui.comboBox_mpris_player->addItem("Audacious", "org.mpris.MediaPlayer2.audacious");
    ui.comboBox_mpris_player->addItem("Clementine", "org.mpris.MediaPlayer2.clementine");
    ui.comboBox_mpris_player->addItem("Rhythmbox", "org.mpris.MediaPlayer2.rhythmbox");
    ui.comboBox_mpris_player->addItem("VLC", "org.mpris.MediaPlayer2.vlc");

    // default values
    QString strLanguage = Settings::instance()->get("language");

    QString strSaveLogsByDate = Settings::instance()->get("save_logs_by_date");
    QString strLogs = Settings::instance()->get("logs");

    QString strSound = Settings::instance()->get("sound");

    QString strDisableBackgroundImage = Settings::instance()->get("disable_background_image");

    QString strHideFormating = Settings::instance()->get("hide_formating");
    QString strHideJoinPart = Settings::instance()->get("hide_join_part");
    QString strHideJoinPart200 = Settings::instance()->get("hide_join_part_200");
    QString strHideEmoticons = Settings::instance()->get("hide_emoticons");
    QString strDisableReplaces = Settings::instance()->get("disable_replaces");
    QString strHideNicklist = Settings::instance()->get("hide_nicklist");
    QString strImgThumbs = Settings::instance()->get("img_thumbs");
    QString strTimeFormat = Settings::instance()->get("time_format");

    QString strWinamp = Settings::instance()->get("winamp");

    QString strMprisFormat = Settings::instance()->get("mpris_format");
    QString strMprisPlayer = Settings::instance()->get("mpris_player");

    QString strTrayMessage = Settings::instance()->get("tray_message");

    QString strUpdates = Settings::instance()->get("updates");

    QString strAutoConnect = Settings::instance()->get("auto_connect");
    QString strAutoBusy = Settings::instance()->get("auto_busy");
    QString strAutoAway = Settings::instance()->get("auto_away");
    QString strAutojoinFavourites = Settings::instance()->get("autojoin_favourites");
    QString strMinimizeToTray = Settings::instance()->get("minimize_to_tray");
    QString strZuoAndIp = Settings::instance()->get("zuo_and_ip");

    // theme
    ui.lineEdit_theme->setText(Settings::instance()->get("themes"));

    // language
    if (strLanguage == "en")
        ui.comboBox_language->setCurrentIndex(0);
    else if (strLanguage == "pl")
        ui.comboBox_language->setCurrentIndex(1);
    else
        ui.comboBox_language->setCurrentIndex(1);

    // save logs by date
    if (strSaveLogsByDate == "true")
        ui.checkBox_save_logs_by_date->setChecked(true);
    else
        ui.checkBox_save_logs_by_date->setChecked(false);

    // logs
    if (strLogs == "true")
        ui.checkBox_logs->setChecked(true);
    else
        ui.checkBox_logs->setChecked(false);

    // sound
    if (strSound == "true")
        ui.checkBox_sound->setChecked(true);
    else
        ui.checkBox_sound->setChecked(false);

    // disable background image
    if (strDisableBackgroundImage == "true")
        ui.checkBox_disable_background_image->setChecked(true);
    else
        ui.checkBox_disable_background_image->setChecked(false);

    // hide formating
    if (strHideFormating == "true")
        ui.checkBox_hide_formating->setChecked(true);
    else
        ui.checkBox_hide_formating->setChecked(false);

    // hide join part
    if (strHideJoinPart == "true")
        ui.checkBox_hide_join_part->setChecked(true);
    else
        ui.checkBox_hide_join_part->setChecked(false);

    // hide join part
    if (strHideJoinPart200 == "true")
        ui.checkBox_hide_join_part_200->setChecked(true);
    else
        ui.checkBox_hide_join_part_200->setChecked(false);

    // hide emoticons
    if (strHideEmoticons == "true")
        ui.checkBox_hide_emoticons->setChecked(true);
    else
        ui.checkBox_hide_emoticons->setChecked(false);

    // disable replaces
    if (strDisableReplaces == "true")
        ui.checkBox_disable_replaces->setChecked(true);
    else
        ui.checkBox_disable_replaces->setChecked(false);

    // hide nicklist
    if (strHideNicklist == "true")
        ui.checkBox_hide_nicklist->setChecked(true);
    else
        ui.checkBox_hide_nicklist->setChecked(false);

    // img thumbs
    if (strImgThumbs == "true")
        ui.checkBox_img_thumbs->setChecked(true);
    else
        ui.checkBox_img_thumbs->setChecked(false);

    // time format
    int index = ui.comboBox_time_format->findText(strTimeFormat);
    if (index != -1)
        ui.comboBox_time_format->setCurrentIndex(index);
    else
        ui.comboBox_time_format->setCurrentIndex(0);
    //ui.comboBox_time_format->setItemText(ui.comboBox_time_format->currentIndex(), strTimeFormat);

    // winamp
    ui.lineEdit_winamp->setText(strWinamp);

    // mpris
    ui.lineEdit_mpris_format->setText(strMprisFormat);
    ui.comboBox_mpris_player->setCurrentIndex(ui.comboBox_mpris_player->findText(strMprisPlayer));

    // tray message
    if (strTrayMessage == "true")
        ui.checkBox_tray_message->setChecked(true);
    else
        ui.checkBox_tray_message->setChecked(false);

    // updates
    if (strUpdates == "true")
        ui.checkBox_updates->setChecked(true);
    else
        ui.checkBox_updates->setChecked(false);

    // auto connect
    if (strAutoConnect == "true")
        ui.checkBox_auto_connect->setChecked(true);
    else
        ui.checkBox_auto_connect->setChecked(false);

    // auto busy
    if (strAutoBusy == "true")
        ui.checkBox_auto_busy->setChecked(true);
    else
        ui.checkBox_auto_busy->setChecked(false);

    // auto away
    if (strAutoAway == "true")
        ui.checkBox_auto_away->setChecked(true);
    else
        ui.checkBox_auto_away->setChecked(false);

    // auto join favourites
    if (strAutojoinFavourites == "true")
        ui.checkBox_autojoin_favourites->setChecked(true);
    else
        ui.checkBox_autojoin_favourites->setChecked(false);

    // minimize to tray
    if (strMinimizeToTray == "true")
        ui.checkBox_minimize_to_tray->setChecked(true);
    else
        ui.checkBox_minimize_to_tray->setChecked(false);

    // zuo & ip
    if (strZuoAndIp == "true")
        ui.checkBox_zuo_and_ip->setChecked(true);
    else
        ui.checkBox_zuo_and_ip->setChecked(false);

    // set mainwindow colors
    setMainwindowColors();

    // set nicklist colors
    setNicklistColors();

    // disable change nick if connected
    if (Settings::instance()->get("logged") == "true")
    {
        ui.comboBox_profiles->setDisabled(true);
        ui.pushButton_profiles->setDisabled(true);
        ui.lineEdit_theme->setDisabled(true);
        ui.pushButton_themes->setDisabled(true);
    }
    else
    {
        ui.comboBox_profiles->setDisabled(false);
        ui.pushButton_profiles->setDisabled(false);
        ui.lineEdit_theme->setDisabled(false);
        ui.pushButton_themes->setDisabled(false);
    }
}

void OptionsGui::createSignals()
{
    connect(ui.listWidget_options, SIGNAL(clicked(QModelIndex)), this, SLOT(changePage(QModelIndex)));

    connect(ui.comboBox_profiles, SIGNAL(activated(int)), this, SLOT(currentProfileChanged(int)));
    connect(ui.pushButton_profiles, SIGNAL(clicked()), this, SLOT(buttonProfiles()));
    connect(ui.pushButton_themes, SIGNAL(clicked()), this, SLOT(buttonThemes()));
    connect(ui.comboBox_language, SIGNAL(activated(int)), this, SLOT(languageChanged(int)));

    connect(ui.pushButton_highlight_add, SIGNAL(clicked()), this, SLOT(highlightAdd()));
    connect(ui.pushButton_highlight_remove, SIGNAL(clicked()), this, SLOT(highlightRemove()));

    connect(ui.pushButton_punish_reason_add, SIGNAL(clicked()), this, SLOT(punishReasonAdd()));
    connect(ui.pushButton_punish_reason_remove, SIGNAL(clicked()), this, SLOT(punishReasonRemove()));

    connect(ui.pushButton_reverse_colors, SIGNAL(clicked()), this, SLOT(reverseColors()));
    connect(ui.pushButton_restore_default_colors, SIGNAL(clicked()), this, SLOT(restoreDefaultColors()));
    connect(ui.pushButton_background_color, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
    connect(ui.pushButton_default_color, SIGNAL(clicked()), this, SLOT(setDefaultColor()));
    connect(ui.pushButton_time_color, SIGNAL(clicked()), this, SLOT(setTimeColor()));
    connect(ui.pushButton_join_color, SIGNAL(clicked()), this, SLOT(setJoinColor()));
    connect(ui.pushButton_part_color, SIGNAL(clicked()), this, SLOT(setPartColor()));
    connect(ui.pushButton_quit_color, SIGNAL(clicked()), this, SLOT(setQuitColor()));
    connect(ui.pushButton_kick_color, SIGNAL(clicked()), this, SLOT(setKickColor()));
    connect(ui.pushButton_mode_color, SIGNAL(clicked()), this, SLOT(setModeColor()));
    connect(ui.pushButton_notice_color, SIGNAL(clicked()), this, SLOT(setNoticeColor()));
    connect(ui.pushButton_info_color, SIGNAL(clicked()), this, SLOT(setInfoColor()));
    connect(ui.pushButton_me_color, SIGNAL(clicked()), this, SLOT(setMeColor()));
    connect(ui.pushButton_error_color, SIGNAL(clicked()), this, SLOT(setErrorColor()));
    connect(ui.pushButton_channel_color, SIGNAL(clicked()), this, SLOT(setChannelColor()));
    connect(ui.pushButton_nicklist_nick_color, SIGNAL(clicked()), this, SLOT(setNicklistNickColor()));
    connect(ui.pushButton_nicklist_selected_nick_color, SIGNAL(clicked()), this, SLOT(setNicklistSelectedNickColor()));
    connect(ui.pushButton_nicklist_busy_nick_color, SIGNAL(clicked()), this, SLOT(setNicklistBusyNickColor()));
    connect(ui.pushButton_nicklist_gradient_1_color, SIGNAL(clicked()), this, SLOT(setNicklistGradient1Color()));
    connect(ui.pushButton_nicklist_gradient_2_color, SIGNAL(clicked()), this, SLOT(setNicklistGradient2Color()));

    connect(ui.pushButton_play_beep, SIGNAL(clicked()), this, SLOT(tryPlayBeep()));
    connect(ui.pushButton_play_query, SIGNAL(clicked()), this, SLOT(tryPlayQuery()));
    connect(ui.pushButton_sound_beep_change, SIGNAL(clicked()), this, SLOT(setSoundBeep()));
    connect(ui.pushButton_sound_query_change, SIGNAL(clicked()), this, SLOT(setSoundQuery()));
    connect(ui.checkBox_sound, SIGNAL(clicked(bool)), this, SLOT(setSound(bool)));

    connect(ui.pushButton_logs_open_folder, SIGNAL(clicked()), this, SLOT(openLogsFolder()));
    connect(ui.checkBox_save_logs_by_date, SIGNAL(clicked(bool)), this, SLOT(setSaveLogsByDate(bool)));
    connect(ui.checkBox_logs, SIGNAL(clicked(bool)), this, SLOT(setLogs(bool)));

    connect(ui.pushButton_set_background_image, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
    connect(ui.checkBox_disable_background_image, SIGNAL(clicked(bool)), this, SLOT(disableBackgroundImage(bool)));

    connect(ui.checkBox_hide_formating, SIGNAL(clicked(bool)), this, SLOT(hideFormating(bool)));
    connect(ui.checkBox_hide_join_part, SIGNAL(clicked(bool)), this, SLOT(hideJoinPart(bool)));
    connect(ui.checkBox_hide_join_part_200, SIGNAL(clicked(bool)), this, SLOT(hideJoinPart200(bool)));
    connect(ui.checkBox_hide_emoticons, SIGNAL(clicked(bool)), this, SLOT(hideEmoticons(bool)));
    connect(ui.checkBox_disable_replaces, SIGNAL(clicked(bool)), this, SLOT(disableReplaces(bool)));
    connect(ui.checkBox_hide_nicklist, SIGNAL(clicked(bool)), this, SLOT(hideNicklist(bool)));
    connect(ui.checkBox_img_thumbs, SIGNAL(clicked(bool)), this, SLOT(imgThumbs(bool)));
    connect(ui.comboBox_time_format, SIGNAL(activated(int)), this, SLOT(timeFormatChanged(int)));

    connect(ui.pushButton_set_winamp, SIGNAL(clicked()), this, SLOT(setWinamp()));

    connect(ui.pushButton_set_mpris_format, SIGNAL(clicked()), this, SLOT(setMprisFormat()));
    connect(ui.comboBox_mpris_player, SIGNAL(activated(int)), this, SLOT(mprisPlayerChanged(int)));

    connect(ui.checkBox_tray_message, SIGNAL(clicked(bool)), this, SLOT(trayMessage(bool)));

    connect(ui.checkBox_updates, SIGNAL(clicked(bool)), this, SLOT(updates(bool)));

    connect(ui.checkBox_auto_connect, SIGNAL(clicked(bool)), this, SLOT(autoConnect(bool)));
    connect(ui.checkBox_auto_busy, SIGNAL(clicked(bool)), this, SLOT(autoBusy(bool)));
    connect(ui.checkBox_auto_away, SIGNAL(clicked(bool)), this, SLOT(autoAway(bool)));
    connect(ui.checkBox_autojoin_favourites, SIGNAL(clicked(bool)), this, SLOT(autojoinFavourites(bool)));
    connect(ui.checkBox_minimize_to_tray, SIGNAL(clicked(bool)), this, SLOT(minimizeToTray(bool)));
    connect(ui.checkBox_zuo_and_ip, SIGNAL(clicked(bool)), this, SLOT(setZuoAndIp(bool)));

    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void OptionsGui::refreshProfilesList()
{
    // clear
    ui.comboBox_profiles->clear();

    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc/";
#else
    path = QDir::homePath()+"/.scc/";
#endif

    path += "profiles/";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    QDir dir(path);
    dir.setSorting(QDir::Name | QDir::IgnoreCase);

    QFileInfoList list = dir.entryInfoList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    foreach (const QFileInfo &info, list)
    {
        QString strName = info.fileName();
        ui.comboBox_profiles->addItem(strName);
    }

    // set current profile
    ui.comboBox_profiles->setCurrentIndex(ui.comboBox_profiles->findText(Settings::instance()->get("current_profile")));
}

void OptionsGui::changePage(QModelIndex index)
{
    ui.stackedWidget->setCurrentIndex(index.row());
}

void OptionsGui::setCurrentProfile(int row)
{
    ui.comboBox_profiles->setCurrentIndex(row);
    currentProfileChanged(row);
}

void OptionsGui::currentProfileChanged(int row)
{
    QString profileName = ui.comboBox_profiles->itemText(row);

    if (profileName.isEmpty())
        return;

    Settings::instance()->set("current_profile", profileName); // must be before config !

    Config *pConfig = new Config(SettingsConfig);
    pConfig->set("current_profile", profileName);
    delete pConfig;

    // refresh settings
    Core::instance()->reloadSettings();

    // refresh options
    setDefaultValues();

    // refresh colors
    Core::instance()->mainWindow()->refreshColors();

    // refresh background image
    Core::instance()->mainWindow()->refreshCSS();

    // refresh tool widget values
    Core::instance()->mainWindow()->refreshToolWidgetValues();
}

void OptionsGui::buttonProfiles()
{
    ProfileManagerGui(this).exec();
}

void OptionsGui::buttonThemes()
{
    ThemesGui(this).exec();
    ui.lineEdit_theme->setText(Settings::instance()->get("themes"));
}

void OptionsGui::languageChanged(int index)
{
    QString strLanguage = "pl";

    switch (index)
    {
        case 0: strLanguage = "en"; break;
        case 1: strLanguage = "pl"; break;
    }

    Settings::instance()->set("language", strLanguage);

    Config *pConfig = new Config();
    pConfig->set("language", strLanguage);
    delete pConfig;

    ui.label_language_warning->setText(tr("Restart program to apply the changes."));
}

void OptionsGui::highlightAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing highlight text"), tr("Add highlight text:"), QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        if (!Highlight::instance()->contains(strText))
        {
            Highlight::instance()->add(strText);

            ui.listWidget_highlight->addItem(strText);
        }
    }
}

void OptionsGui::highlightRemove()
{
    QString strRemove;
    if (ui.listWidget_highlight->selectedItems().size() != 0)
        strRemove = ui.listWidget_highlight->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing highlight text"), tr("Remove highlight text:"), QLineEdit::Normal, strRemove, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Highlight::instance()->remove(strText);

        QList<QListWidgetItem*> items = ui.listWidget_highlight->findItems(strText, Qt::MatchExactly);
        foreach (QListWidgetItem *item, items)
            ui.listWidget_highlight->takeItem(ui.listWidget_highlight->row(item));
    }
}

void OptionsGui::punishReasonAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing punish reason"), tr("Add punish reason:"), QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        if (!PunishReason::instance()->contains(strText))
        {
            PunishReason::instance()->add(strText);

            ui.listWidget_punish_reason->addItem(strText);
        }
    }
}

void OptionsGui::punishReasonRemove()
{
    QString strRemove;
    if (ui.listWidget_punish_reason->selectedItems().size() != 0)
        strRemove = ui.listWidget_punish_reason->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing punish reason"), tr("Remove punish reason:"), QLineEdit::Normal, strRemove, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        PunishReason::instance()->remove(strText);

        QList<QListWidgetItem*> items = ui.listWidget_punish_reason->findItems(strText, Qt::MatchExactly);
        foreach (QListWidgetItem *item, items)
            ui.listWidget_punish_reason->takeItem(ui.listWidget_punish_reason->row(item));
    }
}

void OptionsGui::setBackgroundColor()
{
    setColor("background_color");
}

void OptionsGui::setDefaultColor()
{
    setColor("default_color");
}

void OptionsGui::setTimeColor()
{
    setColor("time_color");
}

void OptionsGui::setJoinColor()
{
    setColor("message_join_color");
}

void OptionsGui::setPartColor()
{
    setColor("message_part_color");
}

void OptionsGui::setQuitColor()
{
    setColor("message_quit_color");
}

void OptionsGui::setKickColor()
{
    setColor("message_kick_color");
}

void OptionsGui::setModeColor()
{
    setColor("message_mode_color");
}

void OptionsGui::setNoticeColor()
{
    setColor("message_notice_color");
}

void OptionsGui::setInfoColor()
{
    setColor("message_info_color");
}

void OptionsGui::setMeColor()
{
    setColor("message_me_color");
}

void OptionsGui::setErrorColor()
{
    setColor("message_error_color");
}

void OptionsGui::setChannelColor()
{
    setColor("channel_color");
}

void OptionsGui::reverseColors()
{
    Config *pConfig = new Config();

    foreach (const QString &key, getColors())
    {
         QString rColor = reverseColor(Settings::instance()->get(key));
         pConfig->set(key, rColor);
         Settings::instance()->set(key, rColor);
    }

    delete pConfig;

    // set
    setMainwindowColors();

    // set
    setNicklistColors();

    // refresh tabs
    Core::instance()->mainWindow()->refreshColors();

    // refresh
    Core::instance()->mainWindow()->refreshCSS();

    // refresh tool widget values
    Core::instance()->mainWindow()->refreshToolWidgetValues();
}

void OptionsGui::restoreDefaultColors()
{
    Config *pConfig = new Config();

    foreach (const QString &key, getColors())
    {
         pConfig->setDefault(key);
         Settings::instance()->set(key, pConfig->get(key));
    }

    delete pConfig;

    // set
    setMainwindowColors();

    // set
    setNicklistColors();

    // refresh tabs
    Core::instance()->mainWindow()->refreshColors();

    // refresh
    Core::instance()->mainWindow()->refreshCSS();

    // refresh tool widget values
    Core::instance()->mainWindow()->refreshToolWidgetValues();
}

QStringList OptionsGui::getColors()
{
    QStringList colors;
    colors << "background_color" << "default_color" << "time_color" << "message_join_color"
           << "message_part_color" << "message_quit_color" << "message_kick_color"
           << "message_mode_color" << "message_notice_color" << "message_info_color"
           << "message_me_color" << "message_error_color" << "channel_color"
           << "nicklist_nick_color" << "nicklist_selected_nick_color"
           << "nicklist_busy_nick_color" << "nicklist_gradient_1_color"
           << "nicklist_gradient_2_color";
    return colors;
}

void OptionsGui::setNicklistNickColor()
{
    setColor("nicklist_nick_color");
}

void OptionsGui::setNicklistSelectedNickColor()
{
    setColor("nicklist_selected_nick_color");
}

void OptionsGui::setNicklistBusyNickColor()
{
    setColor("nicklist_busy_nick_color");
}

void OptionsGui::setNicklistGradient1Color()
{
    setColor("nicklist_gradient_1_color");
}

void OptionsGui::setNicklistGradient2Color()
{
    setColor("nicklist_gradient_2_color");
}

void OptionsGui::tryPlayBeep()
{
    SoundNotify::instance()->play(Beep);
}

void OptionsGui::tryPlayQuery()
{
    SoundNotify::instance()->play(Query);
}

void OptionsGui::setSoundBeep()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Audio File"),
                                     ui.lineEdit_sound_beep->text().trimmed(),
                                     tr("All Files (*);;Mp3 Files (*.mp3);;Wave Files (*.wav)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Settings::instance()->set("sound_beep", fileName);

        Config *pConfig = new Config();
        pConfig->set("sound_beep", fileName);
        delete pConfig;

        ui.lineEdit_sound_beep->setText(QDir::toNativeSeparators(fileName));
    }
}

void OptionsGui::setSoundQuery()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Audio File"),
                                     ui.lineEdit_sound_query->text().trimmed(),
                                     tr("All Files (*);;Mp3 Files (*.mp3);;Wave Files (*.wav)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Settings::instance()->set("sound_query", fileName);

        Config *pConfig = new Config();
        pConfig->set("sound_query", fileName);
        delete pConfig;

        ui.lineEdit_sound_query->setText(QDir::toNativeSeparators(fileName));
    }
}

void OptionsGui::setSound(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("sound", strValue);

    Config *pConfig = new Config();
    pConfig->set("sound", strValue);
    delete pConfig;
}

void OptionsGui::openLogsFolder()
{
    QString strLogsPath = QDir::toNativeSeparators(ui.lineEdit_logs_folder->text().trimmed());

    QDesktopServices::openUrl(QUrl::fromLocalFile(strLogsPath));
}

void OptionsGui::setSaveLogsByDate(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("save_logs_by_date", strValue);

    Config *pConfig = new Config();
    pConfig->set("save_logs_by_date", strValue);
    delete pConfig;
}

void OptionsGui::setLogs(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("logs", strValue);

    Config *pConfig = new Config();
    pConfig->set("logs", strValue);
    delete pConfig;
}

void OptionsGui::setBackgroundImage()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     ui.lineEdit_background_image->text().trimmed(),
                                     tr("All Files (*);;JPG Files (*.jpg);;PNG Files (*.png);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Settings::instance()->set("background_image", fileName);

        Config *pConfig = new Config();
        pConfig->set("background_image", fileName);
        delete pConfig;

        ui.lineEdit_background_image->setText(QDir::toNativeSeparators(fileName));

        // refresh background image
        Core::instance()->mainWindow()->refreshCSS();
    }
}

void OptionsGui::disableBackgroundImage(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("disable_background_image", strValue);

    Config *pConfig = new Config();
    pConfig->set("disable_background_image", strValue);
    delete pConfig;

    // refresh background image
    Core::instance()->mainWindow()->refreshCSS();
}

void OptionsGui::hideFormating(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("hide_formating", strValue);

    Config *pConfig = new Config();
    pConfig->set("hide_formating", strValue);
    delete pConfig;
}

void OptionsGui::hideJoinPart(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("hide_join_part", strValue);

    Config *pConfig = new Config();
    pConfig->set("hide_join_part", strValue);
    delete pConfig;
}

void OptionsGui::hideJoinPart200(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("hide_join_part_200", strValue);

    Config *pConfig = new Config();
    pConfig->set("hide_join_part_200", strValue);
    delete pConfig;
}

void OptionsGui::hideEmoticons(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("hide_emoticons", strValue);

    Config *pConfig = new Config();
    pConfig->set("hide_emoticons", strValue);
    delete pConfig;
}

void OptionsGui::disableReplaces(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("disable_replaces", strValue);

    Config *pConfig = new Config();
    pConfig->set("disable_replaces", strValue);
    delete pConfig;
}

void OptionsGui::hideNicklist(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("hide_nicklist", strValue);

    Config *pConfig = new Config();
    pConfig->set("hide_nicklist", strValue);
    delete pConfig;
}

void OptionsGui::imgThumbs(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("img_thumbs", strValue);

    Config *pConfig = new Config();
    pConfig->set("img_thumbs", strValue);
    delete pConfig;
}

void OptionsGui::timeFormatChanged(int index)
{
    Q_UNUSED(index);

    QString strTimeFormat = ui.comboBox_time_format->currentText();
    Settings::instance()->set("time_format", strTimeFormat);

    Config *pConfig = new Config();
    pConfig->set("time_format", strTimeFormat);
    delete pConfig;
}

void OptionsGui::setWinamp()
{
    QString strValue = ui.lineEdit_winamp->text().trimmed();

    Settings::instance()->set("winamp", strValue);

    Config *pConfig = new Config();
    pConfig->set("winamp", strValue);
    delete pConfig;
}

void OptionsGui::setMprisFormat()
{
    QString strValue = ui.lineEdit_mpris_format->text().trimmed();

    Settings::instance()->set("mpris_format", strValue);

    Config *pConfig = new Config();
    pConfig->set("mpris_format", strValue);
    delete pConfig;
}

void OptionsGui::mprisPlayerChanged(int index)
{
    Settings::instance()->set("mpris_player", ui.comboBox_mpris_player->itemText(index));
    Settings::instance()->set("mpris_service", ui.comboBox_mpris_player->itemData(index).toString());

    Config *pConfig = new Config();
    pConfig->set("mpris_player", ui.comboBox_mpris_player->itemText(index));
    pConfig->set("mpris_service", ui.comboBox_mpris_player->itemData(index).toString());
    delete pConfig;
}

void OptionsGui::trayMessage(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("tray_message", strValue);

    Config *pConfig = new Config();
    pConfig->set("tray_message", strValue);
    delete pConfig;
}

void OptionsGui::updates(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("updates", strValue);

    Config *pConfig = new Config();
    pConfig->set("updates", strValue);
    delete pConfig;
}

void OptionsGui::autoConnect(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("auto_connect", strValue);

    Config *pConfig = new Config();
    pConfig->set("auto_connect", strValue);
    delete pConfig;
}

void OptionsGui::autoBusy(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("auto_busy", strValue);

    Config *pConfig = new Config();
    pConfig->set("auto_busy", strValue);
    delete pConfig;
}

void OptionsGui::autoAway(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("auto_away", strValue);

    Config *pConfig = new Config();
    pConfig->set("auto_away", strValue);
    delete pConfig;
}

void OptionsGui::autojoinFavourites(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("autojoin_favourites", strValue);

    Config *pConfig = new Config();
    pConfig->set("autojoin_favourites", strValue);
    delete pConfig;
}

void OptionsGui::minimizeToTray(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("minimize_to_tray", strValue);

    Config *pConfig = new Config();
    pConfig->set("minimize_to_tray", strValue);
    delete pConfig;
}

void OptionsGui::setZuoAndIp(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Settings::instance()->set("zuo_and_ip", strValue);

    Config *pConfig = new Config();
    pConfig->set("zuo_and_ip", strValue);
    delete pConfig;
}

void OptionsGui::setColor(const QString &strKey)
{
    // get current value
    QString strDefaultColor = Settings::instance()->get(strKey);

    // color dialog
    QColor cColor = QColorDialog::getColor(QColor(strDefaultColor), this);

    if (cColor.isValid())
    {
        // color to name
        QString strColor = cColor.name();

        // save
        Settings::instance()->set(strKey, strColor);

        Config *pConfig = new Config();
        pConfig->set(strKey, strColor);
        delete pConfig;

        // save to pushbutton
        QPixmap color(50,15);
        color.fill(QColor(cColor));

        if (strKey == "background_color")
            ui.pushButton_background_color->setIcon(QIcon(color));
        else if (strKey == "background_color")
            ui.pushButton_background_color->setIcon(QIcon(color));
        else if (strKey == "default_color")
            ui.pushButton_default_color->setIcon(QIcon(color));
        else if (strKey == "time_color")
            ui.pushButton_time_color->setIcon(QIcon(color));
        else if (strKey == "message_join_color")
            ui.pushButton_join_color->setIcon(QIcon(color));
        else if (strKey == "message_part_color")
            ui.pushButton_part_color->setIcon(QIcon(color));
        else if (strKey == "message_quit_color")
            ui.pushButton_quit_color->setIcon(QIcon(color));
        else if (strKey == "message_kick_color")
            ui.pushButton_kick_color->setIcon(QIcon(color));
        else if (strKey == "message_mode_color")
            ui.pushButton_mode_color->setIcon(QIcon(color));
        else if (strKey == "message_notice_color")
            ui.pushButton_notice_color->setIcon(QIcon(color));
        else if (strKey == "message_info_color")
            ui.pushButton_info_color->setIcon(QIcon(color));
        else if (strKey == "message_me_color")
            ui.pushButton_me_color->setIcon(QIcon(color));
        else if (strKey == "message_error_color")
            ui.pushButton_error_color->setIcon(QIcon(color));
        else if (strKey == "channel_color")
            ui.pushButton_channel_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_nick_color")
            ui.pushButton_nicklist_nick_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_selected_nick_color")
            ui.pushButton_nicklist_selected_nick_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_busy_nick_color")
            ui.pushButton_nicklist_busy_nick_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_gradient_1_color")
            ui.pushButton_nicklist_gradient_1_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_gradient_2_color")
            ui.pushButton_nicklist_gradient_2_color->setIcon(QIcon(color));

        // refresh
        Core::instance()->mainWindow()->refreshColors();

        // refresh
        Core::instance()->mainWindow()->refreshCSS();

        // refresh tool widget values
        Core::instance()->mainWindow()->refreshToolWidgetValues();
    }
}

void OptionsGui::setMainwindowColors()
{
    QString strBackgroundColor = Settings::instance()->get("background_color");
    QString strDefaultColor = Settings::instance()->get("default_color");
    QString strTimeColor = Settings::instance()->get("time_color");
    QString strJoinColor = Settings::instance()->get("message_join_color");
    QString strPartColor = Settings::instance()->get("message_part_color");
    QString strQuitColor = Settings::instance()->get("message_quit_color");
    QString strKickColor = Settings::instance()->get("message_kick_color");
    QString strModeColor = Settings::instance()->get("message_mode_color");
    QString strNoticeColor = Settings::instance()->get("message_notice_color");
    QString strInfoColor = Settings::instance()->get("message_info_color");
    QString strMeColor = Settings::instance()->get("message_me_color");
    QString strErrorColor = Settings::instance()->get("message_error_color");
    QString strChannelColor = Settings::instance()->get("channel_color");

    // set background color
    QPixmap bcolor(50,15);
    bcolor.fill(QColor(strBackgroundColor));
    ui.pushButton_background_color->setIcon(QIcon(bcolor));

    // set default font color
    QPixmap dfcolor(50,15);
    dfcolor.fill(QColor(strDefaultColor));
    ui.pushButton_default_color->setIcon(QIcon(dfcolor));

    // set time font color
    QPixmap tcolor(50,15);
    tcolor.fill(QColor(strTimeColor));
    ui.pushButton_time_color->setIcon(QIcon(tcolor));

    // set join font color
    QPixmap jfcolor(50,15);
    jfcolor.fill(QColor(strJoinColor));
    ui.pushButton_join_color->setIcon(QIcon(jfcolor));

    // set part font color
    QPixmap pfcolor(50,15);
    pfcolor.fill(QColor(strPartColor));
    ui.pushButton_part_color->setIcon(QIcon(pfcolor));

    // set quit font color
    QPixmap qfcolor(50,15);
    qfcolor.fill(QColor(strQuitColor));
    ui.pushButton_quit_color->setIcon(QIcon(qfcolor));

    // set kick font color
    QPixmap kfcolor(50,15);
    kfcolor.fill(QColor(strKickColor));
    ui.pushButton_kick_color->setIcon(QIcon(kfcolor));

    // set mode font color
    QPixmap mfcolor(50,15);
    mfcolor.fill(QColor(strModeColor));
    ui.pushButton_mode_color->setIcon(QIcon(mfcolor));

    // set notice font color
    QPixmap nfcolor(50,15);
    nfcolor.fill(QColor(strNoticeColor));
    ui.pushButton_notice_color->setIcon(QIcon(nfcolor));

    // set info font color
    QPixmap ifcolor(50,15);
    ifcolor.fill(QColor(strInfoColor));
    ui.pushButton_info_color->setIcon(QIcon(ifcolor));

    // set me font color
    QPixmap mecolor(50,15);
    mecolor.fill(QColor(strMeColor));
    ui.pushButton_me_color->setIcon(QIcon(mecolor));

    // set error font color
    QPixmap efcolor(50,15);
    efcolor.fill(QColor(strErrorColor));
    ui.pushButton_error_color->setIcon(QIcon(efcolor));

    // set channel font color
    QPixmap cfcolor(50,15);
    cfcolor.fill(QColor(strChannelColor));
    ui.pushButton_channel_color->setIcon(QIcon(cfcolor));
}

void OptionsGui::setNicklistColors()
{
    QString strNicklistNickColor = Settings::instance()->get("nicklist_nick_color");
    QString strNicklistSelectedNickColor = Settings::instance()->get("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = Settings::instance()->get("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = Settings::instance()->get("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = Settings::instance()->get("nicklist_gradient_2_color");

    // set nicklist nick color
    QPixmap nncolor(50,15);
    nncolor.fill(QColor(strNicklistNickColor));
    ui.pushButton_nicklist_nick_color->setIcon(QIcon(nncolor));

    // set nicklist selected nick color
    QPixmap nsncolor(50,15);
    nsncolor.fill(QColor(strNicklistSelectedNickColor));
    ui.pushButton_nicklist_selected_nick_color->setIcon(QIcon(nsncolor));

    // set nicklist busy nick color
    QPixmap nbncolor(50,15);
    nbncolor.fill(QColor(strNicklistBusyNickColor));
    ui.pushButton_nicklist_busy_nick_color->setIcon(QIcon(nbncolor));

    // set nicklist gradient 1 color
    QPixmap ng1color(50,15);
    ng1color.fill(QColor(strNicklistGradient1Color));
    ui.pushButton_nicklist_gradient_1_color->setIcon(QIcon(ng1color));

    // set nicklist gradient 2 color
    QPixmap ng2color(50,15);
    ng2color.fill(QColor(strNicklistGradient2Color));
    ui.pushButton_nicklist_gradient_2_color->setIcon(QIcon(ng2color));
}

QString OptionsGui::reverseColor(QString strColor)
{
    if (strColor.size() != 8)
    {
        strColor.remove(0,1);
        int r = 255 - (strColor.mid(0, 2).toInt(0, 16));
        int g = 255 - (strColor.mid(2, 2).toInt(0, 16));
        int b = 255 - (strColor.mid(4, 2).toInt(0, 16));

        QString sr = QString::number(r, 16).size() != 2 ? "0"+QString::number(r, 16) : QString::number(r, 16);
        QString sg = QString::number(g, 16).size() != 2 ? "0"+QString::number(g, 16) : QString::number(g, 16);
        QString sb = QString::number(b, 16).size() != 2 ? "0"+QString::number(b, 16) : QString::number(b, 16);

        return QString("#%1%2%3").arg(sr, sg, sb);
    }
    else
        return "#000000";
}
