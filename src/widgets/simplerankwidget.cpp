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

#include <QBrush>
#include <QPainter>
#include <QPen>
#include "simplerankwidget.h"

SimpleRankWidget::SimpleRankWidget(QWidget *parent, int param1, int param2, int param3) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    iRank = param1;
    iWidth = param2;
    iHeight = param3;

    setMaximumSize(iWidth, iHeight);
    setMinimumSize(iWidth, iHeight);
}

void SimpleRankWidget::set_rank(int r)
{
    iRank = r;
    update();
}

QSize SimpleRankWidget::sizeHint() const
{
    return QSize(iWidth, iHeight);
}

void SimpleRankWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    // clear
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawRect(0,0,this->sizeHint().width(),this->sizeHint().height());

    if (iRank > 500) iRank = 500;
    if (iRank < -500) iRank = -500;

    if (iRank < 0)
    {
        int x = qAbs(iRank)/4;

        painter.setBrush(Qt::red);
        painter.setPen(Qt::red);
        painter.drawRect(this->sizeHint().width()/2 - x, 0, x, this->sizeHint().height());
    }
    else if (iRank > 0)
    {
        int x = iRank/4;

        painter.setBrush(Qt::green);
        painter.setPen(Qt::green);
        painter.drawRect(this->sizeHint().width()/2, 0, x, this->sizeHint().height());
    }

    // zero
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    painter.drawLine(this->sizeHint().width()/2, 0, this->sizeHint().width()/2, this->sizeHint().height());
}
