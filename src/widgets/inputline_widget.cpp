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

#include <QEvent>
#include <QKeyEvent>
#include <QTextEdit>
#include "channel.h"
#include "commands.h"
#include "core.h"
#include "emoticons.h"
#include "mainwindow.h"
#include "nick.h"
#include "inputline_widget.h"

InputLineWidget::InputLineWidget(QWidget *parent) : QTextEdit(parent), index(0), strLastWord(QString::null), iLastMessage(-1), iLastMessageLimit(30)
{
}

InputLineWidget::~InputLineWidget()
{
    strLastWord = QString::null;
}

void InputLineWidget::insertText(const QString &strText)
{
    this->insertPlainText(strText);
}

QString InputLineWidget::getWord()
{
    QString strWord = this->toPlainText();
    if (!strWord.isEmpty())
    {
        int iLength = 0;
        int iStart = 0;

        // if first word -1, if next -2
        if (this->textCursor().position()-1 == 0)
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-1)+1;
        else
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-2)+1;

        iLength = this->textCursor().position()-iStart;
        strWord = strWord.mid(iStart, iLength);

        strWord.remove(" ");
    }
    return strWord;
}

void InputLineWidget::setWord(const QString &strSetWord)
{
    QString strWord = this->toPlainText();
    if (!strWord.isEmpty())
    {
        int iLength = 0;
        int iStart = 0;

        // if first word -1, if next -2
        if (this->textCursor().position()-1 == 0)
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-1)+1;
        else
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-2)+1;

        iLength = this->textCursor().position()-iStart;
        strWord = strWord.mid(iStart, iLength);

        QString strFixedSetWord = strSetWord+" ";

        QString strNewLine = this->toPlainText();
        strNewLine = strNewLine.replace(iStart, iLength, strFixedSetWord);
        this->setPlainText(strNewLine);

        // set cursor
        QTextCursor cursor = this->textCursor();
        cursor.setPosition(iStart+strFixedSetWord.length());
        this->setTextCursor(cursor);
    }
}

bool InputLineWidget::event(QEvent *e)
{
    if (e->type() != QEvent::KeyPress)
        return QTextEdit::event(e);

    QKeyEvent *k = static_cast<QKeyEvent*>(e);

    // key event to main window
    bool bAlreadyUsed = Core::instance()->mainWindow()->inputLineKeyEvent(k);
    if (bAlreadyUsed)
        return true;

    // key event
    if ((k->key() == Qt::Key_Tab) && (k->modifiers() == Qt::NoModifier))
    {
        iLastMessage = -1;

        QString strChannel = Channel::instance()->getCurrentName();
        QString strWord = getWord();

        if (strWord.isEmpty())
            return true;

        if ((strWord.at(0) == '/') && (strWord.at(1) != '/'))
        {
            Commands *pCommands = new Commands();
            QList<QString> lCommands = pCommands->getList();
            delete pCommands;

            if (lCommands.size() == 0)
                return true;

            if (strLastWord.isEmpty())
            {
                find.clear();
                foreach (const QString &strCommand, lCommands)
                {
                    if (strCommand.startsWith(strWord, Qt::CaseInsensitive))
                        find.append(strCommand);
                }

                strLastWord = strWord;
            }
        }
        else if ((strWord.at(0) == '/') && (strWord.at(1) == '/'))
        {
            QList<CaseIgnoreString> lEmoticons = Emoticons::instance()->listEmoticons();

            if (lEmoticons.size() == 0)
                return true;

            if (strLastWord.isEmpty())
            {
                find.clear();
                foreach (const QString &strEmoticon, lEmoticons)
                {
                    if (strEmoticon.startsWith(strWord, Qt::CaseInsensitive))
                        find.append(strEmoticon);
                }

                strLastWord = strWord;
            }
        }
        else
        {
            QList<CaseIgnoreString> usersList = Nick::instance()->getFromChannel(strChannel);

            if (usersList.size() == 0)
                return true;

            if (strLastWord.isEmpty())
            {
                find.clear();
                foreach (const QString &strUser, usersList)
                {
                    if (strUser.startsWith(strWord, Qt::CaseInsensitive))
                        find.append(strUser);
                    if (strUser.startsWith("~"+strWord, Qt::CaseInsensitive))
                        find.append(strUser);
                }

                strLastWord = strWord;
            }
        }

        if (!find.isEmpty())
        {
            if (index > find.size())
                index = 0;

            if (!find.isEmpty())
            {
                QString strSetWord = find.value(index, QString::null);
                setWord(strSetWord);
            }

            index++;
            if (index >= find.size())
                index = 0;
        }

        return true;
    }
    else if (((k->key() == Qt::Key_Enter) || (k->key() == Qt::Key_Return)) && (k->modifiers() == Qt::NoModifier))
    {
        QString strText = this->toPlainText();

        if (!strText.isEmpty())
        {
            if (!lLastMessages.contains(strText))
            {
                if (lLastMessages.size() >= iLastMessageLimit)
                    lLastMessages.removeLast();

                lLastMessages.push_front(strText);
            }

            iLastMessage = -1;

            emit returnPressed();
        }

        return true;
    }
    else if ((k->key() == Qt::Key_Up) && (k->modifiers() == Qt::ControlModifier))
    {
        QString strText = this->toPlainText();

        if ((!strText.isEmpty()) && (!lLastMessages.contains(strText)))
        {
            if (lLastMessages.size() >= iLastMessageLimit)
                lLastMessages.removeLast();

            lLastMessages.push_front(strText);

            iLastMessage++;
        }

        iLastMessage++;
        if (iLastMessage > iLastMessageLimit)
            iLastMessage = iLastMessageLimit;

        if (iLastMessage > lLastMessages.size()-1)
            iLastMessage = lLastMessages.size()-1;

        QString strLastMessage = lLastMessages.value(iLastMessage, QString::null);

        // text
        this->setPlainText(strLastMessage);
        // cursor
        this->moveCursor(QTextCursor::End);

        return true;
    }
    else if ((k->key() == Qt::Key_Down) && (k->modifiers() == Qt::ControlModifier))
    {
        QString strText = this->toPlainText();

        if ((!strText.isEmpty()) && (!lLastMessages.contains(strText)))
        {
            if (lLastMessages.size() >= iLastMessageLimit)
                lLastMessages.removeLast();

            lLastMessages.push_front(strText);

            iLastMessage--;
        }

        iLastMessage--;
        if (iLastMessage < -1)
            iLastMessage = -1;

        QString strLastMessage = lLastMessages.value(iLastMessage, QString::null);

        // text
        this->setPlainText(strLastMessage);

        // cursor
        this->moveCursor(QTextCursor::End);

        return true;
    }
    else
    {
        iLastMessage = -1;
        index = 0;
        strLastWord = QString::null;
        return QTextEdit::event(e);
    }
}
