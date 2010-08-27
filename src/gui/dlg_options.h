/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef DLG_OPTIONS_H
#define DLG_OPTIONS_H

#include <QCloseEvent>
#include <QColorDialog>
#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QShowEvent>
#include "config.h"
#include "crypt.h"
#include "dlg_register_nick.h"
#include "ui_options.h"

class DlgOptions : public QDialog
{
    Q_OBJECT
public:
    DlgOptions(QWidget *, QSettings *);

private:
    Ui::uiOptions ui;
    QWidget *myparent;
    QSettings *settings;

    void save_settings();

private slots:
    void change_page(QListWidgetItem *, QListWidgetItem *);
    void hide_pass();
    void show_pass();
    void button_register_nick();
    void set_modern_style_avatars();
    void set_modern_style_no_avatars();
    void set_classic_style();
    void auto_busy();
    void show_zuo();
    void hide_formating();
    void hide_join_part();
    void hide_join_part_200();
    void disable_avatars();
    void disable_logs();
    void disable_sounds();
    void set_my_bold(int);
    void set_my_italic(int);
    void set_my_font(QString);
    void set_my_color(int);
    void set_background_color();
    void button_ok();
    void button_cancel();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);

signals:
    void refresh_background_color();

};

#endif // DLG_OPTIONS_H
