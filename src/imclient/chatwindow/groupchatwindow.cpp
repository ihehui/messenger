


#include <QMessageBox>

#include "groupchatwindow.h"
#include "contactsmanager/contactsmanager.h"
#include "imageresource.h"


namespace HEHUI {


GroupChatWindow::GroupChatWindow(InterestGroup *interestGroup, QWidget *parent)
    : QWidget(parent), m_interestGroup(interestGroup)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    m_messageHistoryView = 0;

    Q_ASSERT_X(interestGroup != 0, "GroupChatWindow", "Invalid InterestGroup!");

    ui.chatMessageWindow->setInterestGroup(interestGroup);

    connect(ui.chatMessageWindow, SIGNAL(sendMsgButtonClicked(InterestGroup*, const QString&, const QStringList&)), this, SIGNAL(sendMsgButtonClicked(InterestGroup*, const QString&, const QStringList&)));
    connect(ui.chatMessageWindow, SIGNAL(signalRequestDownloadImage(const QString&, const QString&)), this, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)));
    connect(ui.chatMessageWindow, SIGNAL(signalShowMessageHistory(bool)), this, SLOT(showMessageHistory(bool)));
    connect(ui.chatMessageWindow, SIGNAL(signalCloseWindow()), this, SIGNAL(signalCloseWindow()));


    setWindowTitle(interestGroup->getGroupName() + "(" + QString::number(interestGroup->getGroupID()) + ")");

    ui.textBrowserAnnounce->setHtml(interestGroup->getAnnouncement());

    IMUser *myself = IMUser::instance();
    QString myID = myself->getUserID();

    //Add members
    ContactsManager *contactsManager =  ContactsManager::instance();
    QStringList members = interestGroup->members();
    members.removeAll(myID);
    foreach (const QString &memberID, members) {
        Contact *contact = contactsManager->getUser(memberID);
        QListWidgetItem *item = new QListWidgetItem(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()), contact->getNickName(), ui.listWidgetMembers);
        item->setData(Qt::UserRole, contact->getUserID());
        ui.listWidgetMembers->addItem(item);
    }

    //Add myself
    QListWidgetItem *myItem = new QListWidgetItem(ImageResource::createIconForContact(myself->getFace(), myself->getOnlineState()), myself->getNickName(), ui.listWidgetMembers);
    myItem->setData(Qt::UserRole, myID);
    ui.listWidgetMembers->addItem(myItem);


    connect(ui.listWidgetMembers, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(memberItemActivated(QListWidgetItem*)));

    ui.chatMessageWindow->setEnabled(interestGroup->getState());

    m_preferedSize = QSize();

    QTimer::singleShot(1, this, SLOT(setPreferedSize()));

}

GroupChatWindow::~GroupChatWindow()
{
    emit toBeDstroyed();
}

void GroupChatWindow::closeEvent(QCloseEvent * event){

    if(ui.chatMessageWindow->close()){
        event->accept();
    }else{
        event->ignore();
    }
}

InterestGroup * GroupChatWindow::interestGroup(){
    return m_interestGroup;
}

bool GroupChatWindow::isDownloadingImage(const QString &imageName){
    return ui.chatMessageWindow->isDownloadingImage(imageName);
}

void GroupChatWindow::processImageDownloadResult(const QString &imageName, bool downloaded){
    ui.chatMessageWindow->processImageDownloadResult(imageName, downloaded);
}

void GroupChatWindow::contactOnlineStateChanged(Contact *contact){
    //TODO

    if(!m_interestGroup->hasMember(contact->getUserID())){
        return;
    }

    QListWidgetItem * item = memberItem(contact->getUserID());
    if(!item){return;}

    item->setIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
}

void GroupChatWindow::memberJoinedOrQuitted(const QString &memberID, bool join){

    QListWidgetItem * item = memberItem(memberID);

    if(join){
        if(item){return;}

        Contact *contact = ContactsManager::instance()->getUser(memberID);
        QListWidgetItem *item = new QListWidgetItem(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()), contact->getNickName(), ui.listWidgetMembers);
        item->setData(Qt::UserRole, contact->getUserID());
        ui.listWidgetMembers->addItem(item);
    }else{
        if(!item){return;}

        item = ui.listWidgetMembers->takeItem(ui.listWidgetMembers->row(item));
        delete item;
    }

}

//QSize GroupChatWindow::sizeHint(){
//    return QSize(800, 600);
//}


void GroupChatWindow::appendMessageReceivedFromContact(const QString &message, Contact *contact, const QString &datetime){
    ui.chatMessageWindow->appendChatMessage(message, contact, datetime);
}

void GroupChatWindow::processGrouptHistoryMessage(const QStringList &messages, bool canFetchMore){
    if(m_messageHistoryView){
        m_messageHistoryView->appendHistoryChatMessages(messages, canFetchMore);
    }
}

void GroupChatWindow::memberItemActivated(QListWidgetItem *memberItem){
    //TODO:

    QString memberID = memberItem->data(Qt::UserRole).toString();

    QMessageBox::information(this, "memberID", memberID);
}

void GroupChatWindow::showMessageHistory(bool show){

    ui.chatMessageWindow->resize(0,0);

    if(show){
        setMinimumSize(QSize(0, 0));

        if(!m_messageHistoryView){
            m_messageHistoryView = new MessageHistoryView(this);
            m_messageHistoryView->setMinimumWidth(m_messageHistoryView->width());

            connect(m_messageHistoryView, SIGNAL(signalRequestHistoryMessage(const QString &, const QString &, const QString &, bool)), this, SLOT(requestGroupHistoryMessage(const QString &, const QString &, const QString &, bool)));

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

void GroupChatWindow::requestGroupHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword){
    Q_ASSERT(m_interestGroup);
    emit signalRequestGrouptHistoryMessage(startTime, endTime, content, requestBackword, m_interestGroup->getGroupID());
}

void GroupChatWindow::setPreferedSize(){
    if(!m_preferedSize.isValid()){
        m_preferedSize = size();
        setMinimumSize(m_preferedSize);
        ui.chatMessageWindow->setMinimumWidth(ui.chatMessageWindow->width());

    }
}


QListWidgetItem * GroupChatWindow::memberItem(const QString &memberID){

    int count = ui.listWidgetMembers->count();
    for(int i=0; i<count; i++){
        QListWidgetItem *item = ui.listWidgetMembers->item(i);
        if(!item){continue;}
        if(memberID == item->data(Qt::UserRole).toString()){
            return item;
        }
    }

    return 0;

}




} //namespace HEHUI
