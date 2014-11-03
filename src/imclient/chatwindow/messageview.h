/*
 ****************************************************************************
 * filename
 *
 * Created on: 2013-11-18
 *     Author: 贺辉
 *    License: LGPL
 *    Comment:
 *
 *
 *    =============================  Usage  =============================
 *|
 *|
 *    ===================================================================
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ****************************************************************************
 */

/*
 ***************************************************************************
 * Last Modified on: 2013-11-18
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include <QWebView>
#include <QWebFrame>


#include "../imuser.h"




namespace HEHUI {

class MessageView : public QWebView
{
    Q_OBJECT

public:
    enum ImageDownloadStatus{ImageDownloading, ImageDownloaded, ImageDownloadingFailed};

    explicit MessageView(QWidget *parent = 0);

//    QSize sizeHint();


signals:
    void signalRequestDownloadImage(const QString &contactID, const QString &imageName);
    void signalTipLastUnACKedMessageFromContact(const QString &tip);

public slots:
    void appendChatMessage(const QString &userID, const QString &displayName, const QString &message, const QString &datetime, bool richTextMessage);
    void appendHTML(const QString &htmlTag);

    void updateImage(const QString &imageName, ImageDownloadStatus downloadStatus);

private slots:
    void scrollWebFrame(const QSize &contentsSize);
    void linkClicked(const QUrl & url);


private:
    QString simpleTextToRichTextMessage(const QString &simpleTextMessage);
    QString contactsSimpleTextToPlainTipTextMessage(const QString &simpleTextMessage);
    QString simpleStyleTagToStyleString(const QString &tagsString);


private:

    IMUser *m_myself;
    QString myUserID;

    QString imageCachePath;

    int m_properScrollBarValue;
    QString lastUnACKedMessageFromContact;

    QWebFrame *m_mainWebFrame;


};

}//namespace HEHUI

#endif // MESSAGEVIEW_H
