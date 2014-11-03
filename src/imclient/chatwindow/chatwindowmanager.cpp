#include <QMdiSubWindow>
#include <QDebug>
#include <QMessageBox>

#include "chatwindowmanager.h"
#include "../imageresource.h"


#include "../contactsmanager/contactsmanager.h"

#include "HHSharedCore/hutilities.h"


namespace HEHUI {

ChatWindowManager::ChatWindowManager(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

    connect(ui.mdiArea, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));

    initTabWidget();


    m_chatWindowDisplayStyle = SeparatedChatWindow;
    ui.stackedWidget->setCurrentWidget(ui.pageTabWidget);

//    m_preferedSize = size();

    m_fileTransmissionManager = 0;
    m_fileTransmissionPacketsParser = 0;
    m_socketConnectedToServer = INVALID_SOCK_ID;


}

ChatWindowManager::~ChatWindowManager()
{

}

void ChatWindowManager::closeEvent(QCloseEvent *e) {

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        int tabPages = ui.tabWidget->count();
        for(int i = tabPages-1; i >= 0; i--){
            ui.tabWidget->setCurrentIndex(i);
            slotcloseTab();
        }
    }
        break;
    case MDIChatWindow:
    {
        foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
            window->close();
//            ContactChatWidget *messageWindow = qobject_cast<ContactChatWidget *>(window->widget());
//            if(messageWindow && messageWindow->close()){
//                window->close();
//                continue;
//            }

//            GroupChatWindow *groupChatWindow = qobject_cast<GroupChatWindow *>(window->widget());
//            if(!groupChatWindow){continue;}
//            if(groupChatWindow && groupChatWindow->close()){
//                window->close();
//            }
        }
    }
    default:
        break;
    }

    e->ignore();

//    resize(m_preferedSize);
//    updateGeometry();


/*
    if (Settings::instance()->getHideOnClose()) {
    	showMinimized();
        hide();
        e->ignore();
    }else{

    ui.mdiArea->closeAllSubWindows();
 	//Clear the pairList
    	pairList.clear();

	e->accept();
    }
*/


}

void ChatWindowManager::setChatWindowDisplayStyle(ChatWindowManager::ChatWindowDisplayStyle style){
    m_chatWindowDisplayStyle = style;
}

ChatWindowManager::ChatWindowDisplayStyle ChatWindowManager::getChatWindowDisplayStyle(){
    return m_chatWindowDisplayStyle;
}

void ChatWindowManager::switchChatWindowDisplayStyle(ChatWindowManager::ChatWindowDisplayStyle style){

    if(style == m_chatWindowDisplayStyle){return;}


    //TODO
    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        while (ui.tabWidget->count()) {
            QWidget *wgt = ui.tabWidget->currentWidget();
            ui.tabWidget->removeTab(ui.tabWidget->indexOf(wgt));
            wgt->setParent(0);
        }

        showMinimized();
        hide();
    }
        break;
    case MDIChatWindow:
    {
        foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
            ui.mdiArea->removeSubWindow(window);
            window->widget()->setParent(0);
            window->deleteLater();
        }

        showMinimized();
        hide();
    }
        break;
    case SeparatedChatWindow:
    {
        foreach (ContactChatWidget *ccw, m_contactChatWidgetHash.values()) {
            ccw->hide();
        }
        foreach (GroupChatWindow *gcw, m_groupChatWidgetHash.values()) {
            gcw->hide();
        }

    }
        break;
    default:
        break;
    }


    switch (style) {
    case TabbedChatWindow:
    {
        foreach (ContactChatWidget *ccw, m_contactChatWidgetHash.values()) {
            ui.tabWidget->addTab(ccw, ccw->displayName());
            ui.tabWidget->setCurrentWidget(ccw);
            ccw->show();
        }
        foreach (GroupChatWindow *gcw, m_groupChatWidgetHash.values()) {
            ui.tabWidget->addTab(gcw, gcw->interestGroup()->getGroupName());
            ui.tabWidget->setCurrentWidget(gcw);
            gcw->show();
        }

        ui.stackedWidget->setCurrentWidget(ui.pageTabWidget);
        showNormal();
        activateWindow();
        raise();
    }
        break;
    case MDIChatWindow:
    {
        foreach (ContactChatWidget *ccw, m_contactChatWidgetHash.values()) {
            QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(ccw);
            Contact *contact = ccw->contact();
            subWindow->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
            //connect(subWindow, SIGNAL(destroyed()), this, SLOT(handleChatWindowClosed()));
            ccw->show();
            subWindow->show();
        }
        foreach (GroupChatWindow *gcw, m_groupChatWidgetHash.values()) {
            QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(gcw);
            subWindow->setWindowIcon(ImageResource::createIconForInterestGroup());
            //connect(subWindow, SIGNAL(destroyed()), this, SLOT(handleChatWindowClosed()));
            gcw->show();
        }

        ui.stackedWidget->setCurrentWidget(ui.pageMdiArea);
        showNormal();
        activateWindow();
        raise();
    }
        break;
    case SeparatedChatWindow:
    {
        foreach (ContactChatWidget *ccw, m_contactChatWidgetHash.values()) {
            ccw->showNormal();
        }
        foreach (GroupChatWindow *gcw, m_groupChatWidgetHash.values()) {
            gcw->showNormal();
        }

        showMinimized();
        hide();
    }
        break;
    default:
        break;
    }

    m_chatWindowDisplayStyle = style;


}

