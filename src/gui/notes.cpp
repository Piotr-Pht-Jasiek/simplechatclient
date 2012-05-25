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

#include <QDesktopWidget>
#include <QPushButton>
#include "notes_model.h"
#include "notes.h"

DlgNotes::DlgNotes(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Notes"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();

    // read notes
    read();

    createSignals();
}

void DlgNotes::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
}

void DlgNotes::createSignals()
{
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(buttonOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgNotes::read()
{
    QString strNotesContent = Notes::instance()->get();
    ui.plainTextEdit->setPlainText(strNotesContent);
}

void DlgNotes::save()
{
    QString strNotesContent = ui.plainTextEdit->toPlainText();
    Notes::instance()->set(strNotesContent);
}

void DlgNotes::buttonOk()
{
    // save notes
    save();

    // close
    this->close();
}

void DlgNotes::resizeEvent(QResizeEvent *)
{
    int iWidth = this->width();
    int iHeight = this->height();
    ui.plainTextEdit->setMinimumSize(QSize(iWidth, iHeight-30));
    ui.plainTextEdit->setMaximumSize(QSize(iWidth, iHeight-30));
    ui.buttonBox->setGeometry(0, iHeight-30, iWidth, 30);
}
