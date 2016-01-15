/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2016 Piotr Łuczko <piotr.luczko@gmail.com>
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

#ifndef ABOUT_GUI_H
#define ABOUT_GUI_H

#include <QDialog>
#include "ui_about.h"

class AboutGui : public QDialog
{
    Q_OBJECT
public:
    AboutGui(QWidget *parent = 0);

private:
    Ui::uiAbout ui;

    void createGui();
    void setDefaultValues();
    void createSignals();
};

#endif // ABOUT_GUI_H