bool ChatWindowManager::isContactChatWindowOpen(const QString &contactID){
    ContactChatWidget *ccw = m_contactChatWidgetHash.value(contactID);
    return ccw;
}

bool ChatWindowManager::isInterestGroupChatWindowOpen(int groupID){
    GroupChatWindow *gcw = m_groupChatWidgetHash.value(groupID);
    return gcw;
}

bool ChatWindowManager::closeContactChatWindow(Contact *contact){

    Q_ASSERT(contact);

    ContactChatWidget *ccw = m_contactChatWidgetHash.value(contact->getUserID());
    if(!ccw){return true;}

    return ccw->close();

}

bool ChatWindowManager::closeInterestGroupChatWindow(InterestGroup *group){

    Q_ASSERT(group);

    GroupChatWindow *gcw = m_groupChatWidgetHash.value(group->getGroupID());
    if(!gcw){return true;}

    return gcw->close();

}

void ChatWindowManager::interestGroupMemberJoinedOrQuitted(quint32 groupID, const QString &memberID, bool join){

    GroupChatWindow *gcw = m_groupChatWidgetHash.value(groupID);
    if(!gcw){return;}

    gcw->memberJoinedOrQuitted(memberID, join);

}

void ChatWindowManager::contactOnlineStateChanged(Contact *contact){

    if(!contact){return;}

    foreach (GroupChatWindow *gcw, m_groupChatWidgetHash.values()) {
        gcw->contactOnlineStateChanged(contact);
    }

    ContactChatWidget *ccw = m_contactChatWidgetHash.value(contact->getUserID());
    if(!ccw){return;}
    ccw->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
    ccw->contactOnlineStateChanged();

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        ui.tabWidget->setTabIcon(ui.tabWidget->indexOf(ccw), ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
    }
        break;
    case MDIChatWindow:
    {
        QMdiSubWindow * subWindow = findChatWithContactMdiSubWindow(contact);
        if(!subWindow){break;}
        subWindow->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
    }
        break;
    case SeparatedChatWindow:
    {
//        ccw = m_contactChatWidgetHash.value(contactID);
//        if(!ccw){break;}
//        ccw->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
    }
        return;
    default:
        break;
    }

}

void ChatWindowManager::processImageDownloadResult(const QString &contactID, const QString &imageName, bool downloaded){

//    ContactChatWidget *ccw = m_contactChatWidgetHash.value(contactID);
//    if(ccw){
//        if(ccw->isDownloadingImage(imageName)){
//            ccw->processImageDownloadResult(imageName, downloaded);
//        }
//    }

    foreach (ContactChatWidget *ccw, m_contactChatWidgetHash.values()) {
        if(ccw->isDownloadingImage(imageName)){
            ccw->processImageDownloadResult(imageName, downloaded);
        }
    }


    foreach (GroupChatWindow *gcw, m_groupChatWidgetHash.values()) {
        if(gcw->isDownloadingImage(imageName)){
            gcw->processImageDownloadResult(imageName, downloaded);
        }
    }

}

void ChatWindowManager::processContactHistoryMessage(const QStringList &messages, bool canFetchMore, const QString &contactID){

    Contact *contact = ContactsManager::instance()->getUser(contactID);
    if(!contact){
        qCritical()<<"Error:No such contact:"<<contactID;
        return;
    }

    ContactChatWidget *contactChatWindow = m_contactChatWidgetHash.value(contactID);
    if(!contactChatWindow){return;}

    contactChatWindow->processContactHistoryMessage(messages, canFetchMore);
}

void ChatWindowManager::processGrouptHistoryMessage(const QStringList &messages, bool canFetchMore, quint32 groupID){

    InterestGroup *group = ContactsManager::instance()->getInterestGroup(groupID);
    if(!group){
        qCritical()<<"Error:No such interest group:"<<groupID;
        return;
    }


    GroupChatWindow *groupChatWindow = m_groupChatWidgetHash.value(groupID);
    if(!groupChatWindow){return;}

    groupChatWindow->processGrouptHistoryMessage(messages, canFetchMore);

}

