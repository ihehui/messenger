/*
 * contactsmanager.cpp
 *
 *  Created on: 2009-9-12
 *      Author: 贺辉
 */

#include <QDebug>
#include <QSqlError>

#include "contactsmanager.h"
#include "contact.h"
#include "../settings.h"
#include "../imageresource.h"
//#include "../shared/global_app.h"

#include "../servertime/servertime.h"

//#include "../../shared/core/database/databaseutility.h"
//#include "../../shared/core/user.h"
//#include "../../shared/gui/databaseconnecter/databaseconnecter.h"
#include "HHSharedCore/hdatabaseutility.h"
#include "HHSharedCore/huser.h"
#include "HHSharedGUI/hdatabaseconnecter.h"



namespace HEHUI {


//QHash<int, ContactGroup*> ContactsManager::contactGroupHash = QHash<int, ContactGroup*>();
//QHash<QString, Contact*> ContactsManager::contactHash = QHash<QString, Contact*>();

ContactsManager::ContactsManager(QObject *parent)
    :QObject(parent)
{
    qDebug()<<"----ContactsManager::ContactsManager(QObject *parent)~~";

    //        query = 0;
    //userPrivateDataFilePath = Settings::instance()->getUserPrivateDataFilePath(IMUser::instance()->getUserID());
    //openDatabase();

    m_myself = IMUser::instance();
    //Q_ASSERT_X(m_imUser->getOnlineState() != IM::ONLINESTATE_OFFLINE, "ContactsManager::ContactsManager(QObject *parent)", "User is not online!");


}

ContactsManager::~ContactsManager() {
    
    //    if(query){
    //        query->clear();
    //    }
    //    delete query;
    //    query = 0;
    
    
    
    
}

bool ContactsManager::isFriendContact(const QString &contactID){

    //TODO:改进

//    if(!hasUserInfo(contactID)){
//        return false;
//    }

    Contact * contact = contactHash.value(contactID);
    if(!contact){return false;}
    int groupID = contact->getContactGroupID();
    if((groupID == ContactGroupBase::Group_Strangers_ID) || (groupID == ContactGroupBase::Group_Blacklist_ID)){
        return false;
    }

    return true;

}

bool ContactsManager::isStranger(const QString &contactID){
    Contact * contact = contactHash.value(contactID);
    if(!contact){return false;}
    int groupID = contact->getContactGroupID();
    if((groupID == ContactGroupBase::Group_Strangers_ID)){
        return true;
    }

    return false;
}

bool ContactsManager::hasUserInfo(const QString &userID){
//    bool has = false;
//    QStringList contacts = contactHash.keys();
//    foreach (QString id, contacts) {
//        if(id == userID){
//            has = true;
//            break;
//        }
//    }

//    return has;

    return contactHash.contains(userID);

}

Contact * ContactsManager::getUser(const QString &contactID){
    Contact * contact = 0;
    if(contactHash.contains(contactID)){
        contact = contactHash.value(contactID);
    }else{
        //contact = new Contact(contactID, this);
    }

    return contact;

}

QHash<QString, Contact *> ContactsManager::getAllUsers(){
    return contactHash;
}

bool ContactsManager::loadInterestGroups(){

    QString groupQueryString = QString("select * from interestgroups");
    
    QSqlQueryModel *model = new QSqlQueryModel(this);
    
    model->setQuery(queryDatabase(groupQueryString, true));
    if(model->lastError().type() != QSqlError::NoError){
        qCritical()<<"Query Error!"<<model->lastError().text();
        delete model;
        return false;
    }

    for (int j=0; j<model->rowCount(); j++) {
        quint32 groupID = QVariant(model->record(j).value("GroupID")).toUInt();
        InterestGroup *interestGroup = new InterestGroup(groupID, "", this);
        interestGroup->setGroupTypeID(QVariant(model->record(j).value("TypeID")).toUInt());
        interestGroup->setParentGroupID(QVariant(model->record(j).value("ParentGroup")).toUInt());
        interestGroup->setCreatorID(QVariant(model->record(j).value("Creator")).toString());
        
        interestGroup->setGroupName(QVariant(model->record(j).value("GroupName")).toString());
        interestGroup->setCreationTime(QVariant(model->record(j).value("CreationTime")).toDateTime());
        interestGroup->setGroupInfoVersion(QVariant(model->record(j).value("GroupInfoVersion")).toUInt());
        
        interestGroup->setGroupMemberListInfoVersion(QVariant(model->record(j).value("MemberListVersion")).toUInt());
        interestGroup->setDescription(QVariant(model->record(j).value("Description")).toString());
        interestGroup->setAnnouncement(QVariant(model->record(j).value("Announcement")).toString());
        interestGroup->setRemark(QVariant(model->record(j).value("Remark")).toString());
        interestGroup->setState(QVariant(model->record(j).value("State")).toUInt());

        interestGroup->clearUpdatedProperties();
        
        interestGroupsHash.insert(groupID, interestGroup);
        //m_myself->addInterestGroup(groupID);

        fetchAllInterestGroupMembers(interestGroup);

        qApp->processEvents();
    }
    
    
    model->deleteLater();

    return true;
    
}

InterestGroup * ContactsManager::getInterestGroup(quint32 groupID){

    InterestGroup * group = 0;
    if(interestGroupsHash.contains(groupID)){
        group = interestGroupsHash.value(groupID);
    }
    
    return group;
}

bool ContactsManager::addNewInterestGroupToDatabase(InterestGroup *interestGroup){
    
    quint32 groupID = interestGroup->getGroupID();

    if(interestGroupsHash.contains(groupID)){
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString queryString = QString("Insert  Into [interestgroups] ([GroupID]) Values(%1)").arg(groupID);
    
    if(!query.exec(queryString)){
        qCritical()<<"Can not insert new interest group data to database! Error:"<<query.lastError().text();
        return false;
    }
    
    interestGroupsHash.insert(groupID, interestGroup);
    //m_myself->addInterestGroup(groupID);
    
    return true;
    
}

bool ContactsManager::removeInterestGroupFromLocalDB(quint32 groupID){
    

    if(!interestGroupsHash.contains(groupID)){
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);
    
    
    QString queryString = QString("delete  from [interestgroups] where GroupID='%1' ").arg(groupID) ;
    
    if(!query.exec(queryString)){
        qCritical()<<"Can not delete interest group data from database! Error:"<<query.lastError().text();
        return false;
    }
    
    interestGroupsHash.remove(groupID);
    
    
    return true;
    
}


bool ContactsManager::saveInterestGroupInfoToDatabase(InterestGroup *interestGroup){

    if(!interestGroup){
        return false;
    }
        
    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString updateSQLStatement = interestGroup->getUpdateSQLStatement();
    if(updateSQLStatement.trimmed().isEmpty()){
        return false;
    }
    
    QString statement = QString("update interestgroups set %1 where GroupID='%2' ").arg(updateSQLStatement).arg(interestGroup->getGroupID());
    qDebug()<<"statement:"<<statement;
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save interest group info to database! Group ID:%1, %2 Error Type:%3 Error NO.:%4").arg(interestGroup->getGroupID()).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    interestGroup->clearUpdatedProperties();
    
    return true;
    
    
}


QList<InterestGroup *> ContactsManager::getInterestGroupsList(){
    return interestGroupsHash.values();
}

QList<quint32> ContactsManager::getInterestGroupIDsList(){
    return interestGroupsHash.keys();
}

//bool ContactsManager::isMemberOfInterestGroup(quint32 groupID){
//    InterestGroup *group = interestGroupsHash.value(groupID);
//    if(!group){return false;}
//    return group->getState();
//}

bool ContactsManager::saveInterestGroupMemberToDatabase(quint32 groupID, const QString &userID, quint32 memberRole){

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString statement = QString("insert into interestgroupmembers(MemberID, GroupID, MemberRole) values('%1', %2, %3)  ").arg(userID).arg(groupID).arg(memberRole);
    
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save interest group member info to database! Group ID:%1, User ID:%2! %3 Error Type:%4 Error NO.:%5").arg(groupID).arg(userID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    return true;

}


bool ContactsManager::saveInterestGroupMembersToDatabase(InterestGroup *interestGroup){

    if(!interestGroup){
        qCritical()<<"ERROR! Invalid InterestGroup!";
        return false;
    }
    
    //TODO:
    QHash<QString,InterestGroup::MemberRole> membersHash = interestGroup->getMembersHash();
    if(membersHash.isEmpty()){
        qCritical()<<"ERROR! Empty membersHash!";
        return false;
    }
    quint32 groupID = interestGroup->getGroupID();
    
    
    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString statement = QString("delete from interestgroupmembers where GroupID=%1 ").arg(groupID);
    
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not delete interest group members from database! Group ID:%1, %2 Error Type:%3 Error NO.:%4").arg(groupID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    QStringList members = membersHash.keys();
    
    foreach (QString userID, members) {
        
        if(!saveInterestGroupMemberToDatabase(groupID, userID, membersHash.value(userID))){
            return false;
        }
        
    }
    
    return true;
    
}

bool ContactsManager::fetchAllInterestGroupMembers(InterestGroup *interestGroup){

    if(!interestGroup){
        qCritical()<<"ERROR! Invalid InterestGroup!";
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    quint32 groupID = interestGroup->getGroupID();
    QString statement = QString("select MemberID, MemberRole from interestgroupmembers where GroupID=%1 ").arg(groupID);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query interest group members from database! Group ID:%1, %2 Error Type:%3 Error NO.:%4").arg(groupID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    QHash<QString, InterestGroup::MemberRole> membersHash;

    while (query.next()) {
        membersHash.insert(query.value(0).toString(), InterestGroup::MemberRole(query.value(1).toUInt()));
    }

    interestGroup->setMembersHash(membersHash);

    return true;

}

bool ContactsManager::saveContactChatMessageToDatabase(const QString &senderID, const QString &receiverID, const QString &message, const QString &time){

    if(message.isEmpty()){
        qWarning()<<"Empty Contact Chat Message!";
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }

    QString t = time;
    if(t.trimmed().isEmpty()){
        t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }

    QSqlQuery query(localUserDataDB);

    QString statement = QString("insert into contactchatmessages(SenderID, ReceiverID, Message, DeliveryTime) values('%1', '%2', '%3', '%4') ").arg(senderID).arg(receiverID).arg(message).arg(t);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save contact chat message to database! Sender ID:%1, Receiver ID:%2,  %3 Error Type:%4 Error NO.:%5").arg(senderID).arg(receiverID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    return true;


}

bool ContactsManager::saveInterestGroupChatMessageToDatabase(const QString &senderID, quint32 interestGroupID, const QString &message, const QString &time){
    qDebug()<<"--ContactsManager::saveInterestGroupChatMessageToDatabase(...) senderID:"<<senderID<<" interestGroupID:"<<interestGroupID<<" message:"<<message;

    if(message.isEmpty()){
        qWarning()<<"Empty Interest Group Chat Message!";
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }

    QString t = time;
    if(t.trimmed().isEmpty()){
        t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }

    QSqlQuery query(localUserDataDB);

    QString statement = QString("insert into interestgroupchatmessages(SenderID, GroupID, Message, DeliveryTime) values('%1', '%2', '%3', '%4') ").arg(senderID).arg(interestGroupID).arg(message).arg(t);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save interestgroup chat message to database! Sender ID:%1, GroupID ID:%2,  %3 Error Type:%4 Error NO.:%5").arg(senderID).arg(interestGroupID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    return true;


}

Contact * ContactsManager::createNewContact(const QString &contactID, int groupID, const QString &nickname, const QString &face){
    qDebug()<<"--ContactsManager::createNewContact(...) "<<" contactID:"<<contactID<<" groupID:"<<groupID;


    Q_ASSERT(!contactHash.contains(contactID));
    Q_ASSERT(contactID != m_myself->getUserID());

    if(contactID == m_myself->getUserID()){
        qCritical()<<"ERROR! Contact ID is myself!";
        return 0;
    }

    Contact *contact = 0;
//    if(contactHash.contains(contactID)){
//        contact = contactHash.value(contactID);
//        contact->setNickName(nickname);
//        contact->setFace(face);
//        contact->setContactGroupID(groupID);
//    }else{
        contact = new Contact(contactID, nickname, this);
        contact->setFace(face);
        contact->setContactGroupID(groupID);

        if(!slotAddNewContactToDatabase(contact)){
            //delete contact;
            //contact = 0;
        }

//    }

    return contact;

}

int ContactsManager::onlineContactGroupMembersCount(int contactGroupID){

    ContactGroupBase *group = m_myself->getContactGroup(contactGroupID);
    if(!group){return 0;}

    int onlineCount = 0;
    QStringList members = group->members();
    foreach (QString contactID, members) {
        Contact *contact = contactHash.value(contactID);
        if(!contact){continue;}
        IM::OnlineState state = contact->getOnlineState();
        if(state != IM::ONLINESTATE_OFFLINE && (state != IM::ONLINESTATE_INVISIBLE) ){
            onlineCount++;
        }
    }

    return onlineCount;

}

void ContactsManager::slotFetchAllContactsInfo2(ItemBoxWidget *expandListView){
    qDebug()<<"ContactsManager::slotFetchAllContactsInfo(...)";

    QString groupQueryString = QString("select * from contactgroups;");
    
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQueryModel *contactsModel = new QSqlQueryModel(this);
    
    model->setQuery(queryDatabase(groupQueryString, true));
    if(model->lastError().type() != QSqlError::NoError){
        qCritical()<<"Query Error!"<<model->lastError().text();
        delete model;
        delete contactsModel;
        return;
    }

    while (model->canFetchMore()) {
        model->fetchMore();
    }

    
    for (int i=0; i<model->rowCount(); i++) {
        quint32 groupID = QVariant(model->record(i).value("GroupID")).toUInt();
        QString groupName = QVariant(model->record(i).value("GroupName")).toString();

//        quint32 groupInfoVersion = QVariant(model->record(i).value("GroupInfoVersion")).toUInt();
//        quint32 memberListVersion = QVariant(model->record(i).value("MemberListVersion")).toUInt();

        ContactGroupBase *group = m_myself->addContactGroup(groupID, groupName);
        Q_ASSERT(group);
//        group->setGroupName(groupName);
//        group->setGroupInfoVersion(groupInfoVersion);
//        group->setGroupMemberListInfoVersion(memberListVersion);


        QList<Contact*> list;
        QString contactsQueryString = QString("select * from contacts_detailed_info where ContactGroupID=%1") .arg(groupID);
        
        contactsModel->setQuery(queryDatabase(contactsQueryString, true));
        if(contactsModel->lastError().type() != QSqlError::NoError){
            qCritical()<<"Query Error!"<<contactsModel->lastError().text();
            delete model;
            delete contactsModel;
            return;
        }

        while (contactsModel->canFetchMore()) {
            contactsModel->fetchMore();
        }
        
        for (int j=0; j<contactsModel->rowCount(); j++) {
            QString contactUID = QVariant(contactsModel->record(j).value("UserID")).toString();
            Contact *contact = getUser(contactUID);
            if(!contact){
                contact = new Contact(contactUID, this);
            }

            contact->setTrueName(QVariant(contactsModel->record(j).value("TrueName")).toString());
            contact->setNickName(QVariant(contactsModel->record(j).value("NickName")).toString());
            contact->setGender(IMUserBase::Gender(QVariant(contactsModel->record(j).value("Gender")).toUInt()));
            contact->setAge(IMUserBase::Gender(QVariant(contactsModel->record(j).value("Age")).toUInt()));
            contact->setFace(QVariant(contactsModel->record(j).value("Face")).toString());
            
            //            contact->setInterestGroupID(QVariant(contactsModel->record(j).value("InterestGroupID")).toUInt());
            //            contact->setSystemGroupID(QVariant(contactsModel->record(j).value("SystemGroupID")).toUInt());
            contact->setPersonalDetailInfoVersion(QVariant(contactsModel->record(j).value("PersonalInfoVersion")).toUInt());
            
            contact->setHomeAddress(QVariant(contactsModel->record(j).value("HomeAddress")).toString());
            contact->setHomePhoneNumber(QVariant(contactsModel->record(j).value("HomePhoneNumber")).toString());
            contact->setHomeZipCode(QVariant(contactsModel->record(j).value("HomeZipCode")).toString());
            contact->setPersonalHomepage(QVariant(contactsModel->record(j).value("PersonalHomepage")).toString());
            contact->setPersonalEmailAddress(QVariant(contactsModel->record(j).value("PersonalEmailAddress")).toString());
            
            contact->setLastLoginTime(QVariant(contactsModel->record(j).value("LastLoginTime")).toDateTime());
            contact->setLastLoginExternalHostAddress(QVariant(contactsModel->record(j).value("LastLoginHostAddress")).toString());
            contact->setLastLoginExternalHostPort(QVariant(contactsModel->record(j).value("LastLoginHostPort")).toUInt());
            
            //contact->setQuestionForSecurity(QVariant(contactsModel->record(j).value("QuestionForSecurity")).toString());
            //contact->setAnswerForSecurity(QVariant(contactsModel->record(j).value("AnswerForSecurity")).toString());
            contact->setCompanyName(QVariant(contactsModel->record(j).value("CompanyName")).toString());
            contact->setJobTitle(QVariant(contactsModel->record(j).value("JobTitle")).toString());
            contact->setBusinessAddress(QVariant(contactsModel->record(j).value("BusinessAddress")).toString());
            contact->setBusinessPhoneNumber(QVariant(contactsModel->record(j).value("BusinessPhoneNumber")).toString());
            contact->setBusinessZipCode(QVariant(contactsModel->record(j).value("BusinessZipCode")).toString());
            contact->setBusinessFaxNumber(QVariant(contactsModel->record(j).value("BusinessFaxNumber")).toString());
            contact->setBusinessHomepage(QVariant(contactsModel->record(j).value("BusinessHomepage")).toString());
            contact->setBusinessEmailAddress(QVariant(contactsModel->record(j).value("BusinessEmailAddress")).toString());
            contact->setRegistrationTime(QVariant(contactsModel->record(j).value("RegistrationTime")).toDateTime());
            contact->setLoginTimes(QVariant(contactsModel->record(j).value("LoginTimes")).toInt());
            contact->setRemarkName(QVariant(contactsModel->record(j).value("RemarkName")).toString());

            contact->setContactGroupID(groupID);
            contactHash.insert(contactUID, contact);
            list.append(contact);

            group->addMember(contactUID);

        }

        if(groupID == ContactGroupBase::Group_Friends_ID){
            group->setGroupName(ContactGroupBase::Group_Friends_Name);
        }else if(groupID == ContactGroupBase::Group_Strangers_ID){
            group->setGroupName(ContactGroupBase::Group_Strangers_Name);
        }else if(groupID == ContactGroupBase::Group_Blacklist_ID){
            group->setGroupName(ContactGroupBase::Group_Blacklist_Name);
        }

        group->clearUpdatedProperties();

        slotLoadContactGroupToUI(expandListView, groupID, groupName, list);

    }

//    expandListView->setCategoryHidden(QString::number(ContactGroupBase::Group_Strangers_ID), true);
//    expandListView->setCategoryHidden(QString::number(ContactGroupBase::Group_Blacklist_ID), true);

    if(!m_myself->isStrangersShown()){
        expandListView->setCategoryHidden(QString::number(ContactGroupBase::Group_Strangers_ID), true);
    }

    expandListView->setCategoryExpanded(QString::number(ContactGroupBase::Group_Friends_ID), true);
    expandListView->setCategoryExpanded(QString::number(ContactGroupBase::Group_Strangers_ID), false);
    expandListView->setCategoryExpanded(QString::number(ContactGroupBase::Group_Blacklist_ID), false);

    
    contactsModel->deleteLater();
    model->deleteLater();


}

void ContactsManager::slotFetchAllContactsInfoFromDB(){
    qDebug()<<"ContactsManager::slotFetchAllContactsInfoFromDB()";

    //Q_ASSERT(contactHash.isEmpty());
    foreach (Contact *contact, contactHash.values()) {
        delete contact;
        contact = 0;
    }
    contactHash.clear();


    QSqlQueryModel *contactsModel = new QSqlQueryModel(this);

    QString contactsQueryString = QString("select * from contacts_detailed_info;");

    contactsModel->setQuery(queryDatabase(contactsQueryString, true));
    if(contactsModel->lastError().type() != QSqlError::NoError){
        qCritical()<<"Query Error!"<<contactsModel->lastError().text();
        delete contactsModel;
        return;
    }

    while (contactsModel->canFetchMore()) {
        contactsModel->fetchMore();
    }

    for (int j=0; j<contactsModel->rowCount(); j++) {
        QString contactUID = QVariant(contactsModel->record(j).value("UserID")).toString();
        Contact *contact = getUser(contactUID);
        if(!contact){
            contact = new Contact(contactUID, this);
        }

        contact->setTrueName(QVariant(contactsModel->record(j).value("TrueName")).toString());
        contact->setNickName(QVariant(contactsModel->record(j).value("NickName")).toString());
        contact->setGender(IMUserBase::Gender(QVariant(contactsModel->record(j).value("Gender")).toUInt()));
        contact->setAge(IMUserBase::Gender(QVariant(contactsModel->record(j).value("Age")).toUInt()));
        contact->setFace(QVariant(contactsModel->record(j).value("Face")).toString());

        contact->setContactGroupID(IMUserBase::Gender(QVariant(contactsModel->record(j).value("ContactGroupID")).toInt()));

        //            contact->setInterestGroupID(QVariant(contactsModel->record(j).value("InterestGroupID")).toUInt());
        //            contact->setSystemGroupID(QVariant(contactsModel->record(j).value("SystemGroupID")).toUInt());
        contact->setPersonalDetailInfoVersion(QVariant(contactsModel->record(j).value("PersonalInfoVersion")).toUInt());

        contact->setHomeAddress(QVariant(contactsModel->record(j).value("HomeAddress")).toString());
        contact->setHomePhoneNumber(QVariant(contactsModel->record(j).value("HomePhoneNumber")).toString());
        contact->setHomeZipCode(QVariant(contactsModel->record(j).value("HomeZipCode")).toString());
        contact->setPersonalHomepage(QVariant(contactsModel->record(j).value("PersonalHomepage")).toString());
        contact->setPersonalEmailAddress(QVariant(contactsModel->record(j).value("PersonalEmailAddress")).toString());

        contact->setLastLoginTime(QVariant(contactsModel->record(j).value("LastLoginTime")).toDateTime());
        contact->setLastLoginExternalHostAddress(QVariant(contactsModel->record(j).value("LastLoginHostAddress")).toString());
        contact->setLastLoginExternalHostPort(QVariant(contactsModel->record(j).value("LastLoginHostPort")).toUInt());

        //contact->setQuestionForSecurity(QVariant(contactsModel->record(j).value("QuestionForSecurity")).toString());
        //contact->setAnswerForSecurity(QVariant(contactsModel->record(j).value("AnswerForSecurity")).toString());
        contact->setCompanyName(QVariant(contactsModel->record(j).value("CompanyName")).toString());
        contact->setJobTitle(QVariant(contactsModel->record(j).value("JobTitle")).toString());
        contact->setBusinessAddress(QVariant(contactsModel->record(j).value("BusinessAddress")).toString());
        contact->setBusinessPhoneNumber(QVariant(contactsModel->record(j).value("BusinessPhoneNumber")).toString());
        contact->setBusinessZipCode(QVariant(contactsModel->record(j).value("BusinessZipCode")).toString());
        contact->setBusinessFaxNumber(QVariant(contactsModel->record(j).value("BusinessFaxNumber")).toString());
        contact->setBusinessHomepage(QVariant(contactsModel->record(j).value("BusinessHomepage")).toString());
        contact->setBusinessEmailAddress(QVariant(contactsModel->record(j).value("BusinessEmailAddress")).toString());
        contact->setRegistrationTime(QVariant(contactsModel->record(j).value("RegistrationTime")).toDateTime());
        contact->setLoginTimes(QVariant(contactsModel->record(j).value("LoginTimes")).toInt());
        contact->setRemarkName(QVariant(contactsModel->record(j).value("RemarkName")).toString());

        contactHash.insert(contactUID, contact);


    }


    contactsModel->deleteLater();


}

void ContactsManager::slotFetchRecentChatsFromDB(QStringList *contacts, QList<quint32> *interestGroups){
    qDebug()<<"ContactsManager::slotFetchRecentChatsFromDB()";

    Q_ASSERT(contacts);
    Q_ASSERT(interestGroups);


    QDateTime time = ServerTime::instance()->time();
    if(time.isNull()){
        time = QDateTime::currentDateTime();
    }
    time = time.addDays(-7);
    QString timeString = time.toString("yyyy-MM-dd hh:mm:ss");


    QSqlQuery query(localUserDataDB);

    QString statement = QString("SELECT DISTINCT SenderID AS Contacts FROM contactchatmessages where SenderID <> '%1' and DeliveryTime > '%2' UNION SELECT DISTINCT ReceiverID FROM contactchatmessages where ReceiverID <> '%1' and DeliveryTime > '%2'; ").arg(m_myself->getUserID()).arg(timeString);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query recent contacts from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return ;
    }

    QStringList contactList;
    while (query.next()) {
        contactList.append(query.value(0).toString());
    }
    if(contacts){
        *contacts = contactList;
    }


    statement = QString("SELECT DISTINCT GroupID AS Groups FROM interestgroupchatmessages where DeliveryTime > '%1' ; ").arg(timeString);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query recent chat groups from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return ;
    }

    QList<quint32> groupList;
    while (query.next()) {
        groupList.append(query.value(0).toUInt());
    }

    if(interestGroups){
        *interestGroups = groupList;
    }



    return ;

}

void ContactsManager::slotAddNewContactGroupToUI(ItemBoxWidget *expandListView, int personalContactGroupID, const QString &groupName){
    qDebug()<<"--ContactsManager::slotAddNewContactGroupToUI(...)";

    //    Category *category = new Category();
    //    category->setID(QString::number(groupID));
    //    category->setName(groupName);
    //    expandListView->load(category);

    Category category;
    category.setID(QString::number(personalContactGroupID));
    category.setName(groupName);
//    expandListView->addCategory(category);

    addNewContactGroupToUI(expandListView, category);


}

void ContactsManager::slotDeleteContactGroupFromUI(ItemBoxWidget *expandListView, int contactGroupID){

    expandListView->removeCategory(QString::number(contactGroupID));
}

void ContactsManager::addContactToUI(ItemBoxWidget *expandListView, int groupID, const QString &contactID){
    qDebug()<<"--ContactsManager::addContactToUI(...)  groupID:"<<groupID<<" contactID:"<<contactID;

    Contact *contact = 0;
    if(!contactHash.contains(contactID)){
        return;
    }
    contact = contactHash.value(contactID);
    Q_ASSERT(contact);

    ObjectItem objectItem;
    objectItem.setID(contactID);

//    QString name = contact->getRemarkName();
//    if(name.isEmpty()){
//        name = contact->getNickName();
//    }
//    if(name.isEmpty()){
//        name = contact->getUserID();
//    }
    objectItem.setName(contact->displayName());

    objectItem.setIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));

    expandListView->addItem(QString::number(groupID), objectItem);


}

