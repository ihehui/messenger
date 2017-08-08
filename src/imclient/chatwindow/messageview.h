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

#include <QAbstractListModel>

#include "../imuser.h"




namespace HEHUI
{


class ConversationModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)

public:
    ConversationModel(QObject *parent = 0);

    QString recipient() const;
    void setRecipient(const QString &recipient);

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE void appendMessage(const QString &author, const QString &recipient, uint time, const QString &message);

signals:
    void recipientChanged();

private:
    QString m_recipient;

    struct Message{
        Message(QString author, QString recipient, uint timestamp, QString message){
            this->author = author;
            this->recipient = recipient;
            this->timestamp = timestamp;
            this->message = message;
        }

        QString author;
        QString recipient;
        uint timestamp;
        QString message;
    };

    QList<Message> m_messages;


};







class MessageView : public QWidget
{
    Q_OBJECT
public:
    enum ImageDownloadStatus {ImageDownloading, ImageDownloaded, ImageDownloadingFailed};

    explicit MessageView(bool isGroupChat, QWidget *parent = 0);
    virtual ~MessageView();

//    QSize sizeHint();


signals:
    void appendMessage(const QString &userID, const QString &displayName, const QString &headIcon, const QString &message, uint timestamp);

    void signalRequestDownloadImage(const QString &contactID, const QString &imageName);
    void signalTipLastUnACKedMessageFromContact(const QString &tip);

public slots:
    void appendChatMessage(const QString &userID, const QString &time, const QString &message, bool richtext = false);
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
    QQuickItem *messagesListViewer;

//    ConversationModel *m_conversationModel;



};

}//namespace HEHUI

#endif // MESSAGEVIEW_H