void ChatWindowManager::slotNewChatWithContact(const QString &contactID){
    qDebug()<<"----ChatWindowManager::slotNewChatWithContact(const QString &id)~~~ contactID:"<<contactID;

    Contact *contact = ContactsManager::instance()->getUser(contactID);
    if(!contact){
        qCritical()<<"ERROR! No such contact:"<<contactID;
        return;
    }

    ContactChatWidget *ccw = 0;

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        ccw = findContactChatTabWidget(contact);
        if(!ccw){
            ccw = createContactChatWindow(contact);
            ui.tabWidget->addTab(ccw, ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()), ccw->displayName());
        }
        ui.tabWidget->setCurrentWidget(ccw);
    }
        break;
    case MDIChatWindow:
    {
        QMdiSubWindow * subWindow = findChatWithContactMdiSubWindow(contact);
        if(!subWindow){
            ccw = createContactChatWindow(contact);
            QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(ccw);
            subWindow->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
            //connect(subWindow, SIGNAL(destroyed()), this, SLOT(handleChatWindowClosed()));
            ccw->show();
        }else{
            ui.mdiArea->setActiveSubWindow(subWindow);
        }
    }
        break;
    case SeparatedChatWindow:
    {
        ccw = m_contactChatWidgetHash.value(contactID);
        if(!ccw){
            ccw = createContactChatWindow(contact);
        }
        ccw->show();
        ccw->activateWindow();
        QApplication::alert(ccw);
    }
        break;
    default:
        break;
    }


    QMap<QString/*Time String*/, QString/*Message*/> unreadMessages = contact->takeUnreadMessages();
    foreach (QString time, unreadMessages.keys()) {
        ccw->appendMessageReceivedFromContact(unreadMessages.value(time), 0, time);
    }


    if(m_chatWindowDisplayStyle != SeparatedChatWindow){
        showNormal();
    }

    activateWindow();
    raise();


}

void ChatWindowManager::slotNewMessageReceivedFromContact(const QString &contactID, const QString &message, const QString &time){
    //TODO:
    qWarning()<< "----ChatWindowManager::slotNewMessageReceivedFromContact(....)~~From:"<<contactID<<" msg:"<<message<<" Time:"<<time;


    Contact *contact = ContactsManager::instance()->getUser(contactID);
    if(!contact){
        qCritical()<<"Error:No such contact:"<<contactID;
        return;
    }

    ContactChatWidget *contactChatWindow = 0;

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        contactChatWindow = findContactChatTabWidget(contact);
        if(!contactChatWindow){
            contactChatWindow = createContactChatWindow(contact);
            ui.tabWidget->addTab(contactChatWindow, contactChatWindow->displayName());
        }
        ui.tabWidget->setCurrentWidget(contactChatWindow);

        update();
        showNormal();
        raise();
        QApplication::alert(this);
    }
        break;
    case MDIChatWindow:
    {
        QMdiSubWindow * subWindow = findChatWithContactMdiSubWindow(contact);
        if(!subWindow){
            contactChatWindow = createContactChatWindow(contact);
            QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(contactChatWindow);
            subWindow->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
            //connect(subWindow, SIGNAL(destroyed()), this, SLOT(handleChatWindowClosed()));
            contactChatWindow->show();
        }else{
            ui.mdiArea->setActiveSubWindow(subWindow);
        }

        update();
        showNormal();
        raise();
        QApplication::alert(this);

    }
        break;
    case SeparatedChatWindow:
    {
        contactChatWindow = m_contactChatWidgetHash.value(contactID);
        if(!contactChatWindow){
            contactChatWindow = createContactChatWindow(contact);
        }
        contactChatWindow->show();
        contactChatWindow->activateWindow();
        QApplication::alert(contactChatWindow);
    }
        break;
    default:
        break;
    }


    contactChatWindow->appendMessageReceivedFromContact(message, 0, time);

}

void ChatWindowManager::slotNewChatWithInterestGroup(quint32 interestGroupID){

    InterestGroup *group = ContactsManager::instance()->getInterestGroup(interestGroupID);
    if(!group){
        qCritical()<<"Error:No such interest group:"<<interestGroupID;
        return;
    }

    GroupChatWindow *groupChatWindow = 0;

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        groupChatWindow = findInterestGroupChatTabWidget(group);
        if(!groupChatWindow){
            groupChatWindow = createInterestGroupChatWindow(group);
            ui.tabWidget->addTab(groupChatWindow, ImageResource::createIconForInterestGroup(), group->getGroupName());
        }
        ui.tabWidget->setCurrentWidget(groupChatWindow);
    }
        break;
    case MDIChatWindow:
    {
        QMdiSubWindow * subWindow = findChatWithInterestGroupMdiSubWindow(group);
        if(!subWindow){
            groupChatWindow = createInterestGroupChatWindow(group);
            QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(groupChatWindow);
            subWindow->setWindowIcon(ImageResource::createIconForInterestGroup());
            //connect(subWindow, SIGNAL(destroyed()), this, SLOT(handleChatWindowClosed()));
            groupChatWindow->show();
        }else{
            ui.mdiArea->setActiveSubWindow(subWindow);
        }
    }
        break;
    case SeparatedChatWindow:
    {
        groupChatWindow = m_groupChatWidgetHash.value(interestGroupID);
        if(!groupChatWindow){
            groupChatWindow = createInterestGroupChatWindow(group);
        }
        groupChatWindow->show();
        groupChatWindow->activateWindow();
        QApplication::alert(groupChatWindow);
    }
        break;
    default:
        break;
    }


    QList<InterestGroup::GroupChatMessage> unreadMessages = group->takeUnreadMessages();

    for(int i=0; i<unreadMessages.size(); i++){
        InterestGroup::GroupChatMessage chatMessage = unreadMessages.at(i);
        groupChatWindow->appendMessageReceivedFromContact(chatMessage.message, chatMessage.contact, chatMessage.time);
    }

    //QMap<QString/*Time String*/, QString/*Message*/> unreadMessages = group->takeUnreadMessages();