void ContactsManager::deleteContactFromUI(ItemBoxWidget *expandListView, quint32 groupID, const QString &contactID){

    expandListView->removeItem(QString::number(groupID), contactID);

}

void ContactsManager::moveContactToUI(ItemBoxWidget *expandListView, quint32 old_groupID, quint32 new_groupID, const QString &contactID){
    qDebug()<<"moveContactToUI(...)";

    if(!contactHash.contains(contactID)){
        return;
    }
    expandListView->moveItem(QString::number(old_groupID), QString::number(new_groupID), contactID);

    //expandListView->update();
    //expandListView->updateGeometry();

}

void ContactsManager::updateContactToUI(ItemBoxWidget *expandListView, int personalContactGroupID, const QString &contactID){


    Contact *contact = 0;
    if(!contactHash.contains(contactID)){
        return;
    }
    contact = contactHash.value(contactID);
    Q_ASSERT(contact);

    expandListView->updateObjectItemName(QString::number(personalContactGroupID), contactID, contact->displayName());
    expandListView->updateObjectItemIcon(QString::number(personalContactGroupID), contactID, ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));


}


void ContactsManager::renameContactGroupToUI(ItemBoxWidget *expandListView, quint32 groupID, const QString &new_groupName){

    //    ContactGroup *contactGroup = 0;
    //    int groupID = getGroup(group_name);
    //    if(contactGroupHash.contains(groupID)){
    //        contactGroup = contactGroupHash.value(groupID);
    //    }else{
    //        return;
    //    }
    //    contactGroup->setGroupName(new_groupName);

    expandListView->updateCategoryName(QString::number(groupID), new_groupName);


}


