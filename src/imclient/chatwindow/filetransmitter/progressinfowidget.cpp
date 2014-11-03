#include "progressinfowidget.h"
#include "ui_progressinfowidget.h"

#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>

#include <QDebug>

#include "filetransmissionlistwidget.h"


//namespace HEHUI {

ProgressInfoWidget::ProgressInfoWidget(FileTransmissionListWidget *wgt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressInfoWidget),
    listWidget(wgt)
{
    ui->setupUi(this);

    m_sendingMode = true;
    //m_transmitting = false;

    Q_ASSERT(wgt);

}

ProgressInfoWidget::~ProgressInfoWidget()
{
    delete ui;
}

void ProgressInfoWidget::requestToSendFile(const QString &filePath, const QByteArray &fileMD5){
    m_sendingMode = true;
    m_filePath = filePath;
    m_fileMD5 = fileMD5;

    QFileInfo info(filePath);
    qint64 size = info.size();

    ui->stackedWidget->setCurrentWidget(ui->pageWaitingForSending);
    ui->labelStatus->setText(tr("Send File Request: %1").arg(getFileSizeString(size)));
    ui->labelFileName->setText(info.fileName());
    ui->labelFileName->setToolTip(filePath);

    qDebug()<<"---------------MD5:"<<fileMD5.toHex();

}

void ProgressInfoWidget::requestToReceiveFile(const QString &fileName, qint64 size, const QByteArray &fileMD5){
    m_sendingMode = false;
    m_filePath = fileName;
    m_fileMD5 = fileMD5;

    ui->stackedWidget->setCurrentWidget(ui->pageWaitingForReceiving);
    ui->labelStatus->setText(tr("Accept File Request: %1").arg(getFileSizeString(size)));
    ui->labelFileName->setText(fileName);
}

void ProgressInfoWidget::updateProgress(int percent){
    ui->progressBar->setValue(percent);
}

void ProgressInfoWidget::on_toolButtonFile_clicked(){
    if(!m_sendingMode){
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(m_filePath));
}

void ProgressInfoWidget::on_pushButtonSendOfflineFile_clicked(){
    listWidget->slotSendUploadingFileRequest(m_filePath, m_fileMD5, true);
}

void ProgressInfoWidget::on_pushButtonCancel_clicked(){
//    emit cancelSendingFileRequest(m_fileMD5);
    listWidget->slotCancelSendingFileRequest(m_fileMD5);
}

void ProgressInfoWidget::on_toolButtonAbort_clicked(){
//    emit abortFileTransmission(m_fileMD5);
    listWidget->slotAbortFileTransmission(m_fileMD5);
}

void ProgressInfoWidget::on_pushButtonAccept_clicked(){
//    emit acceptFileRequest(m_fileMD5, "");
    listWidget->slotAcceptFileRequest(m_fileMD5, "");
}

void ProgressInfoWidget::on_pushButtonSaveAs_clicked(){

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"),
                               QDir::homePath() + QDir::separator() + m_filePath
                               );
    if(!fileName.isEmpty()){
//        emit acceptFileRequest(m_fileMD5, fileName);
        listWidget->slotAcceptFileRequest(m_fileMD5, fileName);
    }

}

void ProgressInfoWidget::on_pushButtonDecline_clicked(){
//    emit declineFileRequest(m_fileMD5);
    listWidget->slotDeclineFileRequest(m_fileMD5);
}

QString ProgressInfoWidget::getFileSizeString(qint64 fileSize) const{
    QString sizeString = QString::number(fileSize) + " Byte";
    if(fileSize >= 1024 && fileSize < 1024*1024){
        sizeString = QString::number(fileSize/1024) + " KB";
    }else if(fileSize >= 1024*1024 && fileSize < 1024*1024*1024){
        sizeString = QString::number(fileSize/(1024*1024)) + " MB";
    }else if(fileSize >= 1024*1024*1024){
        sizeString = QString::number(fileSize/(1024*1024*1024)) + " GB";
    }

    return sizeString;
}





//} //namespace HEHUI
