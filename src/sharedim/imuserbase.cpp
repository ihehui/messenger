/*
 ****************************************************************************
 * imuserbase.cpp
 *
 * Created on: 2008-10-14
 *     Author: 贺辉
 *    License: LGPL
 *    Comment:
 *
 *
 *    =============================  Usage  =============================
 *|
 *|
 *    ===================================================================
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ****************************************************************************
 */

/*
  ***************************************************************************
  * Last Modified on: 2010-07-03
  * Last Modified by: 贺辉
  ***************************************************************************
*/




#include "imuserbase.h"
#include "HHSharedCore/hcryptography.h"


namespace HEHUI {

IMUserBase::IMUserBase(QObject *parent)
    :User("", "","", parent)
{

    init();

}

IMUserBase::IMUserBase(const QString &userID, QObject *parent)
    :User(userID, "", "", parent)
{

    init();

}

IMUserBase::~IMUserBase() {

    foreach (ContactGroupBase *group, personalContactGroupsHash) {
        delete group;
        group = 0;
    }
    m_friendsGroup =0;
    m_strangersGroup = 0;
    m_blacklistGroup = 0;
    personalContactGroupsHash.clear();


    delete updatedPropertiesMutex;
    updatedPropertiesMutex = 0;

}

QString IMUserBase::defaultFriendContactGroupName(){
    return ContactGroupBase::Group_Friends_Name;
}
quint32 IMUserBase::defaultFriendContactGroupID(){
    return ContactGroupBase::Group_Friends_ID;
}

QString IMUserBase::defaultStrangerContactGroupName(){
    return ContactGroupBase::Group_Strangers_Name;
}

quint32 IMUserBase::defaultStrangerContactGroupID(){
    return ContactGroupBase::Group_Strangers_ID;
}

QString IMUserBase::defaultBlacklistContactGroupName(){
    return ContactGroupBase::Group_Blacklist_Name;
}

quint32 IMUserBase::defaultBlacklistContactGroupID(){
    return ContactGroupBase::Group_Blacklist_ID;
}


void IMUserBase::init(){


//    int gID = ContactGroupBase::Group_Strangers_ID;
//    m_strangersGroup = new ContactGroupBase(gID, ContactGroupBase::Group_Strangers_Name, this);
//    personalContactGroupsHash.insert(gID, m_strangersGroup);

//    gID = ContactGroupBase::Group_Blacklist_ID;
//    m_blacklistGroup = new ContactGroupBase(gID, ContactGroupBase::Group_Blacklist_Name, this);
//    personalContactGroupsHash.insert(gID, m_blacklistGroup);

    ensureDefaultGroups();


    //    systemGroupsHash = QHash<quint32, QString>();
    //    personalInterestGroupsHash = QHash<quint32, QString> ();
    //    personalContacts = QHash<quint32, QString> ();
    
    //personalContactGroupsHash = QHash<QString, QStringList> ();
    //personalContactGroupsHash.clear();

    personalMessageInfoVersion = 0;
    personalMessage = "";

//    interestGroups.clear();
    interestGroupInfoVersion = 0;

    personalSummaryInfoVersion = 0;
    personalDetailInfoVersion = 0;
    personalContactGroupsInfoVersion = 0;

    onlineState = IM::ONLINESTATE_OFFLINE;
    friendshipApply = FA_AUTO_ACCEPT;
    shortTalk = ST_PROMPT;
    userRole = 0;
    accountState = AS_Normal;

    sessionEncryptionKey = encryptedPassword();
    
    updatedPropertiesMutex =new QMutex();
    

    lastLoginDeviceInfo = "";

    m_socketID = INVALID_SOCK_ID;


}

QByteArray IMUserBase::encryptedPassword() const{

    return Cryptography::MD5(Cryptography::SHA1(getPassword().toUtf8()));

}

void IMUserBase::addUpdatedPersonalInfoProperty(IM::PropertyIDOfUser propertyID, const QString &value, bool summaryInfo){

    QMutexLocker locker(updatedPropertiesMutex);
    if(summaryInfo){
        updatedSummaryInfoProperties.insert(propertyID, value);
    }else{
        updatedDetailInfoProperties.insert(propertyID, value);
    }
    
}

QString IMUserBase::getUpdateSQLStatement(bool summaryInfo) const{
    qDebug()<<"--IMUserBase::getUpdateSQLStatement(...)";

    QMutexLocker locker(updatedPropertiesMutex);
    
    QStringList sqlstatements;
    if(summaryInfo){
        QList<IM::PropertyIDOfUser> keys = updatedSummaryInfoProperties.keys();
        foreach (IM::PropertyIDOfUser propertyID, keys) {
            QString propertyName = databaseColumnName(propertyID);
            if(propertyName.isEmpty()){
                qCritical()<<QString("Unknown Property ID '%1'!").arg(propertyID);
                continue;
            }
            sqlstatements.append(QString(propertyName + "=" + updatedSummaryInfoProperties.value(propertyID)));
        }
    }else{
        QList<IM::PropertyIDOfUser> keys = updatedDetailInfoProperties.keys();
        foreach (IM::PropertyIDOfUser propertyID, keys) {
            QString propertyName = databaseColumnName(propertyID);
            if(propertyName.isEmpty()){
                qCritical()<<QString("Unknown Property ID '%1'!").arg(propertyID);
                continue;
            }
            sqlstatements.append(QString(propertyName + "=" + updatedDetailInfoProperties.value(propertyID)));
        }

    }

    return sqlstatements.join(" , ");
    
}

void IMUserBase::clearUpdatedProperties(){
    QMutexLocker locker(updatedPropertiesMutex);
    
    updatedSummaryInfoProperties.clear();
    updatedDetailInfoProperties.clear();
    
    
}

//QString IMUserBase::databaseColumnName(IM::PropertyID propertyID) const{
//    QString columnName = "";
//    switch(propertyID){
//    case IM::PI_UserID:
//        columnName = "UserID";
//        break;
//    case IM::PI_Password:
//        columnName = "UserPassword";
//        break;
//    case IM::PI_TrueName:
//        columnName = "TrueName";
//        break;
//    case IM::PI_NickName:
//        columnName = "NickName";
//        break;
//    case IM::PI_Gender:
//        columnName = "Gender";
//        break;
//    case IM::PI_Age:
//        columnName = "Age";
//        break;
//    case IM::PI_Face:
//        columnName = "Face";
//        break;

//    case IM::PI_PersonalContactGroupsInfoString:
//        columnName = "PersonalContactGroupsInfo";
//        break;
//    case IM::PI_PersonalContactGroupsInfoVersion:
//        columnName = "PersonalContactGroupsInfoVersion";
//        break;
//    case IM::PI_PersonalInterestGroupsInfoString:
//        columnName = "PersonalInterestGroupsInfo";
//        break;
//    case IM::PI_PersonalInterestGroupsInfoVersion:
//        columnName = "PersonalInterestGroupsInfoVersion";
//        break;
//    case IM::PI_SystemGroupsInfoString:
//        columnName = "SystemGroupsInfo";
//        break;
//    case IM::PI_SystemGroupsInfoVersion:
//        columnName = "SystemGroupsInfoVersion";
//        break;
//    case IM::PI_PersonalInfoVersion:
//        columnName = "PersonalInfoVersion";
//        break;


//    case IM::PI_HomeAddress:
//        columnName = "HomeAddress";
//        break;
//    case IM::PI_HomePhoneNumber:
//        columnName = "HomePhoneNumber";
//        break;
//    case IM::PI_HomeZipCode:
//        columnName = "HomeZipCode";
//        break;
//    case IM::PI_PersonalHomepage:
//        columnName = "PersonalHomepage";
//        break;
//    case IM::PI_PersonalEmailAddress:
//        columnName = "PersonalEmailAddress";
//        break;
//    case IM::PI_LastLoginTime:
//        columnName = "LastLoginTime";
//        break;
//    case IM::PI_LastLoginHostAddress:
//        columnName = "LastLoginHostAddress";
//        break;
//    case IM::PI_LastLoginHostPort:
//        columnName = "LastLoginHostPort";
//        break;
//    case IM::PI_QuestionForSecurity:
//        columnName = "QuestionForSecurity";
//        break;
//    case IM::PI_AnswerForSecurity:
//        columnName = "AnswerForSecurity";
//        break;

//    case IM::PI_CompanyName:
//        columnName = "CompanyName";
//        break;
//    case IM::PI_JobTitle:
//        columnName = "JobTitle";
//        break;
//    case IM::PI_BusinessAddress:
//        columnName = "BusinessAddress";
//        break;
//    case IM::PI_BusinessPhoneNumber:
//        columnName = "BusinessPhoneNumber";
//        break;
//    case IM::PI_BusinessZipCode:
//        columnName = "BusinessZipCode";
//        break;
//    case IM::PI_BusinessFaxNumber:
//        columnName = "BusinessFaxNumber";
//        break;
//    case IM::PI_BusinessHomepage:
//        columnName = "BusinessHomepage";
//        break;
//    case IM::PI_BusinessEmailAddress:
//        columnName = "BusinessEmailAddress";
//        break;
//    case IM::PI_RegistrationTime:
//        columnName = "RegistrationTime";
//        break;
//    case IM::PI_LoginTimes:
//        columnName = "LoginTimes";
//        break;
//    case IM::PI_FriendshipApply:
//        columnName = "FriendshipApply";
//        break;
//    case IM::PI_ShortTalk:
//        columnName = "ShortTalk";
//        break;

//    case IM::PI_Role:
//        columnName = "Role";
//        break;

//    default:
//        columnName = "";




//    }

//    return columnName;



//}



quint32 IMUserBase::updatePersonalContactGroupsInfoVersion(){
    return 0;


    this->personalContactGroupsInfoVersion++;
    
    addUpdatedPersonalInfoProperty(IM::PI_PersonalContactGroupsInfoVersion, QString::number(personalContactGroupsInfoVersion), true);

    return personalContactGroupsInfoVersion;
}

//QString IMUserBase::getDefaultGroupName() const{
//    return defaultGroupName;
//}


void IMUserBase::setPersonalInfoString(const QString &personalInfoString, bool summaryInfo){
    qDebug()<<"--IMUserBase::setPersonalInfoString(...)"<<" personalSummaryInfo:"<<personalInfoString;
    
    QStringList infoList = personalInfoString.split(QString(PACKET_DATA_SEPARTOR));
    if(infoList.at(0) != this->getUserID()){
        return;
    }

    if(summaryInfo){

        QString trueNameString = infoList.at(1);
        setTrueName(trueNameString);
        addUpdatedPersonalInfoProperty(IM::PI_TrueName, "'"+trueNameString+"'", summaryInfo);

        QString nickNameString = infoList.at(2);
        setNickName(nickNameString);
        addUpdatedPersonalInfoProperty(IM::PI_NickName, "'"+nickNameString+"'", summaryInfo);

        QString genderString = infoList.at(3);
        setGender(Gender(genderString.toUInt()));
        addUpdatedPersonalInfoProperty(IM::PI_Gender, genderString, summaryInfo);

        QString ageString = infoList.at(4);
        setAge(ageString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_Age, ageString, summaryInfo);

        QString faceString = infoList.at(5);
        setFace(faceString);
        addUpdatedPersonalInfoProperty(IM::PI_Face, "'"+faceString+"'", summaryInfo);

        QString personalContactGroupsVersionString = infoList.at(6);
        setPersonalContactGroupsVersion(personalContactGroupsVersionString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_PersonalContactGroupsInfoVersion, personalContactGroupsVersionString, summaryInfo);

        QString interestGroupInfoString = infoList.at(7);
        setInterestGroupInfoVersion(interestGroupInfoString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_InterestGroupsInfoVersion, interestGroupInfoString, summaryInfo);

        QString personalMessageInfoVersionString = infoList.at(8);
        setPersonalMessageInfoVersion(personalMessageInfoVersionString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_PersonalMessageInfoVersion, personalMessageInfoVersionString, summaryInfo);

        QString personalSummaryInfoVersionString = infoList.at(9);
        setPersonalSummaryInfoVersion(personalSummaryInfoVersionString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_PersonalSummaryInfoVersion, personalSummaryInfoVersionString, summaryInfo);

        QString personalDetailInfoVersionString = infoList.at(10);
        setPersonalDetailInfoVersion(personalDetailInfoVersionString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_PersonalDetailInfoVersion, personalDetailInfoVersionString, summaryInfo);

        QString friendshipApplyString = infoList.at(11);
        setFriendshipApply(FriendshipApply(friendshipApplyString.toUInt()));
        addUpdatedPersonalInfoProperty(IM::PI_FriendshipApply, friendshipApplyString, summaryInfo);

        QString shortTalkString = infoList.at(12);
        setShortTalk(ShortTalk(shortTalkString.toUInt()));
        addUpdatedPersonalInfoProperty(IM::PI_ShortTalk, shortTalkString, summaryInfo);

        QString userRoleString = infoList.at(13);
        setUserRole(userRoleString.toUInt());
        addUpdatedPersonalInfoProperty(IM::PI_Role, userRoleString, summaryInfo);

        QString descriptionString = infoList.at(14);
        setDescription(descriptionString);
        addUpdatedPersonalInfoProperty(IM::PI_Description, "'" + descriptionString + "'", summaryInfo);

        QString accountStateString = infoList.at(15);
        setAccountState(AccountState(accountStateString.toUInt()));
        addUpdatedPersonalInfoProperty(IM::PI_AccountState, accountStateString, summaryInfo);

        QString personalMessage = infoList.at(16);
        setPersonalMessage(personalMessage);
        addUpdatedPersonalInfoProperty(IM::PI_PersonalMessage, "'" + personalMessage + "'", true);

    }else{

        QString homeAddressString = infoList.at(1);
        setHomeAddress(homeAddressString);
        addUpdatedPersonalInfoProperty(IM::PI_HomeAddress, "'"+homeAddressString+"'", summaryInfo);

        QString homePhoneNumberString = infoList.at(2);
        setHomePhoneNumber(homePhoneNumberString);
        addUpdatedPersonalInfoProperty(IM::PI_HomePhoneNumber, "'"+homePhoneNumberString+"'", summaryInfo);

        QString homeZipCodeString = infoList.at(3);
        setHomeZipCode(homeZipCodeString);
        addUpdatedPersonalInfoProperty(IM::PI_HomeZipCode, "'"+homeZipCodeString+"'", summaryInfo);

        QString personalHomepageString = infoList.at(4);
        setPersonalHomepage(personalHomepageString);
        addUpdatedPersonalInfoProperty(IM::PI_PersonalHomepage, "'"+personalHomepageString+"'", summaryInfo);

        QString personalEmailAddressString = infoList.at(5);
        setPersonalEmailAddress(personalEmailAddressString);
        addUpdatedPersonalInfoProperty(IM::PI_PersonalEmailAddress, "'"+personalEmailAddressString+"'", summaryInfo);

        QString companyNameString = infoList.at(6);
        setPersonalEmailAddress(companyNameString);
        addUpdatedPersonalInfoProperty(IM::PI_CompanyName, "'"+companyNameString+"'", summaryInfo);

        QString jobTitleString = infoList.at(7);
        setJobTitle(jobTitleString);
        addUpdatedPersonalInfoProperty(IM::PI_JobTitle, "'"+jobTitleString+"'", summaryInfo);

        QString businessAddressString = infoList.at(8);
        setBusinessAddress(businessAddressString);
        addUpdatedPersonalInfoProperty(IM::PI_BusinessAddress, "'"+businessAddressString+"'", summaryInfo);

        QString businessPhoneNumberString = infoList.at(9);
        setBusinessPhoneNumber(businessPhoneNumberString);
        addUpdatedPersonalInfoProperty(IM::PI_BusinessPhoneNumber, "'"+businessPhoneNumberString+"'", summaryInfo);

        QString businessZipCodeString = infoList.at(10);
        setBusinessZipCode(businessZipCodeString);
        addUpdatedPersonalInfoProperty(IM::PI_BusinessZipCode, "'"+businessZipCodeString+"'", summaryInfo);

        QString businessFaxNumberString = infoList.at(11);
        setBusinessFaxNumber(businessFaxNumberString);
        addUpdatedPersonalInfoProperty(IM::PI_BusinessFaxNumber, "'"+businessFaxNumberString+"'", summaryInfo);

        QString businessHomepageString = infoList.at(12);
        setBusinessHomepage(businessHomepageString);
        addUpdatedPersonalInfoProperty(IM::PI_BusinessHomepage, "'"+businessHomepageString+"'", summaryInfo);

        QString businessEmailAddressString = infoList.at(13);
        setBusinessEmailAddress(businessEmailAddressString);
        addUpdatedPersonalInfoProperty(IM::PI_BusinessEmailAddress, "'"+businessEmailAddressString+"'", summaryInfo);

    }


}

QString IMUserBase::getPersonalInfoString(bool requestSummaryInfo) const{
    QStringList infoList;
    if(requestSummaryInfo){
        infoList << this->getUserID()
                 << this->getTrueName() << this->getNickName()
                 << QString::number(this->getGender()) << QString::number(getAge()) << this->getFace()
                 << QString::number(this->getPersonalContactGroupsVersion()) << QString::number(this->getInterestGroupInfoVersion())
                 << QString::number(this->getPersonalMessageInfoVersion()) << QString::number(this->getPersonalSummaryInfoVersion()) << QString::number(this->getPersonalDetailInfoVersion())
                 << QString::number(this->getFriendshipApply()) << QString::number(this->getShortTalk()) << QString::number(this->getUserRole())
                 << getdescription() << QString::number(getAccountState()) << getPersonalMessage()
                    ;
    }else{
        infoList << this->getUserID()
                 << this->getHomeAddress() << this->getHomePhoneNumber() << this->getHomeZipCode()
                 << this->getPersonalHomepage() << this->getPersonalEmailAddress()
                 << this->getCompanyName() << this->getJobTitle() << this->getBusinessAddress()
                 << this->getBusinessPhoneNumber() << this->getBusinessZipCode() << this->getBusinessFaxNumber()
                 << this->getBusinessHomepage() << this->getBusinessEmailAddress()
                 //<< this->getRegistrationTime().toString("yyyy-MM-dd HH:mm:ss")
                    ;
    }

    return infoList.join(QString(PACKET_DATA_SEPARTOR));

}

void IMUserBase::setContactGroupsInfoString(const QString &contactGroupsInfo){
    qDebug()<<"--IMUserBase::setContactGroupsInfoString(...) "<<"      contactGroupsInfo:"<<contactGroupsInfo;
    
    //STRING FORMATE: GroupID,GroupName,UserID,,UserID,...||GroupID,...
    //e.g. 100,Group100,user1,user2,user3||101,Group101,user4

    foreach (ContactGroupBase *group, personalContactGroupsHash) {
        delete group;
        group = 0;
    }
    m_friendsGroup = 0;
    m_strangersGroup = 0;
    m_blacklistGroup = 0;
    personalContactGroupsHash.clear();



    if(contactGroupsInfo.trimmed().isEmpty()){
        ensureDefaultGroups();
        return;
    }
    
    QStringList groupsInfoList = contactGroupsInfo.split(GROUP_INFO_SEPARATOR);
    for(int i=0; i<groupsInfoList.size(); i++) {
        QStringList contactList = groupsInfoList.at(i).split(",");
        Q_ASSERT(contactList.size() >= 2);

        quint32 groupID = contactList.takeFirst().toUInt();
        QString groupName = contactList.takeFirst();
        ContactGroupBase * contactGroup = new ContactGroupBase(groupID, groupName, this);
        contactGroup->setMembers(contactList);
        contactGroup->clearUpdatedProperties();
        personalContactGroupsHash.insert(groupID, contactGroup);

    }

    ensureDefaultGroups();

}

QString IMUserBase::getContactGroupsInfoString() const{
    
    //STRING FORMATE: GroupID,GroupName,UserID,,UserID,...||GroupID,...
    //e.g. 100,Group100,user1,user2,user3||101,Group101,user4


    if(personalContactGroupsHash.isEmpty()){
        return "";
    }

    QStringList groupsInfo;
    foreach (ContactGroupBase *group, personalContactGroupsHash) {
        QString infoString = QString::number(group->getGroupID()) + "," + group->getGroupName() ;
        QString membersString = group->getMembersAsString(",");
        if(!membersString.trimmed().isEmpty()){
            infoString += "," + membersString;
        }
        groupsInfo.append(infoString);
    }
    return groupsInfo.join(GROUP_INFO_SEPARATOR);


}



QList<ContactGroupBase *> IMUserBase::getContactGroups(bool noStrangers, bool noBlacklisted){

    QList<ContactGroupBase *> groups = personalContactGroupsHash.values();

    groups.removeAll(m_friendsGroup);
    groups.insert(0, m_friendsGroup);

    groups.removeAll(m_strangersGroup);
    groups.removeAll(m_blacklistGroup);

    if(!noStrangers){
        groups.append(m_strangersGroup);
    }
    if(!noBlacklisted){
        groups.append(m_blacklistGroup);
    }

    return groups;
}

QStringList IMUserBase::friendContactGroupNames(){

    QList<int> groupIDs = personalContactGroupsHash.keys();
    groupIDs.removeAll(ContactGroupBase::Group_Strangers_ID);
    groupIDs.removeAll(ContactGroupBase::Group_Blacklist_ID);
    //groupIDs.removeAll(ContactGroupBase::Group_Friends_ID);

    QStringList groupNames;
    foreach (int groupID , groupIDs) {
        groupNames.append(personalContactGroupsHash.value(groupID)->getGroupName());
    }

    return groupNames;

}

ContactGroupBase *IMUserBase::getContactGroup(int personalContactGroupID){
    ContactGroupBase * group = 0;
    if(personalContactGroupsHash.contains(personalContactGroupID)){
        group = personalContactGroupsHash.value(personalContactGroupID);
    }

    return group;
}

ContactGroupBase *IMUserBase::getContactGroup(const QString &groupName){

    foreach (ContactGroupBase *contactGroup, personalContactGroupsHash.values()) {
        if(contactGroup->getGroupName().toLower() == groupName.toLower()){
            return contactGroup;
        }
    }

    return 0;
}

quint32 IMUserBase::getContactGroupID(const QString &groupName){
    ContactGroupBase * contactGroup = getContactGroup(groupName);
    if(contactGroup){
        return contactGroup->getGroupID();
    }

    return 0;
}

QString IMUserBase::getContactGroupName(int groupID) const{
    ContactGroupBase * contactGroup = personalContactGroupsHash.value(groupID);
    if(!contactGroup){
        return "";
    }

    return contactGroup->getGroupName();
}

ContactGroupBase * IMUserBase::addContactGroup(int contactGroupID, const QString &groupName){
    if(personalContactGroupsHash.contains(contactGroupID)){
        ContactGroupBase *group = personalContactGroupsHash.value(contactGroupID);
        if(groupName != group->getGroupName()){
            group->setGroupName(groupName);
        }
        return group;
    }

    ContactGroupBase * group = new ContactGroupBase(contactGroupID, groupName, this);
    personalContactGroupsHash.insert(contactGroupID, group);
    return group;

}

void IMUserBase::deleteContactGroup(quint32 contactGroupID){
    if(!personalContactGroupsHash.contains(contactGroupID)){
        return ;
    }

    ContactGroupBase * group = personalContactGroupsHash.take(contactGroupID);
    delete group;
    group = 0;

    ensureDefaultGroups();

}

void IMUserBase::renameContactGroup(quint32 contactGroupID, const QString &newGroupName){

    if(!personalContactGroupsHash.contains(contactGroupID)){
        return ;
    }

    ContactGroupBase * group = personalContactGroupsHash.take(contactGroupID);
    group->setGroupName(newGroupName);

}

QString IMUserBase::groupNameThatContactBelongsTo(const QString &contactID) const{

    foreach (ContactGroupBase *contactGroup, personalContactGroupsHash.values()) {
        if(contactGroup->hasMember(contactID)){
            return contactGroup->getGroupName();
        }
    }

    return "";
}

int IMUserBase::groupIDThatContactBelongsTo(const QString &contactID){

    foreach (ContactGroupBase *contactGroup, personalContactGroupsHash.values()) {
        if(contactGroup->hasMember(contactID)){
            return contactGroup->getGroupID();
        }
    }

    return ContactGroupBase::Group_Strangers_ID;
}


bool IMUserBase::hasContactGroup(quint32 groupID){
    return personalContactGroupsHash.contains(groupID);
}

bool IMUserBase::hasContactGroup(const QString &groupName){
    foreach (ContactGroupBase *contactGroup, personalContactGroupsHash.values()) {
        if(contactGroup->getGroupName().toLower() == groupName.toLower()){
            return true;
        }
    }

    return false;
}

QStringList IMUserBase::getContactGroupMembers(int groupID){
    if(!personalContactGroupsHash.contains(groupID)){
        return QStringList();
    }

    return personalContactGroupsHash.value(groupID)->members();
}

int IMUserBase::countOfContactGroupMembers(int groupID){
    if(!personalContactGroupsHash.contains(groupID)){
        return -1;
    }

    return personalContactGroupsHash.value(groupID)->countOfMembers();
}

QStringList IMUserBase::getAllContacts(bool noStrangers, bool noBlacklisted) const{

    QList<ContactGroupBase *> groups = personalContactGroupsHash.values();
    if(noStrangers){
        groups.removeAll(m_strangersGroup);
    }
    if(noBlacklisted){
        groups.removeAll(m_blacklistGroup);
    }


    QStringList contacts;
    foreach (ContactGroupBase *contactGroup, groups) {
        contacts << contactGroup->members();
    }

    return contacts;

}

bool IMUserBase::hasFriendContact(const QString &contactID){

    QList<ContactGroupBase *> groups = personalContactGroupsHash.values();
    groups.removeAll(m_strangersGroup);
    groups.removeAll(m_blacklistGroup);

    foreach (ContactGroupBase *contactGroup, groups) {
        if(contactGroup->hasMember(contactID)){
            return true;
        };
    }

    return false;
}

bool IMUserBase::hasContact(const QString &contactID){

    QList<ContactGroupBase *> groups = personalContactGroupsHash.values();
    foreach (ContactGroupBase *contactGroup, groups) {
        if(contactGroup->hasMember(contactID)){
            return true;
        };
    }

    return false;
}

bool IMUserBase::addNewContact(const QString &contactID, int groupID){
    
    if(contactID.trimmed().isEmpty()){
        return false;
    }
    ContactGroupBase *contactGroup = personalContactGroupsHash.value(groupID);
    if(!contactGroup){
        qDebug()<<"ERROR! Contact group does not exist!";
        return false;
    }


//    QList<ContactGroupBase *> groups = personalContactGroupsHash.values();
//    foreach (ContactGroupBase *group, groups) {
//        if(group->hasMember(contactID)){
//            //if(group->getGroupID() == ContactGroupBase::Group_Strangers_ID){
//                group->deleteMember(contactID);
//            //}else{
//            //    qDebug()<<"ERROR! Contact "<<contactID<<" already exists in group:"<<group->getGroupName();
//            //    return false;
//            //}
//        }
//    }

    deleteContact(contactID);


    contactGroup->addMember(contactID);
    updatePersonalContactGroupsInfoVersion();

    
    return true;

}

bool IMUserBase::deleteContact(const QString &contactID){

    QList<ContactGroupBase *> groups = personalContactGroupsHash.values();

    foreach (ContactGroupBase *contactGroup, groups) {
        if(contactGroup->hasMember(contactID)){
            contactGroup->deleteMember(contactID);
            updatePersonalContactGroupsInfoVersion();
            return true;
        }
    }

    return false;
}

bool IMUserBase::moveFriendContact2(const QString &contactID, int oldGroupID, int newGroupID){
        
    //TODO
    if(oldGroupID == ContactGroupBase::Group_Blacklist_ID
            || oldGroupID == ContactGroupBase::Group_Strangers_ID
            || newGroupID == ContactGroupBase::Group_Blacklist_ID
            || newGroupID == ContactGroupBase::Group_Strangers_ID
            ){
        return false;
    }

    ContactGroupBase *oldContactGroup = personalContactGroupsHash.value(oldGroupID);
    ContactGroupBase *newContactGroup = personalContactGroupsHash.value(newGroupID);
    Q_ASSERT(oldContactGroup);
    Q_ASSERT(newContactGroup);

    oldContactGroup->deleteMember(contactID);
    newContactGroup->addMember(contactID);
    
    updatePersonalContactGroupsInfoVersion();
//    addUpdatedPersonalInfoProperty(IM::PI_PersonalContactGroupsInfoString, "'"+getContactGroupsInfoString()+"'");

    return true;

}

void IMUserBase::moveContactToAnotherGroup(const QString &contactID, int oldGroupID, int newGroupID){

    ContactGroupBase *oldContactGroup = personalContactGroupsHash.value(oldGroupID);
    ContactGroupBase *newContactGroup = personalContactGroupsHash.value(newGroupID);
    Q_ASSERT(oldContactGroup);
    Q_ASSERT(newContactGroup);

    oldContactGroup->deleteMember(contactID);
    newContactGroup->addMember(contactID);

    updatePersonalContactGroupsInfoVersion();
}

//bool IMUserBase::addContactToBlacklist(const QString &contactID){
//    if(blacklist.contains(contactID)){return false;}
//    blacklist.append(contactID);

//    updateBlacklistInfoVersion();
//    addUpdatedProperty(IM::PI_Blacklist, "'"+getBlacklistInfoString()+"'");

//}

//bool IMUserBase::deleteContactFromBlacklist(const QString &contactID){
//    blacklist.removeAll(contactID);

//    updateBlacklistInfoVersion();
//    addUpdatedProperty(IM::PI_Blacklist, "'"+getBlacklistInfoString()+"'");
//}

//void IMUserBase::joinOrLeaveInterestGroup(quint32 interestGroupID, bool join){

//    if(join){
//        if(interestGroups.contains(interestGroupID)){
//            return;
//        }else{
//            interestGroups.append(interestGroupID);
//        }
//    }else{
//        interestGroups.removeAll(interestGroupID);
//    }

//    updateInterestGroupInfoVersion();

////    addUpdatedPersonalInfoProperty(IM::PI_InterestGroupsInfoString, "'"+interestGroups.join(",")+"'");

//}

//bool IMUserBase::isMemberOfInterestGroup(quint32 interestGroupID){
//    return interestGroups.contains(interestGroupID);
//}

quint32 IMUserBase::updateInterestGroupInfoVersion(){

    this->interestGroupInfoVersion++;
    addUpdatedPersonalInfoProperty(IM::PI_InterestGroupsInfoVersion, QString::number(interestGroupInfoVersion), true);

    return interestGroupInfoVersion;

}

bool IMUserBase::addOrDeleteBlacklistedContact(const QString &contactID,  bool addToBlacklist){

    if(contactID.trimmed().isEmpty()){return false;}

    int groupID = ContactGroupBase::Group_Blacklist_ID;
    ContactGroupBase *blacklistGroup = personalContactGroupsHash.value(groupID);
    if(!blacklistGroup){
        blacklistGroup = new ContactGroupBase(groupID, ContactGroupBase::Group_Blacklist_Name, this);
        personalContactGroupsHash.insert(groupID, blacklistGroup);
    }

    if(addToBlacklist){
        if(blacklistGroup->hasMember(contactID)){return false;}
        blacklistGroup->addMember(contactID);
    }else{
        if(!blacklistGroup->hasMember(contactID)){return false;}
        blacklistGroup->deleteMember(contactID);
    }

    updatePersonalMessageInfoVersion();
//    addUpdatedPersonalInfoProperty(IM::PI_Blacklist, "'"+getBlacklistInfoString()+"'");
    
    return true;
}

quint32 IMUserBase::updatePersonalMessageInfoVersion(){
    this->personalMessageInfoVersion++;
    addUpdatedPersonalInfoProperty(IM::PI_PersonalMessageInfoVersion, QString::number(personalMessageInfoVersion), true);

    return personalMessageInfoVersion;

}

bool IMUserBase::isContactBlacklisted(const QString &contactID){
    return m_blacklistGroup->hasMember(contactID);
}

QStringList IMUserBase::blacklistedContacts(){
    return m_blacklistGroup->members();
}

int IMUserBase::getUnusedContactGroupID(){
    QList<int> ids = personalContactGroupsHash.keys();
    int id = -65535;
    for(int i=ContactGroupBase::Group_Friends_ID+1; i<65535; i++){
        if(!ids.contains(i)){
            id = i;
            break;
        }
        continue;
    }

//    qDebug()<<"Unused Contact Group ID:"<<id;
    return id;

}

void IMUserBase::ensureDefaultGroups(){

    int gID = ContactGroupBase::Group_Friends_ID;
    m_friendsGroup = personalContactGroupsHash.value(gID);
    if(!m_friendsGroup){
        m_friendsGroup = new ContactGroupBase(gID, ContactGroupBase::Group_Friends_Name, this);
        personalContactGroupsHash.insert(gID, m_friendsGroup);
    }else{
        m_friendsGroup->setGroupName(ContactGroupBase::Group_Friends_Name);
    }

    gID = ContactGroupBase::Group_Strangers_ID;
    m_strangersGroup = personalContactGroupsHash.value(gID);
    if(!m_strangersGroup){
        m_strangersGroup = new ContactGroupBase(gID, ContactGroupBase::Group_Strangers_Name, this);
        personalContactGroupsHash.insert(gID, m_strangersGroup);
    }else{
        m_strangersGroup->setGroupName(ContactGroupBase::Group_Strangers_Name);
    }

    gID = ContactGroupBase::Group_Blacklist_ID;
    m_blacklistGroup = personalContactGroupsHash.value(gID);
    if(!m_blacklistGroup){
        m_blacklistGroup = new ContactGroupBase(gID, ContactGroupBase::Group_Blacklist_Name, this);
        personalContactGroupsHash.insert(gID, m_blacklistGroup);
    }else{
      m_blacklistGroup->setGroupName(ContactGroupBase::Group_Blacklist_Name);
    }


}

ContactGroupBase * IMUserBase::friendsGroup() const{
    return m_friendsGroup;
}

ContactGroupBase * IMUserBase::strangersGroup() const{
    return m_strangersGroup;
}

ContactGroupBase * IMUserBase::blacklistGroup() const{
    return m_blacklistGroup;
}



//QString IMUserBase::groupNameThatContactBelongsTo(const QString &contactID) const{

//    QString gpName = "";
//    QStringList groups = personalContactGroupsHash.keys();
//    foreach (QString groupName, groups) {
//        QStringList members = personalContactGroupsHash.value(groupName);
//        if(members.contains(contactID)){
//            gpName = groupName;
//            break;
//        }
//    }

//    return gpName;

//}













} //namespace HEHUI