void ContactsManager::slotLoadContactGroupToUI(ItemBoxWidget *expandListView, int groupID, const QString groupName, QList<Contact*> contactList){
    qDebug()<<"--ContactsManager::slotLoadContactGroupToUI(...)  groupID:"<<groupID<<" groupName:"<<groupName;

    Category category;
    category.setID(QString::number(groupID));
    category.setName(groupName);

    foreach(Contact *contact, contactList)
    {
        //ObjectItem *objectItem = new ObjectItem();
        ObjectItem objectItem;
        objectItem.setID(contact->getUserID());

//        QString name = contact->getRemarkName();
//        if(name.isEmpty()){
//            name = contact->getNickName();
//        }
//        if(name.isEmpty()){
//            name = contact->getUserID();
//        }
        objectItem.setName(contact->displayName());

        objectItem.setIcon(ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState()));
        //objectItem.setIconMode(QIcon::Disabled);
        //objectItem.setCategoryID(groupName);

        category.addItem(objectItem);

    }



//    if(groupID == ContactGroupBase::Group_Friends_ID){
//        expandListView->addCategory(category, 0);
//    }else if(groupID == ContactGroupBase::Group_Strangers_ID || groupID == ContactGroupBase::Group_Blacklist_ID){
//        expandListView->addCategory(category, -1);
//    }else{
//        expandListView->addCategory(category, expandListView->categoryCount()-1);
//        //expandListView->addCategory(category, 1);
//    }

    //    expandListView->addCategory(category);


    addNewContactGroupToUI(expandListView, category);

