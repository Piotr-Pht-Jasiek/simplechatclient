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

#include "convert.h"
#include "core.h"
#include "nicklist.h"
#include "html_messages_renderer.h"

void fixContextMenu(QString &strData, MessageCategory eMessageCategory)
{
    QStringList strDataList = strData.split(" ");

    // nick
    if ((strDataList[0] == "*") && ((eMessageCategory == MessageJoin) || (eMessageCategory == MessagePart) || (eMessageCategory == MessageQuit)  || (eMessageCategory == MessageKick)))
    {
        QString strWord = strDataList[1]; Convert::removeStyles(strWord);
        strDataList[1] = QString("<a onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\" href=\"#\">%1</a>").arg(strWord);
    }

    for (int i = 0; i < strDataList.size(); i++)
    {
        QString strWord = strDataList[i];

        // channel
        if (strWord.startsWith('#'))
        {
            Convert::removeStyles(strWord);
            strDataList[i] = QString("<a onclick=\"return false\" name=\"channel\" style=\"text-decoration:none;\" href=\"#\" class=\"ChannelColor\">%1</a>").arg(strWord);
        }

        // web
        if ((strWord.startsWith("http")) || (strWord.startsWith("www.")))
        {
            Convert::removeStyles(strWord);
            strDataList[i] = QString("<a onclick=\"return false\" name=\"website\" style=\"color:inherit;text-decoration:none;\" href=\"%1\">%2</a>").arg(strWord, strWord);
        }
        else if (((strWord.contains("http")) && (!strWord.startsWith("http"))) || ((strWord.contains("www.")) && (!strWord.startsWith("www."))))
        {
            if (strWord.contains("http"))
                strWord.replace("http", " http");
            else
                strWord.replace("www.", " www.");

            QStringList strWords = strWord.split(" ");
            if (strWords.size() == 2)
            {
                QString strBeforeLink = strWords[0];
                QString strAfterLink = strWords[1];

                Convert::removeStyles(strAfterLink);
                strAfterLink = QString("<a onclick=\"return false\" name=\"website\" style=\"color:inherit;text-decoration:none;\" href=\"%1\">%2</a>").arg(strAfterLink, strAfterLink);
                strDataList[i] = strBeforeLink+strAfterLink;
            }
        }
    }

    strData = strDataList.join(" ");
}

QString HtmlMessagesRenderer::renderer(QString strData, MessageCategory eMessageCategory, int iTime, QString strNick)
{
    QDateTime dt = QDateTime::fromTime_t(iTime);

    // fix html chars
    Convert::fixHtmlChars(strData);

    // fix for context menu
    fixContextMenu(strData, eMessageCategory);

    // /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
    if (strData.contains(QRegExp(strRegExpMe)))
    {
        strData.replace(QRegExp(strRegExpMe), "\\1");
        eMessageCategory = MessageMe;
    }

    // convert emoticons, font
    Convert::convertText(strData, true);

    // font class
    QString strFontClass;
    if (eMessageCategory == MessageDefault) strFontClass = "DefaultColor";
    else if (eMessageCategory == MessageJoin) strFontClass = "MessageJoin";
    else if (eMessageCategory == MessagePart) strFontClass = "MessagePart";
    else if (eMessageCategory == MessageQuit) strFontClass = "MessageQuit";
    else if (eMessageCategory == MessageKick) strFontClass = "MessageKick";
    else if (eMessageCategory == MessageMode) strFontClass = "MessageMode";
    else if (eMessageCategory == MessageNotice) strFontClass = "MessageNotice";
    else if (eMessageCategory == MessageInfo) strFontClass = "MessageInfo";
    else if (eMessageCategory == MessageMe) strFontClass = "MessageMe";
    else if (eMessageCategory == MessageError) strFontClass = "MessageError";
    else if (eMessageCategory == MessageHighlight) strFontClass = "DefaultColor";
    else if (eMessageCategory == MessageModerNotice) strFontClass = "NoticeColor";

    // themes
    QString strThemes = Core::instance()->settings["themes"];

    // highlight
    QString strTextDecoration;
    if (eMessageCategory == MessageHighlight)
        strTextDecoration = "style=\"text-decoration:underline;\"";

    // me & modernotice
    QString strBeforeNick;
    QString strAfterNick;
    if ((eMessageCategory == MessageMe) || (eMessageCategory == MessageModerNotice))
        strBeforeNick = "* ";
    else
    {
        if ((strThemes == "Adara") || (strThemes == "Alhena"))
            strAfterNick = ":";
        else
        {
            strBeforeNick = "&lt;";
            strAfterNick = "&gt;";
        }
    }

    if (strThemes == "Adara")
    {
        if (!strNick.isEmpty())
        {
            QString strUserAvatarPath = Nicklist::instance()->getUserAvatarPath(strNick);

            // is valid avatar
            if ((strUserAvatarPath != Nicklist::instance()->getEmptyUserAvatarPath()) && (QImage(strUserAvatarPath).isNull()))
                strUserAvatarPath = Nicklist::instance()->getEmptyUserAvatarPath();

#ifndef Q_WS_WIN
            strUserAvatarPath = "file://"+strUserAvatarPath;
#endif
            QString strUserAvatarImg = QString("<img src=\"%1\" alt=\"avatar\" class=\"avatar\" />").arg(strUserAvatarPath);
            return QString("<table><tr><td class=\"TableText\">%1<span class=\"DefaultColor\">%2<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">%3</a>%4 </span><span class=\"%5\" %6>%7</span></td><td class=\"time\">%8</td></tr></table>").arg(strUserAvatarImg, strBeforeNick, strNick, strAfterNick, strFontClass, strTextDecoration, strData, dt.toString("hh:mm"));
        }
        else
        {
            return QString("<table><tr><td class=\"TableText\">&nbsp;<span class=\"%1\">%2</span></td><td class=\"time\">%3</td></tr></table>").arg(strFontClass, strData, dt.toString("hh:mm"));
        }
    }
    else if (strThemes == "Alhena")
    {
        if (!strNick.isEmpty())
            return QString("<table><tr><td class=\"TableText\"><span class=\"DefaultColor\">%1<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">%2</a>%3 </span><span class=\"%4\" %5>%6</span></td><td class=\"time\">%7</td></tr></table>").arg(strBeforeNick, strNick, strAfterNick, strFontClass, strTextDecoration, strData, dt.toString("hh:mm:ss"));
        else
            return QString("<table><tr><td class=\"TableText\">&nbsp;<span class=\"%1\">%2</span></td><td class=\"time\">%3</td></tr></table>").arg(strFontClass, strData, dt.toString("hh:mm:ss"));
    }
    else
    {
        if (!strNick.isEmpty())
            return QString("<span class=\"DefaultColor\">%1 %2<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">%3</a>%4 <span class=\"%5\" %6>%7</span></span>").arg(dt.toString("[hh:mm:ss]"), strBeforeNick, strNick, strAfterNick, strFontClass, strTextDecoration, strData);
        else
            return QString("<span class=\"DefaultColor\">%1 <span class=\"%2\">%3</span></span>").arg(dt.toString("[hh:mm:ss]"), strFontClass, strData);
    }
}

