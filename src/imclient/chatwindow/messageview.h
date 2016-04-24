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


#include <QWidget>
#include <QQmlError>
#include <QQuickView>


#include "../imuser.h"




namespace HEHUI {

class MessageView : public QWidget
{
    Q_OBJECT

public:
    enum ImageDownloadStatus{ImageDownloading, ImageDownloaded, ImageDownloadingFailed};

    explicit MessageView(bool isGroupChat, QWidget *parent = 0);

//    QSize sizeHint();


signals:
    void appendMessage(const QString &userID, const QString &displayName, const QString &headIcon, const QString &message, uint timestamp);

    void signalRequestDownloadImage(const QString &contactID, const QString &imageName);
    void signalTipLastUnACKedMessageFromContact(const QString &tip);

public slots:
    void appendChatMessage(const QString &userID, const QString &displayName, const QString &headICON, const QString &message, uint timestamp);
    void appendHTML(const QString &htmlTag);
    void setHtml(const QString &html);

    void updateImage(const QString &imageName, ImageDownloadStatus downloadStatus);

private slots:
    void scrollWebFrame(const QSize &contentsSize);
    void linkClicked(const QString &urlString);


    void quickViewStatusChanged(QQuickView::Status);
    void sceneGraphError(QQuickWindow::SceneGraphError error, const QString &message);


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


    QQuickView *m_quickView;
    QObject *messagesListViewer;



};

}//namespace HEHUI

#endif // MESSAGEVIEW_H
