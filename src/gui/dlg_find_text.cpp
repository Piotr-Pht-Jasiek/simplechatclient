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

#include <QDesktopWidget>
#include "maintextedit.h"
#include "mainwindow.h"
#include "dlg_find_text.h"

DlgFindText::DlgFindText(MainWindow *parent, MainTextEdit *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Find"));
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    textEdit = param1;

    create_gui();
    create_signals();
}

void DlgFindText::create_gui()
{
    ui.label_find->setText(tr("Find what:"));
    ui.checkBox_match_whole_word_only->setText(tr("Match whole word only"));
    ui.checkBox_match_case->setText(tr("Match case"));
    ui.groupBox_direction->setTitle(tr("Direction"));
    ui.radioButton_direction_up->setText(tr("Up"));
    ui.radioButton_direction_down->setText(tr("Down"));
    ui.pushButton_find_next->setText(tr("Find next"));
    ui.pushButton_close->setText(tr("Close"));

    ui.pushButton_find_next->setIcon(QIcon(":/images/oxygen/16x16/edit-find.png"));
    ui.pushButton_close->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.radioButton_direction_up->setChecked(true);
}

void DlgFindText::create_signals()
{
    QObject::connect(ui.pushButton_find_next, SIGNAL(clicked()), this, SLOT(button_find_next()));
    QObject::connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
}

void DlgFindText::button_find_next()
{
    QString strText = ui.lineEdit_find->text();
    bool bMatchWholeWordOnly = ui.checkBox_match_whole_word_only->isChecked();
    bool bMatchCase = ui.checkBox_match_case->isChecked();
    bool bDirectionUp = ui.radioButton_direction_up->isChecked();

    QTextDocument::FindFlags findFlags;
    if (bMatchWholeWordOnly) findFlags |= QTextDocument::FindWholeWords;
    if (bMatchCase) findFlags |= QTextDocument::FindCaseSensitively;
    if (bDirectionUp) findFlags |= QTextDocument::FindBackward;

    textEdit->find(strText, findFlags);
}