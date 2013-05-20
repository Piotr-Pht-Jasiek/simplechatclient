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

#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include "defines.h"
#include <QtWebKit/QWebView>

class ChatView : public QWebView
{
    Q_OBJECT
public:
    ChatView(const QString &_strChatViewChannel);
    void clearMessages();
    void displayMessage(const QString &strData, MessageCategory eMessageCategory, qint64 iTime, QString strNick = QString::null);
    void refreshCSS();

private:
    QString strChatViewChannel;
    QString strChannel;
    QString strNick;
    QString strWebsite;
    QAction *openChannelsActs[MAX_OPEN_CHANNELS];
    QAction *kickReasonAct[MAX_PUNISH_REASONS];
    QAction *kbanReasonAct[MAX_PUNISH_REASONS];
    bool bAtBottom;

    void createBody();
    void menuNick(QContextMenuEvent *event);
    void menuChannel(QContextMenuEvent *event);
    void menuWebsite(QContextMenuEvent *event);
    void menuStandard(QContextMenuEvent *event);

public slots:
    void search();

private slots:
    void joinChannel();
    void nick();
    void priv();
    void whois();
    void profile();
    void cam();
    void friendsAdd();
    void friendsDel();
    void ignoreAdd();
    void ignoreDel();
    void kick();
    void kickWithReason();
    void kickWithSelectedReason();
    void ban();
    void kbanWithReason();
    void kbanWithSelectedReason();
    void ipban();
    void opAdd();
    void opDel();
    void halfopAdd();
    void halfopDel();
    void moderatorAdd();
    void moderatorDel();
    void voiceAdd();
    void voiceDel();
    void invite();
    void watchVideo();
    void openWebbrowser();
    void sendToNotes();
    void clear();
    void scrollToBottom();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
};

#endif // CHAT_VIEW_H
