

#include <QMessageBox>
#include <QInputDialog>


#include "search.h"

#include "../../sharedim/constants_global_shared.h"
#include "../imuser.h"
#include "addcontactdialog.h"


namespace HEHUI{

//Search * Search::m_searchInstance = 0;

Search::Search(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ui.comboBoxGender->addItem(tr("Any"), QVariant(IMUser::GENDER_UNKNOWN));
    ui.comboBoxGender->addItem(tr("Male"), QVariant(IMUser::MALE));
    ui.comboBoxGender->addItem(tr("Female"), QVariant(IMUser::FEMALE));
    ui.comboBoxGender->setCurrentIndex(0);

    ui.comboBoxAge->addItem(tr("Any"), QVariant(IMUser::Age_Any));
    ui.comboBoxAge->addItem("18-", QVariant(IMUser::Age_1_18));
    ui.comboBoxAge->addItem("19-30", QVariant(IMUser::Age_19_30));
    ui.comboBoxAge->addItem("31-40", QVariant(IMUser::Age_31_40));
    ui.comboBoxAge->addItem("40+", QVariant(IMUser::Age_40_));
    ui.comboBoxAge->setCurrentIndex(0);



    //        ui.radioButtonUsersMatchWildcard->click();
    //        ui.radioButtonGroupsMatchWildcard->click();

    ui.radioButtonUsersMatchExactly->click();
    ui.lineEditUserID->setFocus();

    connect(this, SIGNAL(accepted()), this, SLOT(reset()));
    connect(this, SIGNAL(rejected()), this, SLOT(reset()));



    connect(ui.tableViewUsersResult, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotUserSelected(const QModelIndex &)));
    connect(ui.tableViewUsersResult, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotAddUserAsAContact(const QModelIndex &)));

    connect(ui.tableViewGroupsResult, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotGroupSelected(const QModelIndex &)));
    connect(ui.tableViewGroupsResult, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotJoinGroup(const QModelIndex &)));


    contactProperties = "";
    matchExactly = searchOnlineUsersOnly = true;
    searchWebcamUsersOnly = false;

    groupKeyword = "";


    userInfoModel = new UserInfoModel(this);
    ui.tableViewUsersResult->setModel(userInfoModel);
    //ui.tableViewUsersResult->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui.tableViewUsersResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    groupInfoModel = new InterestGroupInfoModel(this);
    ui.tableViewGroupsResult->setModel(groupInfoModel);
    //ui.tableViewGroupsResult->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui.tableViewGroupsResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    searchContactTimer.setSingleShot(true);
    searchContactTimer.setInterval(5000);
    connect(&searchContactTimer, SIGNAL(timeout()), this, SLOT(searchContactTimeout()));

    searchGroupTimer.setSingleShot(true);
    searchGroupTimer.setInterval(5000);
    connect(&searchGroupTimer, SIGNAL(timeout()), this, SLOT(searchGroupTimeout()));

    usersResultPageIndex = 1;
    groupsResultPageIndex = 1;

}

Search::~Search()
{
    
    reset();
    
    this->disconnect();


    delete userInfoModel;
    delete groupInfoModel;
    
    
}

void Search::slotSearchContactsResultPacketReceived(const QString &usersString){

    searchContactTimer.stop();
    ui.pushButtonSearchContact->setEnabled(true);

    userInfoModel->setUsersInfoString(usersString);

    if(userInfoModel->rowCount() == SEARCH_RESULT_PAGE_SIZE){
        ui.toolButtonNextUsersResultPage->setEnabled(true);
    }

}


void Search::slotSearchInterestGroupsResultPacketReceived(const QString &groupsString){
    searchGroupTimer.stop();
    ui.pushButtonSearchGroup->setEnabled(true);

    groupInfoModel->setGroupsInfo(groupsString);

    if(groupInfoModel->rowCount() == SEARCH_RESULT_PAGE_SIZE){
        ui.toolButtonNextGroupsResultPage->setEnabled(true);
    }

}


