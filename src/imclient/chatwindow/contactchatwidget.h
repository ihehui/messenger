#ifndef CONTACTCHATWINDOW_H
#define CONTACTCHATWINDOW_H

#include <QWidget>
#include "ui_contactchatwidget.h"
#include "messagehistoryview.h"

#include "filetransmitter/filetransmissionlistwidget.h"



namespace HEHUI {


class ContactChatWidget : public QWidget
{
    Q_OBJECT

public:
    ContactChatWidget(Contact *contact = 0, QWidget *parent = 0);
    ~ContactChatWidget();

    void setContact(Contact *contact);
    Contact * contact();
    QString displayName() const;

    bool isDownloadingImage(const QString &imageName);
    void processImageDownloadResult(const QString &imageName, bool downloaded);

    void contactOnlineStateChanged();

//    QSize sizeHint();

protected:
    void closeEvent(QCloseEvent * event);

    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);


signals:
    void sendMsgButtonClicked(Contact *contact, const QString &message, const QStringList &imageList);
    void toBeDstroyed();
    void signalRequestDownloadImage(const QString &contactID, const QString &imageName);
    void signalCloseWindow();

    void signalRequestContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, const QString &contactID);

    void signalSendUploadingFileRequest(const QString &filePath, const QByteArray &fileMD5, bool offline = false);
    void signalCancelSendingUploadingFileRequest(const QByteArray &fileMD5);
    void signalAbortFileTransmission(const QByteArray &fileMD5);
    void signalAcceptPeerUploadFileRequest(const QByteArray &fileMD5, const QString &localSavePath);
    void signalDeclinePeerUploadFileRequest(const QByteArray &fileMD5);

public slots:
    void appendMessageReceivedFromContact(const QString &message, Contact *contact, const QString &datetime );

    void processContactHistoryMessage(const QStringList &messages, bool canFetchMore);

    void slotFileRequestReceivedFromContact(const QString &fileName, qint64 size, const QByteArray &fileMD5);
    void fileUploadRequestResponsed(const QByteArray &fileMD5Sum, bool accepted, const QString &message);

    void closeFileTransmissionListWidget();
    void cancelFileTransmission(const QByteArray &fileMD5Sum);

private slots:
    void showMessageHistory(bool show);
    void requestContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword);

    void setPreferedSize();

    void showFileTransmissionListWidget(bool show);

    void updateFileTransmissionProgress(const QByteArray &fileMD5, int percent);


private:
    Ui::ContactChatWidgetClass ui;

    Contact *m_contact;

    QString m_displayName;

    MessageHistoryView *m_messageHistoryView;

    QSize m_preferedSize;

    FileTransmissionListWidget *m_fileTransmissionListWidget;

    int m_socketConnectedToPeer;

};

} //namespace HEHUI

#endif // CONTACTCHATWINDOW_H
