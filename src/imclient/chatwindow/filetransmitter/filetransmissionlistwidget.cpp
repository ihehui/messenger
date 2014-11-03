#include "filetransmissionlistwidget.h"
#include "ui_filetransmissionlistwidget.h"

//#include "../filetransmitter/filetransmissionmanager.h"

#include "../contactchatwidget.h"

//namespace HEHUI {


FileTransmissionListWidget::FileTransmissionListWidget(ContactChatWidget *wgt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransmissionListWidget),
    m_contactChatWidget(wgt)
{
    ui->setupUi(this);

    ui->frame->hide();


}

FileTransmissionListWidget::~FileTransmissionListWidget()
{
    delete ui;
}

void FileTransmissionListWidget::slotFileRequestReceivedFromContact(const QString &fileName, qint64 size, const QByteArray &fileMD5){
    ProgressInfoWidget *wgt = createProgressInfoWidgetItem(fileMD5);
    wgt->requestToReceiveFile(fileName, size, fileMD5);

}

void FileTransmissionListWidget::slotSendUploadingFileRequest(const QString &fileName, const QByteArray &fileMD5, bool offline){
    ProgressInfoWidget *wgt = createProgressInfoWidgetItem(fileMD5);
    wgt->requestToSendFile(fileName, fileMD5);

    if(offline){
        wgt->updateProgress(0);
    }

    emit m_contactChatWidget->signalSendUploadingFileRequest(fileName, fileMD5, offline);
}


//void FileTransmissionListWidget::slotFileRequestCanceledByContact(const QString &fileMD5){
//    slotCloseProgressInfoWidget(fileMD5);
//}

//void FileTransmissionListWidget::slotFileTransmissionAbortedByContact(const QString &fileMD5){
//    slotCloseProgressInfoWidget(fileMD5);
//}

void FileTransmissionListWidget::slotCloseProgressInfoWidget(const QByteArray &fileMD5){

    ProgressInfoWidget *wgt = progressInfoWidgetHash.take(fileMD5);
    if(!wgt){return;}

    int row = findListWidgetItemIndex(wgt);
    if(row < 0){return;}

    wgt->disconnect();
    delete wgt;
    wgt = 0;
    QListWidgetItem * item = ui->listWidget->takeItem(row);
    delete item;

    if(progressInfoWidgetHash.isEmpty()){
        m_contactChatWidget->closeFileTransmissionListWidget();
//        emit signlaCloseWidget();
    }

}

void FileTransmissionListWidget::slotCancelSendingFileRequest(const QByteArray &fileMD5){
//    emit cancelSendingFileRequest(fileMD5);
    emit m_contactChatWidget->signalCancelSendingUploadingFileRequest(fileMD5);
    slotCloseProgressInfoWidget(fileMD5);
}

void FileTransmissionListWidget::slotAbortFileTransmission(const QByteArray &fileMD5){
//    emit abortFileTransmission(fileMD5);
    emit m_contactChatWidget->signalAbortFileTransmission(fileMD5);
    slotCloseProgressInfoWidget(fileMD5);
}

void FileTransmissionListWidget::slotAcceptFileRequest(const QByteArray &fileMD5, const QString &localSavePath){
//    emit acceptFileRequest(fileMD5, localSavePath);
    emit m_contactChatWidget->signalAcceptPeerUploadFileRequest(fileMD5, localSavePath);
}

void FileTransmissionListWidget::slotDeclineFileRequest(const QByteArray &fileMD5){
//    emit declineFileRequest(fileMD5);
    emit m_contactChatWidget->signalDeclinePeerUploadFileRequest(fileMD5);
    slotCloseProgressInfoWidget(fileMD5);
}

void FileTransmissionListWidget::updateFileTransmissionProgress(const QByteArray &fileMD5, int percent){

    if(percent == 100){
        slotCloseProgressInfoWidget(fileMD5);
        return;
    }

    ProgressInfoWidget *wgt = progressInfoWidgetHash.value(fileMD5);
    if(!wgt){return;}
    wgt->updateProgress(percent);

}


int FileTransmissionListWidget::findListWidgetItemIndex(ProgressInfoWidget *wgt){

    if(!wgt){return -1;}

    for(int i=0; i<ui->listWidget->count(); i++){
        QListWidgetItem *item = ui->listWidget->item(i);
        if(!item){continue;}
        if(ui->listWidget->itemWidget(item) == wgt){
            return ui->listWidget->row(item);
        }
    }

    return -1;

}

ProgressInfoWidget * FileTransmissionListWidget::createProgressInfoWidgetItem(const QByteArray &fileMD5){

    if(progressInfoWidgetHash.keys().contains(fileMD5)){
        return progressInfoWidgetHash.value(fileMD5);
    }

    ProgressInfoWidget *wgt = new ProgressInfoWidget(this, this);
//    connect(wgt, SIGNAL(cancelSendingFileRequest(QString)), this, SLOT(slotCancelSendingFileRequest(QString)));
//    connect(wgt, SIGNAL(abortFileTransmission(QString)), this, SLOT(slotAbortFileTransmission(QString)));
//    connect(wgt, SIGNAL(acceptFileRequest(QString, QString)), this, SLOT(slotAcceptFileRequest(QString, QString)));
//    connect(wgt, SIGNAL(declineFileRequest(QString)), this, SLOT(slotDeclineFileRequest(QString)));

    QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(200, 100));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, wgt);
    progressInfoWidgetHash.insert(fileMD5, wgt);


    return wgt;

}


//} //namespace HEHUI