//    expandListView->setCategoryExpanded(QString::number(groupID), false);



}

inline void ContactsManager::addNewContactGroupToUI(ItemBoxWidget *expandListView, Category category){

//    int groupID = category.id().toInt();
//    if(groupID == ContactGroupBase::Group_Friends_ID){
//        expandListView->addCategory(category, 0);
//    }else if(groupID == ContactGroupBase::Group_Strangers_ID || groupID == ContactGroupBase::Group_Blacklist_ID){
//        expandListView->addCategory(category, -1);
//    }else{
//        expandListView->addCategory(category, expandListView->categoryCount()-1);
//    }


    int groupID = category.id().toInt();
    if(groupID == ContactGroupBase::Group_Friends_ID){
        expandListView->addCategory(category, 0);
    }else if(groupID == ContactGroupBase::Group_Strangers_ID ){
        expandListView->addCategory(category, expandListView->categoryCount()-1);
    }else if(groupID == ContactGroupBase::Group_Blacklist_ID){
        expandListView->addCategory(category, -1);
    }else{
        expandListView->addCategory(category, 1);
    }



}
//bool ContactsManager::addContact(const QString &contactID, quint32 groupID){

//    Contact *contact = contactHash.value(contactID);
//    if(!contact){
//        return false;
//    }

//    m_imUser->addOrDeleteContact(contactID, groupID, add);

//    contact->setContactGroupID(groupID);

//    return true;

//}

bool ContactsManager::deleteContact(const QString &contactID, bool addToBlacklist){

    Contact *contact = contactHash.value(contactID);
    if(!contact){
        return false;
    }

    int oldGroupID = contact->getContactGroupID();
    if(addToBlacklist){
        contact->setContactGroupID(ContactGroupBase::Group_Blacklist_ID);
        m_myself->moveContactToAnotherGroup(contactID, oldGroupID, ContactGroupBase::Group_Blacklist_ID);
    }else{
        contact->setContactGroupID(ContactGroupBase::Group_Strangers_ID);
        //if(oldGroupID == ContactGroupBase::Group_Strangers_ID){
        //    m_imUser->strangersGroup()->deleteMember(contactID);
        //}else{
            m_myself->moveContactToAnotherGroup(contactID, oldGroupID, ContactGroupBase::Group_Strangers_ID);
        //}
    }

    return true;
}

bool ContactsManager::moveContact(const QString &contactID, quint32 oldGroupID, quint32 newGroupID){

    Contact *contact = contactHash.value(contactID);
    if(!contact){
        return false;
    }
    contact->setContactGroupID(newGroupID);

    m_myself->moveContactToAnotherGroup(contactID, oldGroupID, newGroupID);

    return true;

}


void ContactsManager::slotChangeContactOnlineState(const QString &contactID, quint8 onlineStateCode, const QString &peerAddress, quint16 peerPort, const QString &greetingInfo){

    qDebug()<<"----ContactsManager::slotChangeContactOnlineState(...) "<<" contactID:"<<contactID;

    Contact *contact = contactHash.value(contactID);
    contact->setOnlineState(IM::OnlineState(onlineStateCode));

}



//添加新的联系人
//Add new contact
bool ContactsManager::slotAddNewContactToDatabase(Contact *contact){
    qDebug()<<"----ContactsManager::slotAddNewContactToDatabase(...)  Contact ID:"<<contact->getUserID();
    
    if(contactHash.contains(contact->getUserID())){
        qCritical()<<"Error! Contact already exists!";
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);
    
    QString contactUID = contact->getUserID();
    int contactsGroupID = contact->getContactGroupID();
    QString contactNickName = contact->getNickName();
    QString face = contact->getFace();
    
    QString queryString = QString("Insert  Into [contacts_detailed_info] ([UserID],[NickName],[Face],[ContactGroupID]) Values('%1', '%2', '%3', %4)")
            .arg(contactUID).arg(contactNickName).arg(face).arg(contactsGroupID) ;
    //QSqlQuery query = queryDatabase(queryString, true);
    
    if(!query.exec(queryString)){
        qCritical()<<"ERROR! Can not insert new contact data to database! "<<query.lastError().text();
        return false;
    }

    contactHash.insert(contact->getUserID(), contact);

    m_myself->addNewContact(contactUID, contactsGroupID);

    return true;
    
}

//删除联系人
//Delete contact
bool ContactsManager::slotdeleteContactFromDatabase(Contact *contact){
    qDebug()<<"----ContactsManager::slotdeleteContactFromDatabase(...)";

    QString contactID = contact->getUserID();

    if(!contactHash.contains(contactID)){
        return false;
    }
    contactHash.remove(contactID);


    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString contactUID = contact->getUserID();

    QString queryString = QString("Delete  From [contacts_detailed_info] Where [UserID] ='%1' ").arg(contactUID);
    if(!query.exec(queryString)){
        qCritical()<<"Can not delete contact data from database! Error:"<<query.lastError().text();
        return false;
    }

    m_myself->deleteContact(contactID);

    delete contact;
    contact = 0;


    return true;

}

//QString ContactsManager::getPersonalContactGroupName(int personalContactGroupID) {

//    if(!localUserDataDB.isValid()){
//        if(!openDatabase()){
//            return "";
//        }
//    }
//    QSqlQuery query(localUserDataDB);
    
