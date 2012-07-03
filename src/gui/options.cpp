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
#include "profile_manager.h"
#include "punish_reason.h"
#include "sound_notify.h"
#include "options.h"

DlgOptions::DlgOptions(QWidget *parent) : QDialog(parent)
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

void DlgOptions::createGui()
{
    ui.pushButton_profiles->setIcon(QIcon(":/images/oxygen/16x16/preferences-activities.png"));
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
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    // page basic
    ui.groupBox_profiles->setTitle(tr("Profiles"));
    ui.label_profile->setText(tr("Current profile:"));
    ui.pushButton_profiles->setText(tr("Profiles"));
    ui.groupBox_themes->setTitle(tr("Themes"));
    ui.groupBox_language->setTitle(tr("Language"));

    // page highlight
    ui.groupBox_highlight->setTitle(tr("Highlight text"));
    ui.pushButton_highlight_add->setText(tr("Add"));
    ui.pushButton_highlight_remove->setText(tr("Remove"));

    // page punish reason
    ui.groupBox_punish_reason->setTitle(tr("Punish reason"));
    ui.pushButton_punish_reason_add->setText(tr("Add"));
    ui.pushButton_punish_reason_remove->setText(tr("Remove"));

    // page adv
    ui.checkBox_auto_busy->setText(tr("Busy mode after you log in to chat"));
    ui.checkBox_disable_autojoin_favourites->setText(tr("Disable autojoin favourite channels"));
    ui.checkBox_minimize_to_tray->setText(tr("Minimize to tray"));
    ui.checkBox_show_zuo_and_ip->setText(tr("Show ZUO and IP"));
    ui.checkBox_hide_formating->setText(tr("Disable font size, color..."));
    ui.checkBox_hide_join_part->setText(tr("Hide join/part"));
    ui.checkBox_hide_join_part_200->setText(tr("Hide join/part on big channels"));
    ui.checkBox_disable_emots->setText(tr("Disable emoticons"));
    ui.checkBox_disable_replaces->setText(tr("Disable replaces"));

    // page colors
    ui.pushButton_reverse_colors->setText(tr("Reverse colors"));
    ui.pushButton_restore_default_colors->setText(tr("Restore default"));
    ui.tabWidget->setTabText(0,tr("Main window"));
    ui.tabWidget->setTabText(1,tr("Nicklist"));

    ui.label_background_color->setText(tr("Background color:"));
    ui.label_default_color->setText(tr("Default font color:"));
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
    ui.label_sound_beep->setText(tr("Beep"));
    ui.label_sound_query->setText(tr("Query"));
    ui.pushButton_sound_beep_change->setText(tr("Change"));
    ui.pushButton_sound_query_change->setText(tr("Change"));
    ui.checkBox_disable_sounds->setText(tr("Disable sounds"));

    // page logs
    ui.groupBox_logs->setTitle(tr("Logs"));
    ui.label_logs->setText(tr("Default logs folder:"));
    ui.pushButton_logs_open_folder->setText(tr("Open folder"));
    ui.checkBox_save_logs_by_date->setText(tr("Save logs by date"));
    ui.checkBox_disable_logs->setText(tr("Disable logs"));

    // page background image
    ui.groupBox_background_image->setTitle(tr("Background image"));
    ui.label_background_image->setText(tr("Default background image:"));
    ui.pushButton_set_background_image->setText(tr("Set"));
    ui.checkBox_disable_background_image->setText(tr("Disable background image"));

    // page winamp
    ui.groupBox_winamp->setTitle(tr("Winamp"));
    ui.label_winamp_text->setText(tr("Displayed text:"));
    ui.label_winamp_version->setText(tr("$version - Winamp version"));
    ui.label_winamp_song->setText(tr("$song - current song"));
    ui.label_winamp_position->setText(tr("$position - position"));
    ui.label_winamp_length->setText(tr("$length - length"));
    ui.pushButton_set_winamp->setText(tr("Set"));

    // page notification
    ui.groupBox_notification->setTitle(tr("Notification"));
    ui.checkBox_tray_message->setText(tr("Show tray notification"));

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

    QListWidgetItem *adv = new QListWidgetItem(ui.listWidget_options);
    adv->setIcon(QIcon(":/images/oxygen/16x16/dialog-warning.png"));
    adv->setText(tr("Advanced"));
    adv->setToolTip(tr("Advanced"));

    QListWidgetItem *colors = new QListWidgetItem(ui.listWidget_options);
    colors->setIcon(QIcon(":/images/oxygen/16x16/view-media-visualization.png"));
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

    QListWidgetItem *winamp = new QListWidgetItem(ui.listWidget_options);
    winamp->setIcon(QIcon(":/images/winamp.png"));
    winamp->setText(tr("Winamp"));
    winamp->setToolTip(tr("Winamp"));

    QListWidgetItem *notification = new QListWidgetItem(ui.listWidget_options);
    notification->setIcon(QIcon(":/images/oxygen/16x16/help-hint.png"));
    notification->setText(tr("Notification"));
    notification->setToolTip(tr("Notification"));
}

