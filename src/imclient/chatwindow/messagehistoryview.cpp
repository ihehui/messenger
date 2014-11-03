#include "messagehistoryview.h"

#include "messageview.h"


#include <QMessageBox>



namespace HEHUI {

QString MessageHistoryView::htmlForMessagesView = "";

MessageHistoryView::MessageHistoryView(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);


    m_messageView = new MessageView(this);
    m_messageView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    //connect(m_messageView, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)), this, SLOT(requestDownloadImage(const QString &, const QString &)));


    ui.verticalLayoutMain->insertWidget(0, m_messageView);


    if(htmlForMessagesView.isEmpty()){
        QFile file(":/text/resources/text/sample.html");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            htmlForMessagesView = file.readAll();
        }else{
            htmlForMessagesView = "<html xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-16\" /><title>Chat</title></head><body><div></div></body></html>";
        }
    }
    m_messageView->setHtml(htmlForMessagesView);

    QDateTime curTime = QDateTime::currentDateTime();
    ui.dateTimeEditStartTime->setDate(curTime.date());
    ui.dateTimeEditEndTime->setDateTime(curTime);


    rowsPerPage = HISTORY_MESSAGES_PAGE_SIZE;
    m_requestBackword = true;

    m_startTime = "";
    m_endTime = "";

    m_curPageIndex = 0;
    m_searchMode = false;


    QTimer::singleShot(1, ui.toolButtonShowSearchFrame, SLOT(click()));
    //QTimer::singleShot(1, ui.toolButtonSearch, SLOT(click()));
    QTimer::singleShot(1, this, SLOT(requestLastChatMessages()));


}

MessageHistoryView::~MessageHistoryView()
{
    m_messageView->stop();
}

void MessageHistoryView::appendHistoryChatMessages(const QStringList &messages, bool canFetchMore){

    m_messageView->setHtml(htmlForMessagesView);
    m_startTime = "";

    if(m_searchMode){

        if(m_curPageIndex == 0){
            if(m_requestBackword){
                ui.toolButtonNextPage->setEnabled(true);
            }else{
                ui.toolButtonNextPage->setEnabled(canFetchMore);
            }
            ui.toolButtonPreviousPage->setEnabled(false);

        }else if(m_curPageIndex > 0){
            ui.toolButtonPreviousPage->setEnabled(true);
            ui.toolButtonNextPage->setEnabled(canFetchMore);
        }

    }else{
        if(m_curPageIndex == 0){

            if(m_requestBackword){
                ui.toolButtonPreviousPage->setEnabled(canFetchMore);
            }else{
                ui.toolButtonPreviousPage->setEnabled(true);
            }
            //ui.toolButtonPreviousPage->setEnabled(canFetchMore);
            ui.toolButtonNextPage->setEnabled(false);
        }else if(m_curPageIndex < 0){
            ui.toolButtonPreviousPage->setEnabled(canFetchMore);
            ui.toolButtonNextPage->setEnabled(true);
        }

    }

    foreach (QString messageString, messages) {
        QStringList info = messageString.split(QString(PACKET_DATA_SEPARTOR));
        if(info.size() == 4){
            QString userID, displayName, message, datetime;
            userID = info.at(0);
            displayName = info.at(1);
            message = info.at(2);
            datetime = info.at(3);
            m_messageView->appendChatMessage(userID, displayName, message, datetime, false);

            if(m_startTime.isEmpty()){
                m_startTime = datetime;
            }
            m_endTime = datetime;

        }else{
            qWarning()<<"Invalid Chat Message Format!";
        }
    }

}

void MessageHistoryView::on_toolButtonShowSearchFrame_clicked(bool checked){

    ui.frameSearch->setVisible(checked);
    m_searchMode = checked;

    if(!checked){
        requestLastChatMessages();
    }

}

void MessageHistoryView::on_toolButtonSearch_clicked(){

    QDateTime startTime = ui.dateTimeEditStartTime->dateTime();
    QDateTime endTime = ui.dateTimeEditEndTime->dateTime();
    if(startTime > endTime){
        QMessageBox::critical(this, tr("Error"), tr("End time can not be earlier then Start time! "));
        ui.dateTimeEditStartTime->setFocus();
        return;
    }

    resetContent();

    m_requestBackword = false;
    m_searchMode = true;
    emit signalRequestHistoryMessage(startTime.toString("yyyy-MM-dd hh:mm:ss"), endTime.toString("yyyy-MM-dd hh:mm:ss"),  ui.lineEditContent->text(), false);

}

//void MessageHistoryView::on_toolButtonFirstPage_clicked(){

//}

void MessageHistoryView::on_toolButtonPreviousPage_clicked(){

    m_curPageIndex--;

    m_requestBackword = true;

    if(ui.frameSearch->isVisible()){
        emit signalRequestHistoryMessage(ui.dateTimeEditStartTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"), m_startTime,  ui.lineEditContent->text(), true);
    }else{
        emit signalRequestHistoryMessage("1970-01-01 00:00:00", m_startTime,  "", true);
    }


}

//void MessageHistoryView::on_spinBoxPageIndex_valueChanged(int i){

//}

void MessageHistoryView::on_toolButtonNextPage_clicked(){

    m_curPageIndex++;

    m_requestBackword = false;

    if(ui.frameSearch->isVisible()){
        emit signalRequestHistoryMessage(m_endTime, ui.dateTimeEditEndTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"), ui.lineEditContent->text(), false);
    }else{
        emit signalRequestHistoryMessage(m_endTime, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "", false);
    }
}

//void MessageHistoryView::on_toolButtonLastPage_clicked(){

//}

void MessageHistoryView::resetContent(){

    m_messageView->setHtml(htmlForMessagesView);

    m_requestBackword = true;

    m_startTime = "";
    m_endTime = "";

    m_curPageIndex = 0;

    ui.toolButtonPreviousPage->setEnabled(false);
    ui.toolButtonNextPage->setEnabled(false);

}

void MessageHistoryView::requestLastChatMessages(){

    m_curPageIndex = 0;

    m_requestBackword = true;
    m_searchMode = false;

    //emit signalRequestHistoryMessage(ui.dateTimeEditStartTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"), ui.dateTimeEditEndTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"),  ui.lineEditContent->text(), false);
    emit signalRequestHistoryMessage("1970-01-01 00:00:00", ui.dateTimeEditEndTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"),  ui.lineEditContent->text(), true);

}






} //namespace HEHUI