//    QString queryString = QString("Select [GroupName]  From [contactgroups] where [GroupID] = %1 ").arg(personalContactGroupID);

//    //QSqlQuery query = queryDatabase(queryString, true);
//    if(!query.exec(queryString)){
//        qCritical()<<QString("Can not get contact group name! Group ID:'%1', Error:%2").arg(personalContactGroupID).arg(query.lastError().text());

//        return "";
//    }
    
//    query.first();
//    if(!query.isValid()){
//        return "";
//    }
    
    
//    return query.value(0).toString();
    
//}

bool ContactsManager::slotAddNewContactGroupToDatabase(quint32 groupID, const QString &groupName){
    qDebug()<<"--slotAddNewContactGroupToDatabase(...) groupID:"<<groupID<<" groupName"<<groupName;

    Q_ASSERT(groupID>0);
    if(groupName.trimmed().isEmpty()){
        return false;
    }


    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);
    
    QString queryString = QString("Insert  Into [contactgroups] ([GroupID], [GroupName]) Values(%1, '%2')").arg(groupID).arg(groupName);
//    if(!groupID){
//        queryString = QString("Insert  Into [contactgroups] ([GroupID], [GroupName]) Values(NULL, '%1')").arg(groupName);
//    }

    if(!query.exec(queryString)){
        qCritical()<<QString("Can not add new contact group! Group Name:'%1', Error:%2").arg(groupName).arg(query.lastError().text());

        return false;
    }
    
//    if(!groupID){

//        queryString = QString("Select [GroupID]  From [contactgroups] where [GroupName] = '%1' ").arg(groupName);
//        //query = queryDatabase(queryString, true);
//        if(!query.exec(queryString)){
//            qCritical()<<QString("Can not query contact group info! Group Name:'%1', Error:%2").arg(groupName).arg(query.lastError().text());

//            return 0;
//        }

//        query.first();
//        if(!query.isValid()){
//            return 0;
//        }

//        return query.value(0).toInt();

////        ContactGroupBase *group = m_imUser->addContactGroup(groupID);
////        group->setGroupName(groupName);

//    }
    
//    return groupID;

    return true;
    
} 

bool ContactsManager::renameContactGroupToDatabase(quint32 groupID, const QString &new_groupName){

    if(!m_myself->hasContactGroup(groupID)){
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString queryString = QString("Update [contactgroups] Set [GroupName] = '%1' where [GroupID] = %2 ").arg(new_groupName).arg(groupID);

    if(!query.exec(queryString)){
        qCritical()<<QString("Can not rename contact group! Group ID:'%1', Error:%2").arg(groupID).arg(query.lastError().text());

        return false;
    }


//    ContactGroupBase *contactGroup = m_imUser->getContactGroup(groupID);
//    contactGroup->setGroupName(new_groupName);

    return true;

}

bool ContactsManager::deleteContactGroupFromDatabase(int groupID){
    qDebug()<<"--ContactsManager::deleteContactGroupFromDatabase(..) groupID:"<<groupID;

    if(!m_myself->hasContactGroup(groupID)){
        qCritical()<<"ERROR! Contact group does not exist!"<<" Group ID:"<<groupID;
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString queryString = QString("Delete From [contactgroups] where [GroupID] = %1 ").arg(groupID);

    if(!query.exec(queryString)){
        qCritical()<<QString("Can not delete contact group! Group Name:'%1', Error:%2").arg(groupID).arg(query.lastError().text());

        return false;
    }

    return true;

}

bool ContactsManager::resetAllContactGroupInDatabase(){
    qDebug()<<"--ContactsManager::resetAllContactGroupInDatabase() ";


    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    QString queryString = QString("Delete From [contactgroups] where [GroupID] <> %1 ; ").arg(ContactGroupBase::Group_Strangers_ID);

    if(!query.exec(queryString)){
        qCritical()<<QString("ERROR! Can not delete contact groups! %1").arg(query.lastError().text());
        return false;
    }

    queryString = QString("Insert  Into [contactgroups] ([GroupID], [GroupName]) Values(%1, '%2') ; ").arg(ContactGroupBase::Group_Blacklist_ID).arg(ContactGroupBase::Group_Blacklist_Name);
    if(!query.exec(queryString)){
        qCritical()<<QString("ERROR! Can not create contact group '%1' ! %2").arg(ContactGroupBase::Group_Blacklist_Name).arg(query.lastError().text());
        return false;
    }

    queryString = QString("Insert  Into [contactgroups] ([GroupID], [GroupName]) Values(%1, '%2') ; ").arg(ContactGroupBase::Group_Friends_ID).arg(ContactGroupBase::Group_Friends_Name);
    if(!query.exec(queryString)){
        qCritical()<<QString("ERROR! Can not create contact group '%1' ! %2").arg(ContactGroupBase::Group_Friends_Name).arg(query.lastError().text());
        return false;
    }

    return true;

}


bool ContactsManager::getMyInfoFormLocalDatabase(){
    qDebug()<<"--getMyInfoFormLocalDatabase()";


    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    //IMUser *m_imUser = IMUser::instance();
    QSqlRecord record;
    QString statement = QString("select * from my_detailed_info where UserID='%2' ").arg(m_myself->getUserID());

    
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    
    //    query = queryDatabase(statement, false);
    //    if((query.lastError().type() != QSqlError::NoError)){
    //        qCritical()<<QString("Can not query data from local database! Error: %1").arg(query.lastError().text());
    //        return false;
    //    }
    query.first();
    if(!query.isValid()){
        qCritical()<<QString("Can not query user info! Invalid record! User ID:%1").arg(m_myself->getUserID());
        
        statement = QString("Insert Into my_detailed_info(UserID) Values('%1')").arg(m_myself->getUserID());
        if(!query.exec(statement)){
            qCritical()<<"Can not insert data! "<<query.lastError().text();
        }
        
        return false;
    }
    

    record = query.record();
    
//    m_imUser->setTrueName(QVariant(query.value(record.indexOf("TrueName"))).toString());
//    m_imUser->setNickName(QVariant(query.value(record.indexOf("NickName"))).toString());
//    m_imUser->setGender(IMUserBase::Gender(QVariant(query.value(record.indexOf("Gender"))).toUInt()));
//    m_imUser->setAge(QVariant(query.value(record.indexOf("Age"))).toUInt());
//    m_imUser->setFace(QVariant(query.value(record.indexOf("Face"))).toString());
//    m_imUser->setPersonalContactGroupsVersion(QVariant(query.value(record.indexOf("PersonalContactGroupsInfoVersion"))).toUInt());
//    m_imUser->setInterestGroupInfoVersion(QVariant(query.value(record.indexOf("InterestGroupsInfoVersion"))).toUInt());
//    m_imUser->setBlacklistInfoVersion(QVariant(query.value(record.indexOf("BlacklistInfoVersion"))).toUInt());
//    m_imUser->setPersonalSummaryInfoVersion(QVariant(query.value(record.indexOf("PersonalSummaryInfoVersion"))).toUInt());
//    m_imUser->setPersonalDetailInfoVersion(QVariant(query.value(record.indexOf("PersonalDetailInfoVersion"))).toUInt());
    
//    m_imUser->setFriendshipApply(QVariant(query.value(record.indexOf("FriendshipApply"))).toUInt());
//    m_imUser->setShortTalk(QVariant(query.value(record.indexOf("ShortTalk"))).toUInt());
//    m_imUser->setUserRole(QVariant(query.value(record.indexOf("Role"))).toUInt());
//    m_imUser->setDescription(QVariant(query.value(record.indexOf("Description"))).toUInt());
//    m_imUser->setAccountState(QVariant(query.value(record.indexOf("AccountState"))).toUInt());

//    m_imUser->setHomeAddress(QVariant(query.value(record.indexOf("HomeAddress"))).toString());
//    m_imUser->setHomePhoneNumber(QVariant(query.value(record.indexOf("HomePhoneNumber"))).toString());
//    m_imUser->setHomeZipCode(QVariant(query.value(record.indexOf("HomeZipCode"))).toString());
//    m_imUser->setPersonalHomepage(QVariant(query.value(record.indexOf("PersonalHomepage"))).toString());
//    m_imUser->setPersonalEmailAddress(QVariant(query.value(record.indexOf("PersonalEmailAddress"))).toString());
//    m_imUser->setCompanyName(QVariant(query.value(record.indexOf("CompanyName"))).toString());
//    m_imUser->setJobTitle(QVariant(query.value(record.indexOf("JobTitle"))).toString());

//    m_imUser->setBusinessAddress(QVariant(query.value(record.indexOf("BusinessAddress"))).toString());
//    m_imUser->setBusinessPhoneNumber(QVariant(query.value(record.indexOf("BusinessPhoneNumber"))).toString());
//    m_imUser->setBusinessZipCode(QVariant(query.value(record.indexOf("BusinessZipCode"))).toString());
//    m_imUser->setBusinessFaxNumber(QVariant(query.value(record.indexOf("BusinessFaxNumber"))).toString());
//    m_imUser->setBusinessHomepage(QVariant(query.value(record.indexOf("BusinessHomepage"))).toString());
//    m_imUser->setBusinessEmailAddress(QVariant(query.value(record.indexOf("BusinessEmailAddress"))).toString());

////    m_imUser->setLastLoginTime(QVariant(query.value(record.indexOf("LastLoginTime"))).toDateTime());
////    m_imUser->setLastLoginExternalHostAddress(QVariant(query.value(record.indexOf("LastLoginHostAddress"))).toString());
////    m_imUser->setLastLoginExternalHostPort(QVariant(query.value(record.indexOf("LastLoginHostPort"))).toUInt());

////    m_imUser->setRegistrationTime(QVariant(query.value(record.indexOf("RegistrationTime"))).toDateTime());
////    m_imUser->setLoginTimes(QVariant(query.value(record.indexOf("LoginTimes"))).toInt());

////    info->setQuestionForSecurity(QVariant(query.value(record.indexOf("QuestionForSecurity"))).toString());
////    info->setAnswerForSecurity(QVariant(query.value(record.indexOf("AnswerForSecurity"))).toString());

///////////////////////////////

//    m_imUser->setSysID(QVariant(query.value(record.indexOf(m_imUser->databaseColumnName(IM::PI_SysID)))).toUInt());
    m_myself->setTrueName(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_TrueName)))).toString());
    m_myself->setNickName(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_NickName)))).toString());
    m_myself->setGender(IMUserBase::Gender(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_Gender)))).toUInt()));
    m_myself->setAge(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_Age)))).toInt());
    m_myself->setFace(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_Face)))).toString());

    m_myself->setPersonalContactGroupsVersion(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalContactGroupsInfoVersion)))).toInt());
    m_myself->setInterestGroupInfoVersion(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_InterestGroupsInfoVersion)))).toUInt());
    m_myself->setPersonalMessageInfoVersion(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalMessageInfoVersion)))).toInt());

    m_myself->setPersonalSummaryInfoVersion(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalSummaryInfoVersion)))).toInt());
    m_myself->setPersonalDetailInfoVersion(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalDetailInfoVersion)))).toInt());
    m_myself->setFriendshipApply(IMUserBase::FriendshipApply(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_FriendshipApply)))).toUInt()));
    m_myself->setShortTalk(IMUserBase::ShortTalk(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_ShortTalk)))).toUInt()));
    m_myself->setUserRole(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_Role)))).toInt());
    m_myself->setDescription(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_Description)))).toString());
    m_myself->setAccountState(IMUserBase::AccountState(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_AccountState)))).toUInt()));
    m_myself->setPersonalMessage(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalMessage)))).toString());

    m_myself->setHomeAddress(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_HomeAddress)))).toString());
    m_myself->setHomePhoneNumber(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_HomePhoneNumber)))).toString());
    m_myself->setHomeZipCode(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_HomeZipCode)))).toString());
    m_myself->setPersonalHomepage(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalHomepage)))).toString());
    m_myself->setPersonalEmailAddress(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_PersonalEmailAddress)))).toString());

    m_myself->setCompanyName(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_CompanyName)))).toString());
    m_myself->setJobTitle(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_JobTitle)))).toString());
    m_myself->setBusinessAddress(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_BusinessAddress)))).toString());
    m_myself->setBusinessPhoneNumber(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_BusinessPhoneNumber)))).toString());
    m_myself->setBusinessZipCode(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_BusinessZipCode)))).toString());
    m_myself->setBusinessFaxNumber(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_BusinessFaxNumber)))).toString());
    m_myself->setBusinessHomepage(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_BusinessHomepage)))).toString());
    m_myself->setBusinessEmailAddress(QVariant(query.value(record.indexOf(m_myself->databaseColumnName(IM::PI_BusinessEmailAddress)))).toString());