void DlgOptions::setDefaultValues()
{
    // current option
    ui.listWidget_options->setCurrentRow(0);

    // themes
    QStringList lThemes;
    lThemes << "Standard" << "Origin" << "Alhena" << "Adara";
    ui.comboBox_themes->clear();
    ui.comboBox_themes->addItems(lThemes);

    // language
    QStringList lLanguage;
    lLanguage << tr("English") << tr("Polish");
    ui.comboBox_language->clear();
    ui.comboBox_language->addItems(lLanguage);

    // highlight
    Highlight::instance()->init();

    ui.listWidget_highlight->clear();
    foreach (const QString &strHighlight, Highlight::instance()->get())
        ui.listWidget_highlight->addItem(strHighlight);

    // punish reason
    PunishReason::instance()->init();

    ui.listWidget_punish_reason->clear();
    foreach (const QString &strPunishReason, PunishReason::instance()->get())
        ui.listWidget_punish_reason->addItem(strPunishReason);

    // sound beep
    ui.lineEdit_sound_beep->setText(QDir::toNativeSeparators(Core::instance()->settings.value("sound_beep")));
    ui.lineEdit_sound_query->setText(QDir::toNativeSeparators(Core::instance()->settings.value("sound_query")));

    // logs
    QString strCurrentProfile = Core::instance()->settings.value("current_profile");
    QString strLogsPath;
#ifdef Q_WS_WIN
    strLogsPath = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    strLogsPath += "/scc/profiles/"+strCurrentProfile+"/log";

    // create dir if not exist
    if (!QDir().exists(strLogsPath))
        QDir().mkpath(strLogsPath);
#else
    strLogsPath = QDir::homePath();
    strLogsPath += "/.scc/profiles/"+strCurrentProfile+"/log";

    // create dir if not exist
    if (!QDir().exists(strLogsPath))
        QDir().mkpath(strLogsPath);
#endif

    ui.lineEdit_logs_folder->setText(QDir::toNativeSeparators(strLogsPath));

    // background image
    ui.lineEdit_background_image->setText(QDir::toNativeSeparators(Core::instance()->settings.value("background_image")));

    // default values
    QString strThemes = Core::instance()->settings.value("themes");
    QString strLanguage = Core::instance()->settings.value("language");

    QString strAutoBusy = Core::instance()->settings.value("auto_busy");
    QString strDisableAutojoinFavourites = Core::instance()->settings.value("disable_autojoin_favourites");
    QString strMinimizeToTray = Core::instance()->settings.value("minimize_to_tray");
    QString strShowZuoAndIp = Core::instance()->settings.value("show_zuo_and_ip");
    QString strHideFormating = Core::instance()->settings.value("hide_formating");
    QString strHideJoinPart = Core::instance()->settings.value("hide_join_part");
    QString strHideJoinPart200 = Core::instance()->settings.value("hide_join_part_200");
    QString strDisableEmots = Core::instance()->settings.value("disable_emots");
    QString strDisableReplaces = Core::instance()->settings.value("disable_replaces");

    QString strSaveLogsByDate = Core::instance()->settings.value("save_logs_by_date");
    QString strDisableLogs = Core::instance()->settings.value("disable_logs");
    QString strDisableSounds = Core::instance()->settings.value("disable_sounds");
    QString strDisableBackgroundImage = Core::instance()->settings.value("disable_background_image");

    QString strWinamp = Core::instance()->settings.value("winamp");

    QString strTrayMessage = Core::instance()->settings.value("tray_message");

    // themes
    if (strThemes == "Standard")
        ui.comboBox_themes->setCurrentIndex(0);
    else if (strThemes == "Origin")
        ui.comboBox_themes->setCurrentIndex(1);
    else if (strThemes == "Alhena")
        ui.comboBox_themes->setCurrentIndex(2);
    else if (strThemes == "Adara")
        ui.comboBox_themes->setCurrentIndex(3);
    else
        ui.comboBox_themes->setCurrentIndex(1);

    // language
    if (strLanguage == "en")
        ui.comboBox_language->setCurrentIndex(0);
    else if (strLanguage == "pl")
        ui.comboBox_language->setCurrentIndex(1);
    else
        ui.comboBox_language->setCurrentIndex(1);

    // auto busy
    if (strAutoBusy == "true")
        ui.checkBox_auto_busy->setChecked(true);
    else
        ui.checkBox_auto_busy->setChecked(false);

    // auto join favourites
    if (strDisableAutojoinFavourites == "true")
        ui.checkBox_disable_autojoin_favourites->setChecked(true);
    else
        ui.checkBox_disable_autojoin_favourites->setChecked(false);

    // minimize to tray
    if (strMinimizeToTray == "true")
        ui.checkBox_minimize_to_tray->setChecked(true);
    else
        ui.checkBox_minimize_to_tray->setChecked(false);

    // show zuo
    if (strShowZuoAndIp == "true")
        ui.checkBox_show_zuo_and_ip->setChecked(true);
    else
        ui.checkBox_show_zuo_and_ip->setChecked(false);

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

    // disable emots
    if (strDisableEmots == "true")
        ui.checkBox_disable_emots->setChecked(true);
    else
        ui.checkBox_disable_emots->setChecked(false);

    // disable replaces
    if (strDisableReplaces == "true")
        ui.checkBox_disable_replaces->setChecked(true);
    else
        ui.checkBox_disable_replaces->setChecked(false);

    // save logs by date
    if (strSaveLogsByDate == "true")
        ui.checkBox_save_logs_by_date->setChecked(true);
    else
        ui.checkBox_save_logs_by_date->setChecked(false);

    // disable logs
    if (strDisableLogs == "true")
        ui.checkBox_disable_logs->setChecked(true);
    else
        ui.checkBox_disable_logs->setChecked(false);

    // disable sounds
    if (strDisableSounds == "true")
        ui.checkBox_disable_sounds->setChecked(true);
    else
        ui.checkBox_disable_sounds->setChecked(false);

    // disable background image
    if (strDisableBackgroundImage == "true")
        ui.checkBox_disable_background_image->setChecked(true);
    else
        ui.checkBox_disable_background_image->setChecked(false);

    // winamp
    ui.lineEdit_winamp->setText(strWinamp);

    // tray message
    if (strTrayMessage == "true")
        ui.checkBox_tray_message->setChecked(true);
    else
        ui.checkBox_tray_message->setChecked(false);

    // set mainwindow colors
    setMainwindowColors();

    // set nicklist colors
    setNicklistColors();

    // disable change nick if connected
    if (Core::instance()->settings.value("logged") == "true")
    {
        ui.groupBox_profiles->setDisabled(true);
        ui.groupBox_themes->setDisabled(true);
    }
    else
    {
        ui.groupBox_profiles->setDisabled(false);
        ui.groupBox_themes->setDisabled(false);
    }
}