//    foreach (QString time, unreadMessages.keys()) {
//        groupChatWindow->appendMessageReceivedFromContact(unreadMessages.value(time), 0, time);
//    }


    if(m_chatWindowDisplayStyle != SeparatedChatWindow){
        showNormal();
    }

    activateWindow();
    raise();

//    update();
//    showNormal();
//    raise();

}

void ChatWindowManager::slotNewMessageReceivedFromInterestGroup(quint32 interestGroupID, const QString &contactID, const QString &message, const QString &time){
    qDebug()<<"--ChatWindowManager::slotNewMessageReceivedFromInterestGroup(...) interestGroupID:"<<interestGroupID<<" contactID:"<<contactID<<" message:"<<message;

    InterestGroup *group = ContactsManager::instance()->getInterestGroup(interestGroupID);
    if(!group){
        qCritical()<<"Error:No such interest group:"<<interestGroupID;
        return;
    }
    if(!group->hasMember(contactID)){
        qCritical()<<QString("Error:No such member '%1' in group '%2'!").arg(contactID).arg(interestGroupID);
        return;
    }
    Contact *contact = ContactsManager::instance()->getUser(contactID);
    if(!contact){
        qCritical()<<"Error:No such contact:"<<contactID;
        return;
    }


    GroupChatWindow *groupChatWindow = 0;

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        groupChatWindow = findInterestGroupChatTabWidget(group);
        if(!groupChatWindow){
            groupChatWindow = createInterestGroupChatWindow(group);
            ui.tabWidget->addTab(groupChatWindow, group->getGroupName());
        }
        ui.tabWidget->setCurrentWidget(groupChatWindow);

        update();
        showNormal();
        raise();
    }
        break;
    case MDIChatWindow:
    {
        QMdiSubWindow * subWindow = findChatWithInterestGroupMdiSubWindow(group);
        if(!subWindow){
            groupChatWindow = createInterestGroupChatWindow(group);
            QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(groupChatWindow);
            subWindow->setWindowIcon(ImageResource::createIconForInterestGroup());
            //connect(subWindow, SIGNAL(destroyed()), this, SLOT(handleChatWindowClosed()));
            groupChatWindow->show();
        }else{
            ui.mdiArea->setActiveSubWindow(subWindow);
        }

        update();
        showNormal();
        raise();
    }
        break;
    case SeparatedChatWindow:
    {
        groupChatWindow = m_groupChatWidgetHash.value(interestGroupID);
        if(!groupChatWindow){
            groupChatWindow = createInterestGroupChatWindow(group);
        }
        groupChatWindow->show();
        groupChatWindow->activateWindow();
        QApplication::alert(groupChatWindow);
    }
        break;
    default:
        break;
    }


    groupChatWindow->appendMessageReceivedFromContact(message, contact, time);

}

void ChatWindowManager::initTabWidget(){

    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabPageChanged()));

    QTabBar *tabBar = ui.tabWidget->tabBar();
    QStyleOptionTab opt;
    if (tabBar) {
        opt.init(tabBar);
        opt.shape = tabBar->shape();
    }

    // workaround for sgi style
    //QPalette pal = palette();
    // pal.setColor(QPalette::Active, QPalette::Button, pal.color(QPalette::Active, QPalette::Window));
    // pal.setColor(QPalette::Disabled, QPalette::Button, pal.color(QPalette::Disabled, QPalette::Window));
    // pal.setColor(QPalette::Inactive, QPalette::Button, pal.color(QPalette::Inactive, QPalette::Window));

    QToolButton *newTabButton = new QToolButton(this);
    ui.tabWidget->setCornerWidget(newTabButton, Qt::TopLeftCorner);
    newTabButton->setCursor(Qt::ArrowCursor);
    newTabButton->setAutoRaise(true);
    newTabButton->setIcon(QIcon(":/resources/images/addtab.png"));
    QObject::connect(newTabButton, SIGNAL(clicked()), this, SLOT(slotNewTab()));
    newTabButton->setToolTip(tr("Add Page"));
