#ifndef FILETRANSMISSIONLISTWIDGET_H
#define FILETRANSMISSIONLISTWIDGET_H

#include <QWidget>

#include "progressinfowidget.h"




//namespace HEHUI {


namespace Ui {
class FileTransmissionListWidget;
}

namespace HEHUI{
class ContactChatWidget;
}

class FileTransmissionListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTransmissionListWidget(HEHUI::ContactChatWidget *wgt, QWidget *parent = 0);
    ~FileTransmissionListWidget();


signals:
//    void sendFileRequest(const QString &filePath, const QByteArray &fileMD5);
//    void cancelSendingFileRequest(const QByteArray &fileMD5);
//    void abortFileTransmission(const QByteArray &fileMD5);

//    void acceptFileRequest(const QByteArray &fileMD5, const QString &localSavePath);
//    void declineFileRequest(const QByteArray &fileMD5);

//    void signlaCloseWidget();

public slots:
    void slotFileRequestReceivedFromContact(const QString &fileName, qint64 size, const QByteArray &fileMD5);
    void slotSendUploadingFileRequest(const QString &fileName, const QByteArray &fileMD5, bool offline);

    //    void slotFileRequestCanceledByContact(const QString &fileMD5);
//    void slotFileTransmissionAbortedByContact(const QString &fileMD5);
    void slotCloseProgressInfoWidget(const QByteArray &fileMD5);

    void slotCancelSendingFileRequest(const QByteArray &fileMD5);
    void slotAbortFileTransmission(const QByteArray &fileMD5);
    void slotAcceptFileRequest(const QByteArray &fileMD5, const QString &localSavePath);
    void slotDeclineFileRequest(const QByteArray &fileMD5);

    void updateFileTransmissionProgress(const QByteArray &fileMD5, int percent);



private:
    int findListWidgetItemIndex(ProgressInfoWidget *wgt);
    ProgressInfoWidget * createProgressInfoWidgetItem(const QByteArray &fileMD5);

private:
    Ui::FileTransmissionListWidget *ui;

    //QHash<QString /*File MD5*/, QString /*File Name*/> fileInfoHash;
    QHash<QString /*File MD5*/, ProgressInfoWidget*> progressInfoWidgetHash;

    HEHUI::ContactChatWidget *m_contactChatWidget;

};

//} //namespace HEHUI

#endif // FILETRANSMISSIONLISTWIDGET_H