QString HtmlMessagesRenderer::rendererDebug(QString strData, int iTime)
{
    QString strColor = "#000000";
    if (strData.startsWith("<- "))
    {
        strColor = "#ee0000";
        strData.remove(0,3);
    }
    else if (strData.startsWith("-> "))
    {
        strColor = "#0000ee";
        strData.remove(0,3);
    }

    // fix html chars
    Convert::fixHtmlChars(strData);

    // time
    QString strTime = QDateTime::fromTime_t(iTime).toString("[hh:mm:ss]");

    // display
    return QString("%1 <span style=\"color:%2;\">%3</span>").arg(strTime, strColor, strData);
}

QString HtmlMessagesRenderer::headCSS()
{
    QString strDefaultColor = Core::instance()->settings.value("default_color");
    QString strChannelColor = Core::instance()->settings.value("channel_color");
    QString strFontSize = Core::instance()->settings.value("font_size");

    QString strMessageJoin = Core::instance()->settings.value("message_join_color");
    QString strMessagePart = Core::instance()->settings.value("message_part_color");
    QString strMessageQuit = Core::instance()->settings.value("message_quit_color");
    QString strMessageKick = Core::instance()->settings.value("message_kick_color");
    QString strMessageMode = Core::instance()->settings.value("message_mode_color");
    QString strMessageNotice = Core::instance()->settings.value("message_notice_color");
    QString strMessageInfo = Core::instance()->settings.value("message_info_color");
    QString strMessageMe = Core::instance()->settings.value("message_me_color");
    QString strMessageError = Core::instance()->settings.value("message_error_color");

    QString strHeadCSS = "div{margin-bottom: 2px;}";
    strHeadCSS.append(QString("table{border-spacing: 0; margin: 0; padding: 0; font-family: sans; word-wrap: break-word; font-size:%1;}.TableText{width:100%;}").arg(strFontSize));
    strHeadCSS.append(QString(".time{font-weight:normal; text-decoration:none; color:%1; padding-right:5px;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".avatar{vertical-align:middle; margin-left:4px; margin-right:4px; width:30px; height:30px;}"));

    strHeadCSS.append(QString(".DefaultColor{color:%1;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".ChannelColor{color:%1;}").arg(strChannelColor));

    strHeadCSS.append(QString(".MessageDefault{color:%1;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".MessageJoin{color:%1;}").arg(strMessageJoin));
    strHeadCSS.append(QString(".MessagePart{color:%1;}").arg(strMessagePart));
    strHeadCSS.append(QString(".MessageQuit{color:%1;}").arg(strMessageQuit));
    strHeadCSS.append(QString(".MessageKick{color:%1;}").arg(strMessageKick));
    strHeadCSS.append(QString(".MessageMode{color:%1;}").arg(strMessageMode));
    strHeadCSS.append(QString(".MessageNotice{color:%1;}").arg(strMessageNotice));
    strHeadCSS.append(QString(".MessageInfo{color:%1;}").arg(strMessageInfo));
    strHeadCSS.append(QString(".MessageMe{color:%1;}").arg(strMessageMe));
    strHeadCSS.append(QString(".MessageError{color:%1;}").arg(strMessageError));
    strHeadCSS.append(QString(".MessageHighlight{color:%1;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".MessageModerNotice{color:%1;}").arg(strDefaultColor));

    return strHeadCSS;
}

QString HtmlMessagesRenderer::bodyCSS()
{
    QString strFontSize = Core::instance()->settings.value("font_size");
    QString strBackgroundImage = Core::instance()->settings.value("background_image");
    QString strDisableBackgroundImage = Core::instance()->settings.value("disable_background_image");

#ifndef Q_WS_WIN
    strBackgroundImage = "file://"+strBackgroundImage;
#endif

    QString strBackground;
    if ((strDisableBackgroundImage == "false") && (!strBackgroundImage.isEmpty()))
        strBackground = QString("background-image: url('%1'); background-attachment: fixed; background-position: center; background-repeat: no-repeat;").arg(strBackgroundImage);

    return QString("margin: 0; padding: 0; font-family: sans; word-wrap: break-word; font-size:%1; %2").arg(strFontSize, strBackground);
}