//    newTabButton->setEnabled(false);

    QToolButton *closeTabButton = new QToolButton(this);
    //closeTabButton->setPalette(pal);
    ui.tabWidget->setCornerWidget(closeTabButton, Qt::TopRightCorner);
    closeTabButton->setCursor(Qt::ArrowCursor);
    closeTabButton->setAutoRaise(true);
    closeTabButton->setIcon(QIcon(":/resources/images/closetab.png"));
    QObject::connect(closeTabButton, SIGNAL(clicked()), this, SLOT(slotcloseTab()));
    closeTabButton->setToolTip(tr("Close Page"));
//    closeTabButton->setEnabled(false);


}

void ChatWindowManager::slotTabPageChanged(){

//    SystemManagementWidget *systemManagementWidget = qobject_cast<SystemManagementWidget *>(ui.tabWidget->currentWidget());
//    if(systemManagementWidget){
//        if(systemManagementWidget == localSystemManagementWidget){
//            ui.tabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(false);
//        }else{
//            ui.tabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(true);
//        }
//    }else{
//        ui.tabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(false);
//    }


}

void ChatWindowManager::slotNewTab(){

    if(m_chatWindowDisplayStyle != TabbedChatWindow) {
        return;
    }

    QWidget *cornerWidget = ui.tabWidget->cornerWidget(Qt::TopLeftCorner);
    QPoint pos = cornerWidget->mapToGlobal(QPoint(0, 0));
    pos.setY(pos.y() + cornerWidget->height());

    QMenu menu;
    QMenu *historymenu = chatHistoryMenu();
    menu.addMenu(historymenu);
    menu.addSeparator();
    menu.addAction(tr("Sub Window View"), this, SLOT(switchToSubWindowView()));
    menu.addAction(tr("Separated Window View"), this, SLOT(switchToSeparatedView()));

    menu.exec(pos);
    historymenu->deleteLater();


}

void ChatWindowManager::slotcloseTab(){

    QWidget *wgt = ui.tabWidget->currentWidget();

    ContactChatWidget *ccw = qobject_cast<ContactChatWidget *>(wgt);
    if(ccw){
        if(!ccw->close()){
            return;
        }
        ui.tabWidget->removeTab(ui.tabWidget->currentIndex());
        ccw->deleteLater();
        wgt = 0;
        //m_contactChatWidgetHash.remove(ccw->contact()->getUserID());
    }


    GroupChatWindow *gcw = qobject_cast<GroupChatWindow *>(wgt);
    if(gcw){
        if(!gcw->close()){
            return;
        }
        ui.tabWidget->removeTab(ui.tabWidget->currentIndex());
        gcw->deleteLater();
        //m_groupChatWidgetHash.remove(gcw->interestGroup()->getGroupID());
    }

//    if(ui.tabWidget->count() == 0){
//        showMinimized();
//        hide();
//    }

    //如果只有一页，则关闭按钮不可用
    //ui.tabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(ui.tabWidget->count() > 1);


}


//void ChatWindowManager::handleChatWindowClosed(){

//    QMdiSubWindow *sw = qobject_cast<QMdiSubWindow *>(sender());

//    ContactChatWidget *ccw = qobject_cast<ContactChatWidget *>(sw->widget());
//    if(ccw){
//        ui.tabWidget->removeTab(ui.tabWidget->currentIndex());
//        ccw->close();
//        ccw->deleteLater();
//        m_contactChatWidgetHash.remove(ccw->contact()->getUserID());
//    }

//    GroupChatWindow *gcw = qobject_cast<GroupChatWindow *>(sw->widget());
//    if(gcw){
//        ui.tabWidget->removeTab(ui.tabWidget->currentIndex());
//        gcw->close();
//        gcw->deleteLater();
//        m_groupChatWidgetHash.remove(gcw->interestGroup()->getGroupID());
//    }

//    if(!ui.mdiArea->subWindowList().size()){
//        showMinimized();
//        hide();
//    }
    
//}

void ChatWindowManager::handleCloseWindowRequest(){

    switch (m_chatWindowDisplayStyle) {
    case TabbedChatWindow:
    {
        slotcloseTab();
    }
        break;
    case MDIChatWindow:
    {
        QMdiSubWindow * subWindow = ui.mdiArea->currentSubWindow();
        if(subWindow){
            subWindow->close();
        }
    }
        break;
    case SeparatedChatWindow:
    {
        ContactChatWidget *ccw = qobject_cast<ContactChatWidget *>(sender());
        if(ccw){
            ccw->close();
        }

        GroupChatWindow *gcw = qobject_cast<GroupChatWindow *>(sender());
        if(gcw){
            gcw->close();
        }

    }
        break;
    default:
        break;
    }

}

