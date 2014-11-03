#include "contactchatwidget.h"

#include <QMessageBox>


#include "HHSharedCore/hcryptography.h"


namespace HEHUI {


ContactChatWidget::ContactChatWidget(Contact *contact, QWidget *parent)
    : QWidget(parent), m_contact(contact)
{
	ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);


    connect(ui.chatMessageWindow, SIGNAL(sendMsgButtonClicked(Contact*, const QString&, const QStringList&)), this, SIGNAL(sendMsgButtonClicked(Contact*, const QString&, const QStringList&)));
    connect(ui.chatMessageWindow, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)), this, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)));
    connect(ui.chatMessageWindow, SIGNAL(signalShowMessageHistory(bool)), this, SLOT(showMessageHistory(bool)));
    connect(ui.chatMessageWindow, SIGNAL(signalCloseWindow()), this, SIGNAL(signalCloseWindow()));


    m_displayName = tr("Unknown");

    if(contact){
        setContact(contact);
    }

    m_messageHistoryView = 0;


    m_preferedSize = QSize();

    m_fileTransmissionListWidget = 0;

    m_socketConnectedToPeer = INVALID_SOCK_ID;


    QTimer::singleShot(1, this, SLOT(setPreferedSize()));


//    ui.chatMessageWindow->installEventFilter(this);
//    installEventFilter(this);


}

void ContactChatWidget::setContact(Contact *contact){

    Q_ASSERT_X(contact != 0, "ContactChatWidget::setContact", "NULL Contact!");

    this->m_contact = contact;

    ui.chatMessageWindow->setContact(contact);

    m_displayName = contact->displayName();
    QString contactID = contact->getUserID();
    if(contactID != m_displayName){
        m_displayName = m_displayName + "("  + contactID + ")";
    }
    setWindowTitle(m_displayName);

}

ContactChatWidget::~ContactChatWidget()
{
    emit toBeDstroyed();
}

Contact * ContactChatWidget::contact(){
    return m_contact;
}

QString ContactChatWidget::displayName() const{
    return m_displayName;
}

bool ContactChatWidget::isDownloadingImage(const QString &imageName){
    return ui.chatMessageWindow->isDownloadingImage(imageName);
}

void ContactChatWidget::processImageDownloadResult(const QString &imageName, bool downloaded){
    return ui.chatMessageWindow->processImageDownloadResult(imageName, downloaded);
}

void ContactChatWidget::contactOnlineStateChanged(){
    //TODO

}

//QSize ContactChatWidget::sizeHint(){
//    return QSize(640, 480);
//}

void ContactChatWidget::closeEvent(QCloseEvent * event){

    if(ui.chatMessageWindow->close()){
        event->accept();
    }else{
        event->ignore();
    }
}

void ContactChatWidget::dragEnterEvent(QDragEnterEvent *event){
    event->accept();
}

//void ContactChatWidget::dragMoveEvent(QDragMoveEvent *event){
//    qDebug()<<"--------------------------dragMoveEvent";
//    event->accept();
//}

void ContactChatWidget::dropEvent(QDropEvent *event){
    event->ignore();

    if (!event->mimeData()->hasUrls()) {
        return;
    }

    QStringList files;
    foreach (QUrl url, event->mimeData()->urls()) {
        if(url.isLocalFile()){
            files.append(url.toLocalFile());
        }
    }
    if(files.isEmpty()){
        return;
    }


    bool userOffline = (m_contact->getOnlineState() == IM::ONLINESTATE_OFFLINE);
    bool yesToAll = false;

    foreach (QString file, files) {
        QByteArray md5 = Cryptography::getFileMD5(file);
        if(md5.isEmpty()){continue;}

        if(userOffline && (!yesToAll)){
            int ret = QMessageBox::question(this, tr("Contact Offline"),
                                            tr("Contact is offline! Do you want to send offline file to server?"),
                                            QMessageBox::YesToAll | QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                            QMessageBox::Yes
                                            );

            if(ret == QMessageBox::YesToAll){
                yesToAll = true;
            }else if(ret == QMessageBox::No){
                continue;
            }else if(ret == QMessageBox::Cancel){
                return;
            }

        }

        if(!m_fileTransmissionListWidget){
            showFileTransmissionListWidget(true);
        }
        m_fileTransmissionListWidget->slotSendUploadingFileRequest(file, md5, userOffline);
    }


}

void ContactChatWidget::appendMessageReceivedFromContact(const QString &message, Contact *contact, const QString &datetime ){
    ui.chatMessageWindow->appendChatMessage(message, contact, datetime);
}

void ContactChatWidget::processContactHistoryMessage(const QStringList &messages, bool canFetchMore){
    if(m_messageHistoryView){
        m_messageHistoryView->appendHistoryChatMessages(messages, canFetchMore);
    }
}