void DlgOptions::createSignals()
{
    connect(ui.listWidget_options, SIGNAL(clicked(QModelIndex)), this, SLOT(changePage(QModelIndex)));
    connect(ui.comboBox_profiles, SIGNAL(activated(int)), this, SLOT(currentProfileChanged(int)));
    connect(ui.pushButton_profiles, SIGNAL(clicked()), this, SLOT(buttonProfiles()));
    connect(ui.comboBox_themes, SIGNAL(activated(int)), this, SLOT(themesChanged(int)));
    connect(ui.comboBox_language, SIGNAL(activated(int)), this, SLOT(languageChanged(int)));
    connect(ui.pushButton_highlight_add, SIGNAL(clicked()), this, SLOT(highlightAdd()));
    connect(ui.pushButton_highlight_remove, SIGNAL(clicked()), this, SLOT(highlightRemove()));
    connect(ui.pushButton_punish_reason_add, SIGNAL(clicked()), this, SLOT(punishReasonAdd()));
    connect(ui.pushButton_punish_reason_remove, SIGNAL(clicked()), this, SLOT(punishReasonRemove()));
    connect(ui.checkBox_auto_busy, SIGNAL(clicked(bool)), this, SLOT(autoBusy(bool)));
    connect(ui.checkBox_disable_autojoin_favourites, SIGNAL(clicked(bool)), this, SLOT(disableAutojoinFavourites(bool)));
    connect(ui.checkBox_minimize_to_tray, SIGNAL(clicked(bool)), this, SLOT(minimizeToTray(bool)));
    connect(ui.checkBox_show_zuo_and_ip, SIGNAL(clicked(bool)), this, SLOT(showZuoAndIp(bool)));
    connect(ui.checkBox_hide_formating, SIGNAL(clicked(bool)), this, SLOT(hideFormating(bool)));
    connect(ui.checkBox_hide_join_part, SIGNAL(clicked(bool)), this, SLOT(hideJoinPart(bool)));
    connect(ui.checkBox_hide_join_part_200, SIGNAL(clicked(bool)), this, SLOT(hideJoinPart200(bool)));
    connect(ui.checkBox_disable_emots, SIGNAL(clicked(bool)), this, SLOT(disableEmots(bool)));
    connect(ui.checkBox_disable_replaces, SIGNAL(clicked(bool)), this, SLOT(disableReplaces(bool)));
    connect(ui.pushButton_reverse_colors, SIGNAL(clicked()), this, SLOT(reverseColors()));
    connect(ui.pushButton_restore_default_colors, SIGNAL(clicked()), this, SLOT(restoreDefaultColors()));
    connect(ui.pushButton_background_color, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
    connect(ui.pushButton_default_color, SIGNAL(clicked()), this, SLOT(setDefaultColor()));
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
    connect(ui.checkBox_disable_sounds, SIGNAL(clicked(bool)), this, SLOT(disableSounds(bool)));
    connect(ui.pushButton_logs_open_folder, SIGNAL(clicked()), this, SLOT(openLogsFolder()));
    connect(ui.checkBox_save_logs_by_date, SIGNAL(clicked(bool)), this, SLOT(setSaveLogsByDate(bool)));
    connect(ui.checkBox_disable_logs, SIGNAL(clicked(bool)), this, SLOT(disableLogs(bool)));
    connect(ui.pushButton_set_background_image, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
    connect(ui.checkBox_disable_background_image, SIGNAL(clicked(bool)), this, SLOT(disableBackgroundImage(bool)));
    connect(ui.pushButton_set_winamp, SIGNAL(clicked()), this, SLOT(setWinamp()));
    connect(ui.checkBox_tray_message, SIGNAL(clicked(bool)), this, SLOT(trayMessage(bool)));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgOptions::refreshProfilesList()
{
    // clear
    ui.comboBox_profiles->clear();

    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    path += "/profiles";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    QDir dir(path);
    dir.setSorting(QDir::Name);

    QFileInfoList list = dir.entryInfoList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    foreach (QFileInfo info, list)
    {
        QString strName = info.fileName();
        ui.comboBox_profiles->addItem(strName);
    }

    // set current profile
    ui.comboBox_profiles->setCurrentIndex(ui.comboBox_profiles->findText(Core::instance()->settings.value("current_profile")));
}

void DlgOptions::changePage(QModelIndex index)
{
    ui.stackedWidget->setCurrentIndex(index.row());
}

void DlgOptions::setCurrentProfile(int row)
{
    ui.comboBox_profiles->setCurrentIndex(row);
    currentProfileChanged(row);
}

void DlgOptions::currentProfileChanged(int row)
{
    QString profileName = ui.comboBox_profiles->itemText(row);

    if (profileName.isEmpty())
        return;

    Core::instance()->settings["current_profile"] = profileName; // must be before config !

    Config *pConfig = new Config(false);
    pConfig->set("current_profile", profileName);
    delete pConfig;

    // refresh settings
    Core::instance()->createSettings();

    // refresh options
    setDefaultValues();

    // refresh colors
    Core::instance()->mainWindow()->refreshColors();

    // refresh background image
    Core::instance()->mainWindow()->refreshCSS();

    // refresh tool widget values
    Core::instance()->mainWindow()->refreshToolWidgetValues();
}

void DlgOptions::buttonProfiles()
{
    DlgProfileManager(Core::instance()->mainWindow(), this).exec();
}

void DlgOptions::themesChanged(int index)
{
    QString strTheme = "Standard";

    switch (index)
    {
        case 0: strTheme = "Standard"; break;
        case 1: strTheme = "Origin"; break;
        case 2: strTheme = "Alhena"; break;
        case 3: strTheme = "Adara"; break;
    }

    Config *pConfig = new Config();
    pConfig->set("themes", strTheme);
    Core::instance()->settings["themes"] = strTheme;
}

void DlgOptions::languageChanged(int index)
{
    QString strLanguage = "pl";

    switch (index)
    {
        case 0: strLanguage = "en"; break;
        case 1: strLanguage = "pl"; break;
    }

    Config *pConfig = new Config();
    pConfig->set("language", strLanguage);
    Core::instance()->settings["language"] = strLanguage;
    delete pConfig;

    ui.label_language_warning->setText(tr("Restart program to apply the changes."));
}

void DlgOptions::highlightAdd()
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

void DlgOptions::highlightRemove()
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

void DlgOptions::punishReasonAdd()
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

void DlgOptions::punishReasonRemove()
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

void DlgOptions::autoBusy(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("auto_busy", strValue);
    Core::instance()->settings["auto_busy"] = strValue;
    delete pConfig;
}

void DlgOptions::disableAutojoinFavourites(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("disable_autojoin_favourites", strValue);
    Core::instance()->settings["disable_autojoin_favourites"] = strValue;
    delete pConfig;
}

void DlgOptions::minimizeToTray(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("minimize_to_tray", strValue);
    Core::instance()->settings["minimize_to_tray"] = strValue;
    delete pConfig;
}

void DlgOptions::showZuoAndIp(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("show_zuo_and_ip", strValue);
    Core::instance()->settings["show_zuo_and_ip"] = strValue;
    delete pConfig;
}

void DlgOptions::hideFormating(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("hide_formating", strValue);
    Core::instance()->settings["hide_formating"] = strValue;
    delete pConfig;
}

void DlgOptions::hideJoinPart(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("hide_join_part", strValue);
    Core::instance()->settings["hide_join_part"] = strValue;
    delete pConfig;
}

void DlgOptions::hideJoinPart200(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("hide_join_part_200", strValue);
    Core::instance()->settings["hide_join_part_200"] = strValue;
    delete pConfig;
}

void DlgOptions::disableEmots(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("disable_emots", strValue);
    Core::instance()->settings["disable_emots"] = strValue;
    delete pConfig;
}

void DlgOptions::disableReplaces(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("disable_replaces", strValue);
    Core::instance()->settings["disable_replaces"] = strValue;
    delete pConfig;
}

void DlgOptions::setBackgroundColor()
{
    setColor("background_color");
}

void DlgOptions::setDefaultColor()
{
    setColor("default_color");
}

void DlgOptions::setJoinColor()
{
    setColor("message_join_color");
}

void DlgOptions::setPartColor()
{
    setColor("message_part_color");
}

void DlgOptions::setQuitColor()
{
    setColor("message_quit_color");
}

void DlgOptions::setKickColor()
{
    setColor("message_kick_color");
}

void DlgOptions::setModeColor()
{
    setColor("message_mode_color");
}

void DlgOptions::setNoticeColor()
{
    setColor("message_notice_color");
}

void DlgOptions::setInfoColor()
{
    setColor("message_info_color");
}

void DlgOptions::setMeColor()
{
    setColor("message_me_color");
}

void DlgOptions::setErrorColor()
{
    setColor("message_error_color");
}

void DlgOptions::setChannelColor()
{
    setColor("channel_color");
}

void DlgOptions::reverseColors()
{
    Config *pConfig = new Config();

    pConfig->set("background_color", reverseColor(Core::instance()->settings["background_color"]));
    Core::instance()->settings["background_color"] = reverseColor(Core::instance()->settings["background_color"]);

    pConfig->set("default_color", reverseColor(Core::instance()->settings["default_color"]));
    Core::instance()->settings["default_color"] = reverseColor(Core::instance()->settings["default_color"]);

    pConfig->set("message_join_color", reverseColor(Core::instance()->settings["message_join_color"]));
    Core::instance()->settings["message_join_color"] = reverseColor(Core::instance()->settings["message_join_color"]);

    pConfig->set("message_part_color", reverseColor(Core::instance()->settings["message_part_color"]));
    Core::instance()->settings["message_part_color"] = reverseColor(Core::instance()->settings["message_part_color"]);

    pConfig->set("message_quit_color", reverseColor(Core::instance()->settings["message_quit_color"]));
    Core::instance()->settings["message_quit_color"] = reverseColor(Core::instance()->settings["message_quit_color"]);

    pConfig->set("message_kick_color", reverseColor(Core::instance()->settings["message_kick_color"]));
    Core::instance()->settings["message_kick_color"] = reverseColor(Core::instance()->settings["message_kick_color"]);

    pConfig->set("message_mode_color", reverseColor(Core::instance()->settings["message_mode_color"]));
    Core::instance()->settings["message_mode_color"] = reverseColor(Core::instance()->settings["message_mode_color"]);

    pConfig->set("message_notice_color", reverseColor(Core::instance()->settings["message_notice_color"]));
    Core::instance()->settings["message_notice_color"] = reverseColor(Core::instance()->settings["message_notice_color"]);

    pConfig->set("message_info_color", reverseColor(Core::instance()->settings["message_info_color"]));
    Core::instance()->settings["message_info_color"] = reverseColor(Core::instance()->settings["message_info_color"]);

    pConfig->set("message_me_color", reverseColor(Core::instance()->settings["message_me_color"]));
    Core::instance()->settings["message_me_color"] = reverseColor(Core::instance()->settings["message_me_color"]);

    pConfig->set("message_error_color", reverseColor(Core::instance()->settings["message_error_color"]));
    Core::instance()->settings["message_error_color"] = reverseColor(Core::instance()->settings["message_error_color"]);

    pConfig->set("channel_color", reverseColor(Core::instance()->settings["channel_color"]));
    Core::instance()->settings["channel_color"] = reverseColor(Core::instance()->settings["channel_color"]);

    pConfig->set("nicklist_nick_color", reverseColor(Core::instance()->settings["nicklist_nick_color"]));
    Core::instance()->settings["nicklist_nick_color"] = reverseColor(Core::instance()->settings["nicklist_nick_color"]);

    pConfig->set("nicklist_selected_nick_color", reverseColor(Core::instance()->settings["nicklist_selected_nick_color"]));
    Core::instance()->settings["nicklist_selected_nick_color"] = reverseColor(Core::instance()->settings["nicklist_selected_nick_color"]);

    pConfig->set("nicklist_busy_nick_color", reverseColor(Core::instance()->settings["nicklist_busy_nick_color"]));
    Core::instance()->settings["nicklist_busy_nick_color"] = reverseColor(Core::instance()->settings["nicklist_busy_nick_color"]);

    pConfig->set("nicklist_gradient_1_color", reverseColor(Core::instance()->settings["nicklist_gradient_1_color"]));
    Core::instance()->settings["nicklist_gradient_1_color"] = reverseColor(Core::instance()->settings["nicklist_gradient_1_color"]);

    pConfig->set("nicklist_gradient_2_color", reverseColor(Core::instance()->settings["nicklist_gradient_2_color"]));
    Core::instance()->settings["nicklist_gradient_2_color"] = reverseColor(Core::instance()->settings["nicklist_gradient_2_color"]);

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

void DlgOptions::restoreDefaultColors()
{
    Config *pConfig = new Config();

    pConfig->set("background_color", "#ffffff");
    Core::instance()->settings["background_color"] = "#ffffff";

    pConfig->set("default_color", "#000000");
    Core::instance()->settings["default_color"] = "#000000";

    pConfig->set("message_join_color", "#009300");
    Core::instance()->settings["message_join_color"] = "#009300";

    pConfig->set("message_part_color", "#4733FF");
    Core::instance()->settings["message_part_color"] = "#4733FF";

    pConfig->set("message_quit_color", "#00007F");
    Core::instance()->settings["message_quit_color"] = "#00007F";

    pConfig->set("message_kick_color", "#00007F");
    Core::instance()->settings["message_kick_color"] = "#00007F";

    pConfig->set("message_mode_color", "#009300");
    Core::instance()->settings["message_mode_color"] = "#009300";

    pConfig->set("message_notice_color", "#0066FF");
    Core::instance()->settings["message_notice_color"] = "#0066FF";

    pConfig->set("message_info_color", "#666666");
    Core::instance()->settings["message_info_color"] = "#666666";

    pConfig->set("message_me_color", "#800080");
    Core::instance()->settings["message_me_color"] = "#800080";

    pConfig->set("message_error_color", "#ff0000");
    Core::instance()->settings["message_error_color"] = "#ff0000";

    pConfig->set("channel_color", "#0000ff");
    Core::instance()->settings["channel_color"] = "#0000ff";

    pConfig->set("nicklist_nick_color", "#333333");
    Core::instance()->settings["nicklist_nick_color"] = "#333333";

    pConfig->set("nicklist_selected_nick_color", "#ffffff");
    Core::instance()->settings["nicklist_selected_nick_color"] = "#ffffff";

    pConfig->set("nicklist_busy_nick_color", "#a0a0a4");
    Core::instance()->settings["nicklist_busy_nick_color"] = "#a0a0a4";

    pConfig->set("nicklist_gradient_1_color", "#77d5f7");
    Core::instance()->settings["nicklist_gradient_1_color"] = "#77d5f7";

    pConfig->set("nicklist_gradient_2_color", "#1b86b7");
    Core::instance()->settings["nicklist_gradient_2_color"] = "#1b86b7";

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

void DlgOptions::setNicklistNickColor()
{
    setColor("nicklist_nick_color");
}

void DlgOptions::setNicklistSelectedNickColor()
{
    setColor("nicklist_selected_nick_color");
}

void DlgOptions::setNicklistBusyNickColor()
{
    setColor("nicklist_busy_nick_color");
}

void DlgOptions::setNicklistGradient1Color()
{
    setColor("nicklist_gradient_1_color");
}

void DlgOptions::setNicklistGradient2Color()
{
    setColor("nicklist_gradient_2_color");
}

void DlgOptions::tryPlayBeep()
{
    SoundNotify::instance()->play(Beep);
}

void DlgOptions::tryPlayQuery()
{
    SoundNotify::instance()->play(Query);
}

void DlgOptions::setSoundBeep()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Audio File"),
                                     ui.lineEdit_sound_beep->text(),
                                     tr("All Files (*);;Mp3 Files (*.mp3);;Wave Files (*.wav)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Config *pConfig = new Config();
        pConfig->set("sound_beep", fileName);
        Core::instance()->settings["sound_beep"] = fileName;
        delete pConfig;

        ui.lineEdit_sound_beep->setText(QDir::toNativeSeparators(fileName));
    }
}

void DlgOptions::setSoundQuery()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Audio File"),
                                     ui.lineEdit_sound_query->text(),
                                     tr("All Files (*);;Mp3 Files (*.mp3);;Wave Files (*.wav)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Config *pConfig = new Config();
        pConfig->set("sound_query", fileName);
        Core::instance()->settings["sound_query"] = fileName;
        delete pConfig;

        ui.lineEdit_sound_query->setText(QDir::toNativeSeparators(fileName));
    }
}

void DlgOptions::disableSounds(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("disable_sounds", strValue);
    Core::instance()->settings["disable_sounds"] = strValue;
    delete pConfig;
}

void DlgOptions::openLogsFolder()
{
    QString strLogsPath = QDir::toNativeSeparators(ui.lineEdit_logs_folder->text());

    QDesktopServices::openUrl(QUrl::fromLocalFile(strLogsPath));
}

void DlgOptions::setSaveLogsByDate(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("save_logs_by_date", strValue);
    Core::instance()->settings["save_logs_by_date"] = strValue;
    delete pConfig;
}

void DlgOptions::disableLogs(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("disable_logs", strValue);
    Core::instance()->settings["disable_logs"] = strValue;
    delete pConfig;
}

void DlgOptions::setBackgroundImage()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     ui.lineEdit_background_image->text(),
                                     tr("All Files (*);;JPG Files (*.jpg);;PNG Files (*.png);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Config *pConfig = new Config();
        pConfig->set("background_image", fileName);
        Core::instance()->settings["background_image"] = fileName;
        delete pConfig;

        ui.lineEdit_background_image->setText(QDir::toNativeSeparators(fileName));

        // refresh background image
        Core::instance()->mainWindow()->refreshCSS();
    }
}

void DlgOptions::disableBackgroundImage(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("disable_background_image", strValue);
    Core::instance()->settings["disable_background_image"] = strValue;
    delete pConfig;

    // refresh background image
    Core::instance()->mainWindow()->refreshCSS();
}

void DlgOptions::setWinamp()
{
    QString strValue = ui.lineEdit_winamp->text();

    Config *pConfig = new Config();
    pConfig->set("winamp", strValue);
    Core::instance()->settings["winamp"] = strValue;
    delete pConfig;
}

void DlgOptions::trayMessage(bool bValue)
{
    QString strValue = (bValue ? "true" : "false");

    Config *pConfig = new Config();
    pConfig->set("tray_message", strValue);
    Core::instance()->settings["tray_message"] = strValue;
    delete pConfig;
}

void DlgOptions::setColor(const QString &strKey)
{
    // get current value
    QString strDefaultColor = Core::instance()->settings.value(strKey);

    // color dialog
    QColor cColor = QColorDialog::getColor(QColor(strDefaultColor), this);

    if (cColor.isValid())
    {
        // color to name
        QString strColor = cColor.name();

        // save
        Config *pConfig = new Config();
        pConfig->set(strKey, strColor);
        Core::instance()->settings[strKey] = strColor;
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

void DlgOptions::setMainwindowColors()
{
    QString strBackgroundColor = Core::instance()->settings.value("background_color");
    QString strDefaultColor = Core::instance()->settings.value("default_color");
    QString strJoinColor = Core::instance()->settings.value("message_join_color");
    QString strPartColor = Core::instance()->settings.value("message_part_color");
    QString strQuitColor = Core::instance()->settings.value("message_quit_color");
    QString strKickColor = Core::instance()->settings.value("message_kick_color");
    QString strModeColor = Core::instance()->settings.value("message_mode_color");
    QString strNoticeColor = Core::instance()->settings.value("message_notice_color");
    QString strInfoColor = Core::instance()->settings.value("message_info_color");
    QString strMeColor = Core::instance()->settings.value("message_me_color");
    QString strErrorColor = Core::instance()->settings.value("message_error_color");
    QString strChannelColor = Core::instance()->settings.value("channel_color");

    // set background color
    QPixmap bcolor(50,15);
    bcolor.fill(QColor(strBackgroundColor));
    ui.pushButton_background_color->setIcon(QIcon(bcolor));

    // set default font color
    QPixmap dfcolor(50,15);
    dfcolor.fill(QColor(strDefaultColor));
    ui.pushButton_default_color->setIcon(QIcon(dfcolor));

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

void DlgOptions::setNicklistColors()
{
    QString strNicklistNickColor = Core::instance()->settings.value("nicklist_nick_color");
    QString strNicklistSelectedNickColor = Core::instance()->settings.value("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = Core::instance()->settings.value("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = Core::instance()->settings.value("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = Core::instance()->settings.value("nicklist_gradient_2_color");

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

QString DlgOptions::reverseColor(QString strColor)
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