void ChatWindowManager::handleChatWindowClosed(){
    qDebug()<<"----ChatWindowManager::handleChatWindowClosed()";

    ContactChatWidget *ccw = qobject_cast<ContactChatWidget *>(sender());
    if(ccw){
        m_contactChatWidgetHash.remove(ccw->contact()->getUserID());
    }

    GroupChatWindow *gcw = qobject_cast<GroupChatWindow *>(sender());
    if(gcw){
        m_groupChatWidgetHash.remove(gcw->interestGroup()->getGroupID());
    }

    if(((m_chatWindowDisplayStyle == MDIChatWindow) && (!ui.mdiArea->subWindowList().size()))
            || ((m_chatWindowDisplayStyle == TabbedChatWindow) && (!ui.tabWidget->count())) ){
        showMinimized();
        hide();
    }

}


void ChatWindowManager::showContextMenu(const QPoint &pos){

    if(m_chatWindowDisplayStyle != MDIChatWindow) {
        return;
    }


    QMenu menu;
    QMenu *historymenu = chatHistoryMenu();
    menu.addMenu(historymenu);
    menu.addSeparator();
    menu.addAction(tr("Cascade Windows"), ui.mdiArea, SLOT(cascadeSubWindows()));
    menu.addAction(tr("Tile Windows"), ui.mdiArea, SLOT(tileSubWindows()));
    menu.addSeparator();
    menu.addAction(tr("Tabbed Window View"), this, SLOT(switchToTabbedView()));
    menu.addAction(tr("Separated Window View"), this, SLOT(switchToSeparatedView()));


    menu.exec(mapToGlobal(pos));
    historymenu->deleteLater();

}

void ChatWindowManager::handleContactChatHistoryMenuAction(){
    QAction *act = qobject_cast<QAction *>(sender());
    QString contactID = act->data().toString();
    slotNewChatWithContact(contactID);

}

void ChatWindowManager::handleGroupChatHistoryMenuAction(){
    QAction *act = qobject_cast<QAction *>(sender());
    quint32 groupID = act->data().toUInt();
    slotNewChatWithInterestGroup(groupID);

}

void ChatWindowManager::switchToSubWindowView(){
    switchChatWindowDisplayStyle(MDIChatWindow);
}

void ChatWindowManager::switchToTabbedView(){
    switchChatWindowDisplayStyle(TabbedChatWindow);
}

void ChatWindowManager::switchToSeparatedView(){
    switchChatWindowDisplayStyle(SeparatedChatWindow);
}

void ChatWindowManager::sendUploadingFileRequest(const QString &filePath, const QByteArray &fileMD5, bool offline){

    ContactChatWidget *wgt = qobject_cast<ContactChatWidget*>(sender());
    if(!wgt){return;}
    Contact *contact = wgt->contact();
    //TODO

    if(offline){
        if(!initFileTransmission()){
            wgt->cancelFileTransmission(fileMD5);
            return;
        }

//        while (m_socketConnectedToServer == INVALID_SOCK_ID) {
//            m_socketConnectedToServer = m_fileTransmissionPacketsParser->connectToServer();

//            if(m_socketConnectedToServer == INVALID_SOCK_ID){
//                int btn = QMessageBox::critical(this, tr("Connection Failed"), tr("Can not connect to server!"),
//                                                QMessageBox::Retry | QMessageBox::Cancel,
//                                                QMessageBox::Retry
//                                                );
//                if(btn == QMessageBox::Cancel){
//                    wgt->cancelFileTransmission(fileMD5);
//                    return;
//                }

//            }
//        }
        QFileInfo info(filePath);
        m_fileTransmissionPacketsParser->requestUploadFile(m_socketConnectedToServer, contact->getUserID(), fileMD5, info.fileName(), info.size());

    }else{
        emit signalSendUploadingFileRequest(contact, filePath, fileMD5);
    }

}

void ChatWindowManager::cancelSendingFileRequest(const QByteArray &fileMD5){

    ContactChatWidget *wgt = qobject_cast<ContactChatWidget*>(sender());
    if(!wgt){return;}
    Contact *contact = wgt->contact();

    emit signalCancelSendingFileUploadingRequest(contact, fileMD5);

}

void ChatWindowManager::abortFileTransmission(const QByteArray &fileMD5){

    ContactChatWidget *wgt = qobject_cast<ContactChatWidget*>(sender());
    if(!wgt){return;}
    Contact *contact = wgt->contact();

    //m_fileTransmissionManagerBase->abortFileTransmission();

    //TODO

}

void ChatWindowManager::acceptPeerUploadFileRequest(const QByteArray &fileMD5, const QString &localSavePath){

    ContactChatWidget *wgt = qobject_cast<ContactChatWidget*>(sender());
    if(!wgt){return;}
    Contact *contact = wgt->contact();

    emit signalAcceptPeerUploadFileRequest(contact, fileMD5, localSavePath);





}

void ChatWindowManager::declineFileRequest(const QByteArray &fileMD5){

    ContactChatWidget *wgt = qobject_cast<ContactChatWidget*>(sender());
    if(!wgt){return;}
    Contact *contact = wgt->contact();

    emit signalDeclinePeerUploadFileRequest(contact, fileMD5);

}

