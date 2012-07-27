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

#include <QClipboard>
#include <QDesktopWidget>
#include <QPushButton>
#include "awaylog_model.h"
#include "core.h"
#include "awaylog.h"

DlgAwaylog::DlgAwaylog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Awaylog"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgAwaylog::createGui()
{
    ui.pushButton_clear->setIcon(QIcon(":/images/oxygen/16x16/document-revert.png"));
    ui.pushButton_copy->setIcon(QIcon(":/images/oxygen/16x16/edit-copy.png"));
    ui.pushButton_close->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_clear->setText(tr("Clear"));
    ui.pushButton_copy->setText(tr("Copy"));
    ui.pushButton_close->setText(tr("Close"));
}

void DlgAwaylog::createSignals()
{
    connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(buttonClear()));
    connect(ui.pushButton_copy, SIGNAL(clicked()), this, SLOT(buttonCopy()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
}

void DlgAwaylog::refresh()
{
    QList<QString> list = Awaylog::instance()->get();
    foreach (const QString &strAwaylog, list)
        ui.listWidget->addItem(strAwaylog);
}

void DlgAwaylog::buttonClear()
{
    Awaylog::instance()->clear();
    ui.listWidget->clear();
}

void DlgAwaylog::buttonCopy()
{
    if (ui.listWidget->selectedItems().size() != 0)
    {
        QString strText = ui.listWidget->selectedItems().at(0)->text();

        QApplication::clipboard()->setText(strText, QClipboard::Selection);
        QApplication::clipboard()->setText(strText, QClipboard::Clipboard);
    }
}

void DlgAwaylog::buttonClose()
{
    this->close();
}