///////////////////////////////


    m_myself->clearUpdatedProperties();

    return true;
    


}

bool ContactsManager::saveMyInfoToDatabase(){

    qDebug()<<"--ContactsManager::saveMyInfoToDatabase()";
    
    //IMUser *info = IMUser::instance();
    IMUser *info = m_myself;
    QStringList updateSQLStatements;
    QString summaryInfoStatement = info->getUpdateSQLStatement(true);
    QString detailInfoStatement = info->getUpdateSQLStatement(false);
    if(!summaryInfoStatement.trimmed().isEmpty()){
        updateSQLStatements.append(summaryInfoStatement);
    }
    if(!detailInfoStatement.trimmed().isEmpty()){
        updateSQLStatements.append(detailInfoStatement);
    }
    if(updateSQLStatements.isEmpty()){
        return false;
    }
    
    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);
    QString statement = QString("update my_detailed_info set %1 where UserID='%2' ").arg(updateSQLStatements.join(",")).arg(info->getUserID());
    qDebug()<<"------statement:"<<statement;
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save user info to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    info->clearUpdatedProperties();
    
    return true;
    
}

bool ContactsManager::setContactInfoString(const QString &contactID, const QString &contactInfoString, bool summaryInfo){

    if(contactHash.contains(contactID)){
        contactHash.value(contactID)->setPersonalInfoString(contactInfoString, summaryInfo);
        return true;
    }
    
    return false;
    
}

bool ContactsManager::getContactInfoFormLocalDatabase(const QString &contactID){
    qWarning()<<"--getContactInfoFormLocalDatabase()";
    
    
    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }

    QSqlQuery query(localUserDataDB);
    QSqlRecord record;
    QString statement = QString("select * from contacts_detailed_info where UserID='%2' ").arg(contactID);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query contact's' info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    Contact *contact = 0;
    if(contactHash.contains(contactID)){
        contact = contactHash.value(contactID);
    }else{
        contact = new Contact(contactID, this);
        contactHash.insert(contactID, contact);
    }

    //    query = queryDatabase(statement, false);
    //    if((query.lastError().type() != QSqlError::NoError)){
    //        qCritical()<<QString("Can not query data from local database! Error: %1").arg(query.lastError().text());
    //        return false;
    //    }
    query.first();
    if(!query.isValid()){
        qCritical()<<QString("Can not query contact's' info! Invalid record! Contact ID:%1").arg(contactID);
        
        return false;
    }
    

    record = query.record();
    

    contact->setTrueName(QVariant(query.value(record.indexOf("TrueName"))).toString());
    contact->setNickName(QVariant(query.value(record.indexOf("NickName"))).toString());
    contact->setGender(IMUserBase::Gender(QVariant(query.value(record.indexOf("Gender"))).toUInt()));
    contact->setAge(QVariant(query.value(record.indexOf("Age"))).toUInt());
    contact->setFace(QVariant(query.value(record.indexOf("Face"))).toString());
    
    //    contact->setInterestGroupID(QVariant(query.value(record.indexOf("InterestGroupID"))).toUInt());
    //    contact->setSystemGroupID(QVariant(query.value(record.indexOf("SystemGroupID"))).toUInt());
    contact->setContactGroupID(QVariant(query.value(record.indexOf("ContactGroupID"))).toUInt());
    contact->setPersonalDetailInfoVersion(QVariant(query.value(record.indexOf("PersonalInfoVersion"))).toUInt());
    
    //    info->setSystemGroupInfoVersion(QVariant(query.value(record.indexOf("SystemGroupsInfoVersion"))).toUInt());
    //    info->setInterestGroupInfoVersion(QVariant(query.value(record.indexOf("PersonalInterestGroupsInfoVersion"))).toUInt());
    //    info->setPersonalContactGroupsVersion(QVariant(query.value(record.indexOf("PersonalContactGroupsInfoVersion"))).toUInt());
    
    contact->setHomeAddress(QVariant(query.value(record.indexOf("HomeAddress"))).toString());
    contact->setHomePhoneNumber(QVariant(query.value(record.indexOf("HomePhoneNumber"))).toString());
    contact->setHomeZipCode(QVariant(query.value(record.indexOf("HomeZipCode"))).toString());
    contact->setPersonalHomepage(QVariant(query.value(record.indexOf("PersonalHomepage"))).toString());
    contact->setPersonalEmailAddress(QVariant(query.value(record.indexOf("PersonalEmailAddress"))).toString());

    contact->setLastLoginTime(QVariant(query.value(record.indexOf("LastLoginTime"))).toDateTime());
    contact->setLastLoginExternalHostAddress(QVariant(query.value(record.indexOf("LastLoginHostAddress"))).toString());
    contact->setLastLoginExternalHostPort(QVariant(query.value(record.indexOf("LastLoginHostPort"))).toUInt());

    //info->setQuestionForSecurity(QVariant(query.value(record.indexOf("QuestionForSecurity"))).toString());
    //info->setAnswerForSecurity(QVariant(query.value(record.indexOf("AnswerForSecurity"))).toString());
    
    contact->setCompanyName(QVariant(query.value(record.indexOf("CompanyName"))).toString());
    contact->setJobTitle(QVariant(query.value(record.indexOf("JobTitle"))).toString());

    contact->setBusinessAddress(QVariant(query.value(record.indexOf("BusinessAddress"))).toString());
    contact->setBusinessPhoneNumber(QVariant(query.value(record.indexOf("BusinessPhoneNumber"))).toString());
    contact->setBusinessZipCode(QVariant(query.value(record.indexOf("BusinessZipCode"))).toString());
    contact->setBusinessFaxNumber(QVariant(query.value(record.indexOf("BusinessFaxNumber"))).toString());
    contact->setBusinessHomepage(QVariant(query.value(record.indexOf("BusinessHomepage"))).toString());
    contact->setBusinessEmailAddress(QVariant(query.value(record.indexOf("BusinessEmailAddress"))).toString());

    contact->setRegistrationTime(QVariant(query.value(record.indexOf("RegistrationTime"))).toDateTime());
    contact->setLoginTimes(QVariant(query.value(record.indexOf("LoginTimes"))).toInt());

    contact->setRemarkName(QVariant(query.value(record.indexOf("RemarkName"))).toString());


    contact->clearUpdatedProperties();

    return true;
    


}