void ChatWindowManager::contactRequestUploadFile(const QString &contactID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size){

    ContactChatWidget *ccw = m_contactChatWidgetHash.value(contactID);
    if(!ccw){return;}
    ccw->slotFileRequestReceivedFromContact(fileName, size, fileMD5Sum);
}

void ChatWindowManager::contactRequestDownloadFile(const QString &contactID, const QString &localBaseDir, const QString &fileName){

}

void ChatWindowManager::fileDownloadRequestAccepted(const QString &contactID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size){
    //TODO:

    if(!initFileTransmission()){
        return;
    }

}

void ChatWindowManager::fileDownloadRequestDenied(const QString &contactID, const QString &remoteFileName, const QString &message){
    //TODO
}

void ChatWindowManager::fileUploadRequestResponsed(const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message){

    ContactChatWidget *ccw = m_contactChatWidgetHash.value(contactID);
    if(!ccw){return;}

    ccw->fileUploadRequestResponsed(fileMD5Sum, accepted, message);

    //TODO
    if(!accepted){return;}

    if(!initFileTransmission()){
        return;
    }

}

ContactChatWidget * ChatWindowManager::createContactChatWindow(Contact *contact){

        ContactChatWidget *contactChatWindow = new ContactChatWidget(contact);
        contactChatWindow->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));

        connect(contactChatWindow, SIGNAL(sendMsgButtonClicked(Contact *, const QString &, const QStringList &)), this, SIGNAL(signalSendChatMessageToCantact(Contact *, const QString &, const QStringList &)));
        connect(contactChatWindow, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)), this, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)));

        connect(contactChatWindow, SIGNAL(signalCloseWindow()), this, SLOT(handleCloseWindowRequest()));
        connect(contactChatWindow, SIGNAL(toBeDstroyed()), this, SLOT(handleChatWindowClosed()));
        connect(contactChatWindow, SIGNAL(signalRequestContactHistoryMessage(const QString &, const QString &, const QString &, bool, const QString &)), this, SIGNAL(signalRequestContactHistoryMessage(const QString &, const QString &, const QString &, bool, const QString &)));


        //FILE TX
        connect(contactChatWindow, SIGNAL(signalSendUploadingFileRequest(const QString &, const QByteArray &, bool)), this, SLOT(sendUploadingFileRequest(const QString &, const QByteArray &, bool)) );
        connect(contactChatWindow, SIGNAL(signalCancelSendingUploadingFileRequest(const QByteArray &)), this, SLOT(cancelSendingFileRequest(const QByteArray &)) );
        connect(contactChatWindow, SIGNAL(signalAbortFileTransmission(const QByteArray &)), this, SLOT(abortFileTransmission(const QByteArray &)) );
        connect(contactChatWindow, SIGNAL(signalAcceptPeerUploadFileRequest(const QByteArray &, const QString &)), this, SLOT(acceptPeerUploadFileRequest(const QByteArray &, const QString &)) );
        connect(contactChatWindow, SIGNAL(signalDeclinePeerUploadFileRequest(const QByteArray &)), this, SLOT(declineFileRequest(const QByteArray &)) );



        QString contactID = contact->getUserID();
        m_contactChatWidgetHash.insert(contactID, contactChatWindow);

        if(!m_contactChatHistoryList.contains(contactID, Qt::CaseInsensitive)){
            m_contactChatHistoryList.append(contactID);
        }

//        QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(contactChatWindow);
//        //subWindow->setWindowIcon(ImageResource::createMixedIcon((QString(RESOURCE_PATH) + "/" +contact->getFace()), contact->getOnlineState()));
//        subWindow->setWindowIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));

//        connect(subWindow, SIGNAL(destroyed()), this, SLOT(chatWindowClosed()));


        return contactChatWindow;

}

QMdiSubWindow* ChatWindowManager::findChatWithContactMdiSubWindow(Contact *contact){

    foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
        ContactChatWidget *messageWindow = qobject_cast<ContactChatWidget *>(window->widget());
        if(!messageWindow){continue;}
        if(messageWindow->contact() == contact){
            return window;
        }
    }

    return 0;

}

ContactChatWidget * ChatWindowManager::findContactChatTabWidget(Contact *contact){

    int tabPages = ui.tabWidget->count();
    for(int i = tabPages-1; i >= 0; i--){
        ContactChatWidget *ccw = qobject_cast<ContactChatWidget *>(ui.tabWidget->widget(i));
        if(!ccw){continue;}
        if(ccw->contact() == contact){
            return ccw;
        }
    }

    return 0;
}