void Search::reset(){   

    contactProperties = "";
    matchExactly = searchOnlineUsersOnly = true;
    searchWebcamUsersOnly = false;
    
    ui.radioButtonUsersMatchExactly->click();
    ui.lineEditUserID->setFocus();
    
    ui.lineEditUserID->clear();
    ui.lineEditNickname->clear();
    
    ui.comboBoxGender->setCurrentIndex(0);
    ui.comboBoxAge->setCurrentIndex(0);
    ui.tableViewUsersResult->reset();

    ui.pushButtonSearchContact->setEnabled(true);



    //TODO:重置
    groupKeyword = "";

    groupInfoModel->setGroupsInfo("");
    ui.tableViewGroupsResult->reset();
    ui.lineEditGroupKeyword->clear();

    ui.pushButtonSearchGroup->setEnabled(true);

    searchContactTimer.stop();
    searchGroupTimer.stop();
    
    
}

void Search::slotUserSelected(const QModelIndex &index){
    if(!index.isValid()){
        ui.pushButtonUserDetails->setEnabled(false);
        ui.pushButtonAddAsContact->setEnabled(false);
    }else{
        ui.pushButtonUserDetails->setEnabled(true);
        ui.pushButtonAddAsContact->setEnabled(true);
    }
    
}

void Search::slotAddUserAsAContact(const QModelIndex &index){

    if(!index.isValid()){
        return;
    }
    
    ui.pushButtonAddAsContact->click();
    
    //    int row = index.row();
    //    Contact *user = usersHash.values().at(row);
    //    QMessageBox::information(this,"", user->getUserID());
    
}

void Search::slotGroupSelected(const QModelIndex &index){

    if(!index.isValid()){
        ui.pushButtonGroupDetails->setEnabled(false);
        ui.pushButtonJoinGroup->setEnabled(false);
    }else{
        ui.pushButtonGroupDetails->setEnabled(true);
        ui.pushButtonJoinGroup->setEnabled(true);
    }

}

void Search::slotJoinGroup(const QModelIndex &index){

    //QModelIndex index = ui.tableViewGroupsResult->currentIndex();
    if(!index.isValid()){
        return;
    }

    ui.pushButtonJoinGroup->click();

    int row = index.row();

    GroupInfo groupInfo = groupInfoModel->getGroupInfo(row);
    if(groupInfo.isNull()){return;}

    QMessageBox::information(this, QString::number(groupInfo.groupID), groupInfo.groupName);

}




void Search::on_tabWidget_currentChanged( int index ){
    //    if(ui.tabWidget->currentWidget() == ui.tabUsers){

    //    }else{

    //    }

}


void Search::on_pushButtonSearchContact_clicked(){

    contactProperties = "";

    QStringList propertiesList;
    QString userID = ui.lineEditUserID->text().trimmed();
    QString nickName = ui.lineEditNickname->text();
    quint8 age = IMUserBase::Age_Any;
    if(ui.comboBoxAge->currentIndex() != 0){
        age = ui.comboBoxAge->itemData(ui.comboBoxAge->currentIndex()).toUInt();
        qDebug()<<"----------age"<<age;
    }
    quint8 gender = IMUser::GENDER_UNKNOWN;
    if(ui.comboBoxGender->currentIndex() != 0){
        gender = ui.comboBoxGender->itemData(ui.comboBoxGender->currentIndex()).toUInt();
    }
    QString hometown = ui.lineEditHomeAddress->text().trimmed();
    QString businessAddress = ui.lineEditBusinessAddress->text().trimmed();

    matchExactly = ui.radioButtonUsersMatchExactly->isChecked();
    searchOnlineUsersOnly = ui.checkBoxUserSearchOnlineUsers->isChecked();
    searchWebcamUsersOnly = ui.checkBoxUserSearchWebcamUsers->isChecked();


    if(matchExactly){
        if(!userID.isEmpty()){
            if(userID == IMUser::instance()->getUserID()){
                //QMessageBox::critical(this, tr("Error"), tr("Please input a valid user id!"));
                ui.lineEditUserID->clear();
                return;
            }
            if(IMUser::instance()->isFriendContact(userID)){
                QMessageBox::critical(this, tr("Search"), tr("'%1' is already your contact!").arg(userID));
                ui.lineEditUserID->clear();
                return;
            }
        }else{
            if(nickName.isEmpty()){
                QMessageBox::critical(this, tr("Search"), tr("Keyword needed!"));
                ui.lineEditUserID->setFocus();
                return;
            }
        }
        searchOnlineUsersOnly = searchWebcamUsersOnly = false;
    }

    propertiesList << userID << nickName << QString::number(age) << QString::number(gender) <<hometown << businessAddress;
    contactProperties = propertiesList.join(QString(CONTACT_INFO_SEPARATOR));

    emit signalSearchContact(contactProperties, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, 0);

    searchContactTimer.start();

    ui.pushButtonSearchContact->setEnabled(false);
    userInfoModel->setUsersInfoString("");
    ui.toolButtonPreviousUsersResultPage->setEnabled(false);
    ui.toolButtonNextUsersResultPage->setEnabled(false);

    usersResultPageIndex = 1;


}