bool ContactsManager::saveContactInfoToDatabase(const QString &contactID){

    qDebug()<<"--ContactsManager::saveContactInfoToDatabase(...)";
    

    Contact *contact = 0;
    if(contactHash.contains(contactID)){
        contact = contactHash.value(contactID);
    }else{
        qCritical()<<"Error! Contact '"<<contactID<<"' does not exist!";
        return false;
    }

    QStringList updateSQLStatements;
    QString summaryInfoStatement = contact->getUpdateSQLStatement(true);
    QString detailInfoStatement = contact->getUpdateSQLStatement(false);
    if(!summaryInfoStatement.trimmed().isEmpty()){
        updateSQLStatements.append(summaryInfoStatement);
    }
    if(!detailInfoStatement.trimmed().isEmpty()){
        updateSQLStatements.append(detailInfoStatement);
    }
    if(updateSQLStatements.isEmpty()){
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);


    
    QString statement = QString("update contacts_detailed_info set %1 where UserID='%2' ").arg(updateSQLStatements.join(",")).arg(contactID);
    qDebug()<<"statement:"<<statement;
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save contact info to database! Contact ID:%1, %2 Error Type:%3 Error NO.:%4").arg(contactID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }
    
    contact->clearUpdatedProperties();
    
    return true;
    
}

bool ContactsManager::getContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, const QString &contactID, QStringList *messages, bool *canFetchMore){

    if(!messages || !canFetchMore){
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    int rowCount = HISTORY_MESSAGES_PAGE_SIZE+1;

    QString statement = QString("SELECT SenderID, ReceiverID, Message, DeliveryTime From contactchatmessages where (SenderID = '%1' or ReceiverID = '%1') and Message like '%%2%' and  DeliveryTime between '%3' and '%4' order by DeliveryTime %5 LIMIT %6 ").arg(contactID).arg(content).arg(startTime).arg(endTime).arg(requestBackword?"desc":"asc").arg(rowCount);
    qDebug()<<"statement:"<<statement;
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query contact chat message from database! Contact ID:%1, %2 Error Type:%3 Error NO.:%4").arg(contactID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    QString myID = m_myself->getUserID();
    QString myDisplayName = m_myself->getNickName();

    while (query.next()) {
        QStringList infoList;

        QString senderID = query.value(0).toString();
        QString displayName;
        if(senderID == myID){
            displayName = myDisplayName;
        }else{
            Contact *contact = contactHash.value(senderID);
            Q_ASSERT(contact);
            if(!contact){
                qCritical()<<"ERROR! Contact does not exist!";
                continue;
            }
            displayName = contact->displayName();
        }

        infoList.append(senderID);
        infoList.append(displayName);
        infoList.append(query.value(2).toString());
        infoList.append(query.value(3).toString());

        if(requestBackword){
            messages->prepend(infoList.join(QString(PACKET_DATA_SEPARTOR)));
        }else{
            messages->append(infoList.join(QString(PACKET_DATA_SEPARTOR)));
        }
    }

    if(messages->size() == rowCount){
        if(requestBackword){
            messages->removeFirst();
        }else{
            messages->removeLast();
        }

        *canFetchMore = true;
    }else{
        *canFetchMore = false;
    }

    return true;
}

bool ContactsManager::getGrouptHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, quint32 groupID, QStringList *messages, bool *canFetchMore){

    if(!messages || !canFetchMore){
        return false;
    }

    if(!localUserDataDB.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(localUserDataDB);

    int rowCount = HISTORY_MESSAGES_PAGE_SIZE + 1;

    QString statement = QString("SELECT SenderID, GroupID, Message, DeliveryTime From interestgroupchatmessages where GroupID = %1 and Message like '%%2%' and DeliveryTime between '%3' and '%4' order by DeliveryTime %5 LIMIT %6 ").arg(groupID).arg(content).arg(startTime).arg(endTime).arg(requestBackword?"desc":"asc").arg(rowCount);
    qDebug()<<"statement:"<<statement;
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query group chat message from database! Group ID:%1, %2 Error Type:%3 Error NO.:%4").arg(groupID).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    QString myID = m_myself->getUserID();
    QString myDisplayName = m_myself->getNickName();

    while (query.next()) {
        QStringList infoList;

        QString senderID = query.value(0).toString();
        QString displayName;
        if(senderID == myID){
            displayName = myDisplayName;
        }else{
            Contact *contact = contactHash.value(senderID);
            Q_ASSERT(contact);
            if(!contact){
                qCritical()<<"ERROR! Contact does not exist!";
                continue;
            }
            displayName = contact->displayName();
        }

        infoList.append(senderID);
        infoList.append(displayName);
        infoList.append(query.value(2).toString());
        infoList.append(query.value(3).toString());

        if(requestBackword){
            messages->prepend(infoList.join(QString(PACKET_DATA_SEPARTOR)));
        }else{
            messages->append(infoList.join(QString(PACKET_DATA_SEPARTOR)));
        }
    }

    if(messages->size() == rowCount){
        if(requestBackword){
            messages->removeFirst();
        }else{
            messages->removeLast();
        }

        *canFetchMore = true;
    }else{
        *canFetchMore = false;
    }

    return true;

}




//bool ContactsManager::saveContactGroupsInfoToDatabase(){
    
//    foreach (Contact *contact, contactHash.values()) {
//        contact->setContactGroupID(0);
//    }
    
    
//    QHash<QString/*Group Name*/, QStringList/*Group Members' ID*/> personalContactGroupsHash = m_imUser->getPersonalContactGroupsHash();

    
//    QStringList groups = personalContactGroupsHash.keys();
//    foreach (QString groupName, groups) {
//        int groupID = getPersonalContactGroupID(groupName);
//        if(!groupID){
//            groupID = slotAddNewContactGroupToDatabase(groupName);
//        }
//        QStringList members = personalContactGroupsHash.value(groupName);
//        foreach (QString contactID, members) {
//            if(contactID.trimmed().isEmpty()){continue;}
//            Contact *contact = 0;
//            if(contactHash.contains(contactID)){
//                contact = contactHash.value(contactID);
//            }else{
//                contact = new Contact(contactID, 0);
//                contactHash.insert(contactID, contact);
//                slotAddNewContactToDatabase(contact);
//            }
//            contact->setContactGroupID(groupID);
//            saveContactInfoToDatabase(contactID);
            
//        }
        
        
//    }
    
//    //TODO:更新联系人信息
    
//    return true;
    
    
//}


bool ContactsManager::openDatabase(bool reopen){
    qDebug()<<"--ContactsManager::openDatabase(...)";

    userPrivateDataFilePath = Settings::instance()->getCurrentUserPrivateDataFilePath();

    //Check Local Database
    bool needInitUserDB = false;
    if(!QFile(userPrivateDataFilePath).exists()){
        needInitUserDB = true;
    }



    if(reopen){
        //        if(query){
        //            query->clear();
        //            delete query;
        //            query = 0;
        //        }
        DatabaseUtility::closeDBConnection(LOCAL_USERDATA_DB_CONNECTION_NAME);
    }


    //QSqlDatabase db = QSqlDatabase::database(LOCAL_USERDATA_DB_CONNECTION_NAME);
    localUserDataDB = QSqlDatabase::database(LOCAL_USERDATA_DB_CONNECTION_NAME);
    if(!localUserDataDB.isValid()){
        QSqlError err;
        DatabaseUtility databaseUtility;
        userPrivateDataFilePath = Settings::instance()->getCurrentUserPrivateDataFilePath();
        err = databaseUtility.openDatabase(LOCAL_USERDATA_DB_CONNECTION_NAME,
                                           LOCAL_USERDATA_DB_DRIVER,
                                           "",
                                           0,
                                           "",
                                           "",
                                           userPrivateDataFilePath,
                                           HEHUI::SQLITE);
        if (err.type() != QSqlError::NoError) {
            qCritical() << QString("An error occurred when opening the database on '%1'! %2").arg(REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST).arg(err.text());
            return false;
        }

    }

    localUserDataDB = QSqlDatabase::database(LOCAL_USERDATA_DB_CONNECTION_NAME);
    if(!localUserDataDB.isOpen()){
        qCritical()<<QString("Database is not open! %1").arg(localUserDataDB.lastError().text());
        return false;
    }

    //    if(!query){
    //        query = new QSqlQuery(localUserDataDB);
    //    }

    if(needInitUserDB){
        if(!initLocalDatabase()){
            return false;
        }
    }

    return true;



}

bool ContactsManager::initLocalDatabase(QString *errorMessage){
    //qWarning()<<"--ContactsManager::initLocalDatabase(...)";

    if(!localUserDataDB.isValid() || !localUserDataDB.isOpen()){
        if(errorMessage){
            *errorMessage = tr("Database Invalid Or Not Open!");
        }
        return false;
    }

    QSqlQuery query;
    QSqlError error = DatabaseUtility::excuteSQLScriptFromFile(localUserDataDB, ":/text/resources/text/userdata.sql", "UTF-8", &query, true);
    if(error.type() != QSqlError::NoError){
        QString msg = error.text();
        if(errorMessage){
            *errorMessage = msg;
        }
        return false;
    }


    QString statement = QString("insert into my_detailed_info(UserID) values('%1')").arg(m_myself->getUserID());
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not initialize user database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        if(errorMessage){
            *errorMessage = msg;
        }
        return false;
    }

    return true;

}