GroupChatWindow* ChatWindowManager::createInterestGroupChatWindow(InterestGroup *group){

    //InterestGroup *group = ContactsManager::instance()->getInterestGroup(interestGroupID);
    if(!group){
            qCritical()<<"Error:No such interest group!";
            return 0;
    }

    GroupChatWindow *groupChatWindow = new GroupChatWindow(group);
    groupChatWindow->setWindowIcon(ImageResource::createIconForInterestGroup());

    connect(groupChatWindow, SIGNAL(sendMsgButtonClicked(InterestGroup *, const QString &, const QStringList &)), this, SIGNAL(signalSendChatMessageToInterestGroup(InterestGroup *, const QString &, const QStringList &)));
    connect(groupChatWindow, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)), this, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)));

    connect(groupChatWindow, SIGNAL(signalCloseWindow()), this, SLOT(handleCloseWindowRequest()));
    connect(groupChatWindow, SIGNAL(toBeDstroyed()), this, SLOT(handleChatWindowClosed()));

    connect(groupChatWindow, SIGNAL(signalRequestGrouptHistoryMessage(const QString &, const QString &, const QString &, bool, quint32)), this, SIGNAL(signalRequestGrouptHistoryMessage(const QString &, const QString &, const QString &, bool, quint32)));

    quint32 groutID = group->getGroupID();
    m_groupChatWidgetHash.insert(groutID, groupChatWindow);

    if(!m_groupChatHistoryList.contains(groutID)){
        m_groupChatHistoryList.append(groutID);
    }


//    QMdiSubWindow *subWindow = ui.mdiArea->addSubWindow(groupChatWindow);
//    subWindow->setWindowIcon(ImageResource::createIconForInterestGroup());
//    connect(subWindow, SIGNAL(destroyed()), this, SLOT(chatWindowClosed()));

    return groupChatWindow;
}

QMdiSubWindow* ChatWindowManager::findChatWithInterestGroupMdiSubWindow(InterestGroup *group){

    foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
            GroupChatWindow *groupChatWindow = qobject_cast<GroupChatWindow *>(window->widget());
            if(!groupChatWindow){continue;}
            if(groupChatWindow->interestGroup() == group){
                    return window;
            }
    }

    return 0;

}

GroupChatWindow * ChatWindowManager::findInterestGroupChatTabWidget(InterestGroup *group){

    int tabPages = ui.tabWidget->count();
    for(int i = tabPages-1; i >= 0; i--){
        GroupChatWindow *gcw = qobject_cast<GroupChatWindow *>(ui.tabWidget->widget(i));
        if(!gcw){continue;}
        if(gcw->interestGroup() == group){
            return gcw;
        }
    }

    return 0;

}

bool ChatWindowManager::initFileTransmission(){

    if(!m_fileTransmissionPacketsParser){

        IMUser *myself = IMUser::instance();
        QString myID = myself->getUserID();

        //TODO:Get File Server Info
        emit signalRequestFileServerInfo();

        int n = 0;
        while (myself->getFileServerPort() == 0) {
            Utilities::msleep(500);
            qApp->processEvents();

            n++;
            if(10 == n){
                QMessageBox::critical(this, tr("Error"), tr("Timeout! Can not get file server info!"));
                return false;
            }
        }

        m_fileTransmissionPacketsParser = new ClientFileTransmissionPacketsParser(myID, this);
        m_fileTransmissionManager = new ClientFileTransmissionManager(myID, m_fileTransmissionPacketsParser, this);

    }

    while (m_socketConnectedToServer == INVALID_SOCK_ID) {
        m_socketConnectedToServer = m_fileTransmissionPacketsParser->connectToServer();

        if(m_socketConnectedToServer == INVALID_SOCK_ID){
            int btn = QMessageBox::critical(this, tr("Connection Failed"), tr("Can not connect to file server!"),
                                            QMessageBox::Retry | QMessageBox::Cancel,
                                            QMessageBox::Retry
                                            );
            if(btn == QMessageBox::Cancel){
                return false;
            }
        }
    }


    return true;

}

QMenu *ChatWindowManager::chatHistoryMenu(){

    QMenu *menu = new QMenu(tr("History"));

    foreach (QString  contactID, m_contactChatHistoryList) {
        Contact *contact = ContactsManager::instance()->getUser(contactID);
        if(!contact){
            m_contactChatHistoryList.removeAll(contactID);
            continue;
        }
        QString displayName = contact->displayName();
        if(contactID != displayName){
            displayName = displayName + "("  + contactID + ")";
        }

        QAction *act = menu->addAction(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()), displayName, this, SLOT(handleContactChatHistoryMenuAction()));
        act->setData(QVariant(contactID));
    }

    foreach (quint32 groupID, m_groupChatHistoryList) {
        InterestGroup *group = ContactsManager::instance()->getInterestGroup(groupID);
        if(!group){
            m_groupChatHistoryList.removeAll(groupID);
            continue;
        }
        QAction *act = menu->addAction(ImageResource::createIconForInterestGroup(), group->getGroupName(), this, SLOT(handleGroupChatHistoryMenuAction()));
        act->setData(QVariant(groupID));

    }

    return menu;

}


//bool ChatWindowManager::isChatWindowOpened(Contact *contact){
//	return false;
//}

























} //namespace HEHUI