void Search::on_radioButtonUsersMatchWildcard_clicked(){
    ui.stackedWidgetUsersSearchCondition->setCurrentWidget(ui.pageUsersSearchMatchWildcard);
}

void Search::on_radioButtonUsersMatchExactly_clicked(){
    ui.stackedWidgetUsersSearchCondition->setCurrentWidget(ui.pageUsersSearchMatchExactly);
}


void Search::on_pushButtonUserDetails_clicked(){
    QModelIndex index = ui.tableViewUsersResult->currentIndex();
    if(!index.isValid()){
        return;
    }
    int row = index.row();
    

    ContactInfo contactInfo = userInfoModel->getContactInfo(row);
    if(contactInfo.isNull()){return;}

    QMessageBox::information(this,"", contactInfo.userID);
    
    
}

void Search::on_pushButtonAddAsContact_clicked(){

    QModelIndex index = ui.tableViewUsersResult->currentIndex();
    if(!index.isValid()){
        return;
    }
    int row = index.row();
    
    ContactInfo contactInfo = userInfoModel->getContactInfo(row);
    if(contactInfo.isNull()){return;}

    QString userID = contactInfo.userID;
    if(userID == IMUser::instance()->getUserID()){
        QMessageBox::critical(this, tr("Error"), tr("You can't add yourself as a contact!"));
        return;
    }
    if(IMUser::instance()->isFriendContact(userID)){
        QMessageBox::critical(this, tr("Error"), tr("'%1' was already your contact!").arg(userID));
        return;
    }
    
    Contact user(userID, contactInfo.nickName, this);
    user.setGender(contactInfo.gender);
    user.setAge(contactInfo.age);
    user.setFace(contactInfo.face);
    user.setFriendshipApply(contactInfo.friendshipApply);
    user.setBusinessAddress(contactInfo.businessAddress);
    user.setOnlineState(contactInfo.onlineState);

    AddContactDialog dlg(&user, true, this);
    if(dlg.exec()  == QDialog::Accepted){
        emit signalAddContact(userID, dlg.getMessage());
    }
    
    
    //    if(user->getFriendshipApply() == Contact::FA_AUTO_ACCEPT){
    
    //    }
    
    
    //    QMessageBox::information(this,"", user->getUserID());
    
    
}

void Search::on_toolButtonPreviousUsersResultPage_clicked(){

    usersResultPageIndex--;

    emit signalSearchContact(contactProperties, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, (usersResultPageIndex-1)*SEARCH_RESULT_PAGE_SIZE);

    searchContactTimer.start();

    ui.pushButtonSearchContact->setEnabled(false);
    userInfoModel->setUsersInfoString("");
    ui.toolButtonPreviousUsersResultPage->setEnabled( (usersResultPageIndex > 1) );
    ui.toolButtonNextUsersResultPage->setEnabled(true);

}

void Search::on_toolButtonNextUsersResultPage_clicked(){

    usersResultPageIndex++;

    emit signalSearchContact(contactProperties, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, (usersResultPageIndex-1)*SEARCH_RESULT_PAGE_SIZE);

    searchContactTimer.start();

    ui.pushButtonSearchContact->setEnabled(false);
    userInfoModel->setUsersInfoString("");
    ui.toolButtonPreviousUsersResultPage->setEnabled(true);
    ui.toolButtonNextUsersResultPage->setEnabled(false);

}

