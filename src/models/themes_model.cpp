/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include "settings.h"
#include "themes_model.h"

ThemesModel * ThemesModel::Instance = 0;

ThemesModel * ThemesModel::instance()
{
    if (!Instance)
    {
        Instance = new ThemesModel();
        Instance->init();
    }

    return Instance;
}

ThemesModel::ThemesModel()
{
    add("Standard", ThemeFlags::Default);
    add("Alhena");
    add("Origin", ThemeFlags::NicklistAvatar);
    add("Adara", ThemeFlags::ChannelAvatar);
}

void ThemesModel::init()
{
    refreshCurrent();
}

void ThemesModel::add(const QString &name, ThemeFlags::Flags flags)
{
    Theme add;
    add.name = name;
    add.flags = flags;

    lThemes.append(add);
}

Theme ThemesModel::get()
{
    QString currentName = Settings::instance()->get("themes");

    foreach (Theme _Theme, lThemes)
    {
        if (_Theme.name == currentName)
            return _Theme;
    }

    foreach (Theme _Theme, lThemes)
    {
        if (_Theme.flags & ThemeFlags::Default)
            return _Theme;
    }

	return lThemes.at(0);
}

void ThemesModel::refreshCurrent()
{
    current = get();
}

QList<QString> ThemesModel::getAll()
{
    QList<QString> lThemesName;

    foreach (Theme _Theme, lThemes)
    {
        lThemesName.append(_Theme.name);
    }

    return lThemesName;
}

bool ThemesModel::isCurrentWithAvatar()
{
    if (current.flags & (ThemeFlags::ChannelAvatar | ThemeFlags::NicklistAvatar))
        return true;
    else
        return false;
}

bool ThemesModel::isCurrentWithChannelAvatar()
{
    return current.flags & ThemeFlags::ChannelAvatar;
}

bool ThemesModel::isCurrentWithNicklistAvatar()
{
    return current.flags & ThemeFlags::NicklistAvatar;
}