/*

void ContactsManager::slotUpdateContactsInfo2(){



 QSqlQuery query;
 QSqlRecord record;


 //Local
 QString queryString = QString("select ContactsListVersion from contacts where UID = '%1'") .arg(User::instance()->getUserID());

 int localContactsListVersion = -1;

 query = queryDatabase(queryString, true);
 if(query.lastError().type() == QSqlError::NoError && query.isValid()){
  query.first();
  localContactsListVersion = query.value(0).toInt();
  qDebug()<<"Local Contacts List Version:"<<localContactsListVersion;
 }else{
  //QMessageBox::critical(this, QObject::tr("Fatal Error"), tr("Can not query data from local database! <br> %1").arg(model.lastError().text()));
  qCritical()<<QString("Can not query data from local database!");
  qCritical()<<QString("Error: %1").arg(query.lastError().text());
  //statusBar()->showMessage(tr("Error! Can not query data from local database!"));
 }


 if(User::instance()->getRemoteServerContactsListVersion() != localContactsListVersion){
  qDebug()<<"Remote Server Contacts List Version:"<<User::instance()->getRemoteServerContactsListVersion();

  if(User::instance()->getRemoteServerContactsListVersion() > localContactsListVersion){
   queryString = QString("update contacts set ContactsList = '%1'  where UID = '%2'") .arg(User::instance()->getContactsList(), User::instance()->getUserID());
   query = queryDatabase(queryString, true);
   qDebug()<<"Update Local Contacts List Version";
  }else{
   queryString = QString("select ContactsList from contacts where UID = '%1'") .arg(User::instance()->getUserID());
   QString contactsList;
   query = queryDatabase(queryString, true);
   if(query.lastError().type() == QSqlError::NoError && query.isValid()){
    query.first();
    contactsList = query.value(0).toString();
   }else{
    //QMessageBox::critical(this, QObject::tr("Fatal Error"), tr("Can not query data from database server! <br> %1").arg(model.lastError().text()));
    qCritical()<<QString("Can not query data from database server!");
    qCritical()<<QString("Error: %1").arg(query.lastError().text());
    //statusBar()->showMessage(tr("Error! Can not query data from database server!"));
   }
   queryString = QString("update contacts set ContactsList = '%1' , ContactsListVersion = %2 where UID = '%3'") .arg(contactsList).arg(localContactsListVersion).arg(User::instance()->getUserID());
   query = queryDatabase(queryString, false);
   if(query.lastError().type() != QSqlError::NoError){qDebug()<<"Query Error:"<<query.lastError().text();}
   qDebug()<<"Update Remote Server Contacts List Version";
   User::instance()->setContactsList(contactsList);
   User::instance()->setRemoteServerContactsListVersion(localContactsListVersion);
   //qDebug()<<"contactsList~~~~"<<contactsList;
  }


 }


 qDebug()<<"-------------contactsList:"<<User::instance()->getContactsList();
 //queryString = QString("select UID, Role, Dept, TrueName, NickName, HeadPortrait, LastLoginIP from contacts where UID in (%1) ") .arg(User::instance()->getContactsList());
 queryString = QString("select UID, Role, Dept, TrueName, NickName, Icon, LastLoginIP from contacts where UID in (%1) ") .arg(User::instance()->getContactsList());
query.clear();
 query = queryDatabase(queryString, true);
 query.first();
 qDebug()<<"-------------queryString:"<<queryString;
 qDebug()<<"-------------query.size:"<<query.size();
 record = query.record();

 while (query.next()){
  QString contactUID = QVariant(query.value(record.indexOf("UID"))).toString();
  int contactsRoleID = QVariant(query.value(record.indexOf("Role"))).toInt();
  QString contactDept = QVariant(query.value(record.indexOf("Dept"))).toString();
  QString contactTrueName = QVariant(query.value(record.indexOf("TrueName"))).toString();
  QString contactNickName = QVariant(query.value(record.indexOf("NickName"))).toString();
  QString contactHeadPortrait = QVariant(query.value(record.indexOf("HeadPortrait"))).toString();
  QString contactLastLoginIP = QVariant(query.value(record.indexOf("LastLoginIP"))).toString();

  ContactBase *contact = new ContactBase();
  contact->setUID(contactUID);
  contact->setRoleID(contactsRoleID);
  contact->setDept(contactDept);
  contact->setHeadPortrait(contactHeadPortrait);
  contact->setTrueName(contactTrueName);
  contact->setNickName(contactNickName);
  contact->setLastLoginIP(contactLastLoginIP);

  contactHash.insert(contactUID, contact);

 }

 record.clearValues();
 queryString = QString("select * from group");
 query = queryDatabase(queryString, true);
 record = query.record();
 while (query.next()){
  int contactGroupID = QVariant(query.value(record.indexOf("ID"))).toInt();
  QString contactGroupName = QVariant(query.value(record.indexOf("Name"))).toString();
  int contactGroupParentID = QVariant(query.value(record.indexOf("ParentCategory"))).toInt();
  QString contactGroupRemark = QVariant(query.value(record.indexOf("Remark"))).toString();

  ContactGroup *group = new ContactGroup();
  group->setGroupID(contactGroupID);
  group->setGroupName(contactGroupName);
  group->setParentGroupID(contactGroupParentID);

  contactGroupHash.insert(contactGroupID, group);

 }



 qDebug()<<"ContactsManager::slotUpdateContactsInfo()~~~~";

}

*/


/*
 void ContactsManager::slotLoadContacts(ItemBoxWidget *widgetBox, bool loadFromFile, const QString &string){

 QString file; // = QApplication::applicationDirPath()+QDir::separator()+"contacts.xml";

  if(string.isEmpty() || !QFile(string).exists()){
   file = ":/text/resources/text/default_contacts.xml";
  }


  if(loadFromFile){
   widgetBox->setFileName(file);
   widgetBox->load();
  }else{
   widgetBox->loadContents(string);
  }


  qDebug()<<"~~~~~~~~~~~~~ Load Contacts ~~~~~~~~~~~~~~";


}
*/


QSqlQuery ContactsManager::queryDatabase(const QString & queryString, bool localConfigDatabase) {

    QSqlQuery query;
    DatabaseUtility du;


    //                QString userPrivateDataFilePath = Settings::instance()->getUserPrivateDataFilePath(IMUser::instance()->getUserID());

    if(localConfigDatabase){
        query = du.queryDatabase(queryString,
                                 LOCAL_USERDATA_DB_CONNECTION_NAME,
                                 LOCAL_USERDATA_DB_DRIVER,
                                 "",
                                 0,
                                 "",
                                 "",
                                 userPrivateDataFilePath,
                                 HEHUI::SQLITE);
    }else{
        query = du.queryDatabase(queryString,
                                 REMOTE_SITOY_COMPUTERS_DB_CONNECTION_NAME,
                                 REMOTE_SITOY_COMPUTERS_DB_DRIVER,
                                 REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST,
                                 REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT,
                                 REMOTE_SITOY_COMPUTERS_DB_USER_NAME,
                                 REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD,
                                 REMOTE_SITOY_COMPUTERS_DB_NAME,
                                 HEHUI::MYSQL);
    }

    return query;


}


QSqlQuery ContactsManager::queryDatabase(const QString & queryString, const QString &connectionName, const QString &driver,
                                         const QString &host, int port, const QString &user, const QString &passwd,
                                         const QString &databaseName, HEHUI::DatabaseType databaseType) {


    QSqlQuery query;
    DatabaseUtility du(this);

    query = du.queryDatabase(queryString, connectionName, driver, host, port, user, passwd, databaseName, databaseType);

    return query;

}





} //namespace HEHUI