void Search::on_pushButtonSearchGroup_clicked(){

    groupKeyword = ui.lineEditGroupKeyword->text().trimmed();

    if(groupKeyword.isEmpty()){
        QMessageBox::critical(this, tr("Search"), tr("Keyword needed!"));
        ui.lineEditGroupKeyword->setFocus();
        return;
    }

    emit signalSearchInterestGroup(groupKeyword, 0);

    searchGroupTimer.start();

    ui.pushButtonSearchGroup->setEnabled(false);
    groupInfoModel->setGroupsInfo("");
    ui.toolButtonPreviousGroupsResultPage->setEnabled(false);
    ui.toolButtonNextGroupsResultPage->setEnabled(false);

    groupsResultPageIndex = 1;


}

void Search::on_pushButtonGroupDetails_clicked(){

    QModelIndex index = ui.tableViewGroupsResult->currentIndex();
    if(!index.isValid()){
        qCritical()<<"Error! Invalid Model Index!";
        return;
    }
    int row = index.row();
    GroupInfo groupInfo = groupInfoModel->getGroupInfo(row);
    if(groupInfo.isNull()){return;}

    QMessageBox::information(this, QString::number(groupInfo.groupID), groupInfo.groupName);



}

void Search::on_pushButtonJoinGroup_clicked(){

    QModelIndex index = ui.tableViewGroupsResult->currentIndex();
    if(!index.isValid()){
        return;
    }
    int row = index.row();

    GroupInfo groupInfo = groupInfoModel->getGroupInfo(row);
    if(groupInfo.isNull()){return;}

    if(groupInfo.privacy == quint8(InterestGroupBase::Only_Invited_Can_Join)){
        QMessageBox::critical(this, tr("Join a Group"), tr("Only invited user can join this group!"));
        return;
    }

    QString verificationMessage = "";
    if(groupInfo.privacy == quint8(InterestGroupBase::Request_Verfication_To_Join)){

        bool ok = false;
        QString labelText = tr("Verification Message:");
        verificationMessage = QInputDialog::getText(this, tr("Join a Group"),
                                                     labelText, QLineEdit::Normal,
                                                     "", &ok);
        if (!ok){
            return;
        }

    }


    emit signalJoinInterestGroup(groupInfo.groupID, verificationMessage);

//    QMessageBox::information(this, QString::number(groupInfo.groupID), groupInfo.groupName);

}

void Search::on_toolButtonPreviousGroupsResultPage_clicked(){

    groupsResultPageIndex--;

    emit signalSearchInterestGroup(groupKeyword, (groupsResultPageIndex-1)*SEARCH_RESULT_PAGE_SIZE);

    searchGroupTimer.start();

    ui.pushButtonSearchGroup->setEnabled(false);
    groupInfoModel->setGroupsInfo("");
    ui.toolButtonPreviousGroupsResultPage->setEnabled( (groupsResultPageIndex > 1) );
    ui.toolButtonNextGroupsResultPage->setEnabled(true);

}

void Search::on_toolButtonNextGroupsResultPage_clicked(){

    groupsResultPageIndex++;

    emit signalSearchInterestGroup(groupKeyword, (groupsResultPageIndex-1)*SEARCH_RESULT_PAGE_SIZE);

    searchGroupTimer.start();

    ui.pushButtonSearchGroup->setEnabled(false);
    groupInfoModel->setGroupsInfo("");
    ui.toolButtonPreviousGroupsResultPage->setEnabled(true);
    ui.toolButtonNextGroupsResultPage->setEnabled(false);


}


void Search::searchContactTimeout(){
    ui.pushButtonSearchContact->setEnabled(true);
    //QMessageBox::critical(this, tr("Search"), tr("Request Timeout!"));
}

void Search::searchGroupTimeout(){
    ui.pushButtonSearchGroup->setEnabled(true);
    //QMessageBox::critical(this, tr("Search"), tr("Request Timeout!"));
}












} //namespace HEHUI