void ContactChatWidget::showMessageHistory(bool show){

    ui.chatMessageWindow->resize(0,0);

    if(show){
        setMinimumSize(QSize(0, 0));

        if(!m_messageHistoryView){
            m_messageHistoryView = new MessageHistoryView(this);
            m_messageHistoryView->setMinimumWidth(m_messageHistoryView->width());
            connect(m_messageHistoryView, SIGNAL(signalRequestHistoryMessage(const QString &, const QString &, const QString &, bool)), this, SLOT(requestContactHistoryMessage(const QString &, const QString &, const QString &, bool)));

            ui.tabWidget->addTab(m_messageHistoryView, tr("Message History"));
        }
        ui.tabWidget->setCurrentWidget(m_messageHistoryView);

    }else{
        setMinimumSize(m_preferedSize);

        ui.tabWidget->removeTab(ui.tabWidget->indexOf(m_messageHistoryView));
        delete m_messageHistoryView;
        m_messageHistoryView = 0;

        resize(m_preferedSize);
    }

}

void ContactChatWidget::showFileTransmissionListWidget(bool show){

    ui.chatMessageWindow->resize(0,0);

    if(show){
        setMinimumSize(QSize(0, 0));

        if(!m_fileTransmissionListWidget){
            m_fileTransmissionListWidget = new FileTransmissionListWidget(this, this);
            m_fileTransmissionListWidget->setMinimumWidth(m_fileTransmissionListWidget->width());
//            connect(m_fileTransmissionListWidget, SIGNAL(sendFileRequest(const QString &, const QByteArray &)), this, SIGNAL(signalSendUploadintFileRequest(const QString &, const QByteArray &)));
//            connect(m_fileTransmissionListWidget, SIGNAL(cancelSendingFileRequest(const QByteArray &)), this, SIGNAL(signalCancelSendingUploadingFileRequest(const QByteArray &)));
//            connect(m_fileTransmissionListWidget, SIGNAL(abortFileTransmission(const QByteArray &)), this, SIGNAL(signalAbortFileTransmission(const QByteArray &)));
//            connect(m_fileTransmissionListWidget, SIGNAL(acceptFileRequest(const QByteArray &, const QString &)), this, SIGNAL(signalAcceptPeerUploadFileRequest(const QByteArray &, const QString &)));
//            connect(m_fileTransmissionListWidget, SIGNAL(declineFileRequest(const QByteArray &)), this, SIGNAL(signalDeclinePeerUploadFileRequest(const QByteArray &)));
//            connect(m_fileTransmissionListWidget, SIGNAL(signlaCloseWidget()), this, SLOT(closeFileTransmissionListWidget()));

            ui.tabWidget->addTab(m_fileTransmissionListWidget, tr("File Transmitter"));
        }
        ui.tabWidget->setCurrentWidget(m_fileTransmissionListWidget);

    }else{
        setMinimumSize(m_preferedSize);

        ui.tabWidget->removeTab(ui.tabWidget->indexOf(m_fileTransmissionListWidget));
        delete m_fileTransmissionListWidget;
        m_fileTransmissionListWidget = 0;

        resize(m_preferedSize);
    }

}

void ContactChatWidget::slotFileRequestReceivedFromContact(const QString &fileName, qint64 size, const QByteArray &fileMD5){

    if(!m_fileTransmissionListWidget){
        showFileTransmissionListWidget(true);
    }

    m_fileTransmissionListWidget->slotFileRequestReceivedFromContact(fileName, size, fileMD5);

}

void ContactChatWidget::fileUploadRequestResponsed(const QByteArray &fileMD5Sum, bool accepted, const QString &message){
    Q_ASSERT(m_fileTransmissionListWidget);
    if(!m_fileTransmissionListWidget){
        return;
    }

    if(accepted){
        ui.chatMessageWindow->appendSystemMessage("Uploading file request accepted!");
    }else{
        ui.chatMessageWindow->appendSystemMessage("Uploading file request denied!!");
        m_fileTransmissionListWidget->slotCloseProgressInfoWidget(fileMD5Sum);
    }

}


void ContactChatWidget::updateFileTransmissionProgress(const QByteArray &fileMD5, int percent){
    Q_ASSERT(m_fileTransmissionListWidget);
    if(!m_fileTransmissionListWidget){
        return;
    }

    m_fileTransmissionListWidget->updateFileTransmissionProgress(fileMD5, percent);


}

void ContactChatWidget::closeFileTransmissionListWidget(){
    showFileTransmissionListWidget(false);
}

void ContactChatWidget::cancelFileTransmission(const QByteArray &fileMD5Sum){
    m_fileTransmissionListWidget->slotCloseProgressInfoWidget(fileMD5Sum);
}

void ContactChatWidget::requestContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword){
    Q_ASSERT(m_contact);
    emit signalRequestContactHistoryMessage(startTime, endTime, content, requestBackword, m_contact->getUserID());
}

void ContactChatWidget::setPreferedSize(){
    if(!m_preferedSize.isValid()){
        m_preferedSize = size();
        setMinimumSize(m_preferedSize);
        ui.chatMessageWindow->setMinimumWidth(ui.chatMessageWindow->width());
    }

}






} //namespace HEHUI
