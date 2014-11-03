/*
 ****************************************************************************
 * imuserbase.h
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




#ifndef IMUSERBASE_H_
#define IMUSERBASE_H_

#include <QtCore>
#include <QObject>
#include <QStringList>


#include "constants_global_shared.h"
#include "sharedimlib.h"
#include "contactgroupbase.h"



#include "HHSharedCore/huser.h"
#include "HHSharedCore/hglobal_core.h"



namespace HEHUI {

class SHAREDIMLIB_API IMUserBase : public User//, public Singleton<IMUserBase>
{
    Q_OBJECT
    //friend class Singleton<IMUserBase>;

public:
    enum FriendshipApply{FA_AUTO_ACCEPT = 0, FA_REQUIRE_AUTHENTICATION = 1};
    enum FriendshipApplyResult{FAR_UNKNOWN = 0, FAR_ACCEPTED = 1, FAR_DENIED};

    enum ShortTalk{ST_AUTO_ACCEPT = 0, ST_PROMPT = 1, ST_DENY = 2};
    enum AgeSection{Age_Any = 0, Age_1_18, Age_19_30, Age_23_30, Age_31_40, Age_40_};

    enum AccountState{AS_Invalid = 0, AS_Normal, AS_Banned, AS_Limitted};

    IMUserBase(QObject *parent = 0);
    IMUserBase(const QString & userID, QObject *parent = 0);
    virtual ~IMUserBase();

    static QString defaultFriendContactGroupName();
    static quint32 defaultFriendContactGroupID();
    static QString defaultStrangerContactGroupName();
    static quint32 defaultStrangerContactGroupID();
    static QString defaultBlacklistContactGroupName();
    static quint32 defaultBlacklistContactGroupID();

    QByteArray encryptedPassword() const;
    
    void addUpdatedPersonalInfoProperty(IM::PropertyIDOfUser propertyID, const QString &value, bool summaryInfo);
    QString getUpdateSQLStatement(bool summaryInfo) const;
    void clearUpdatedProperties();
    
    virtual QString databaseColumnName(IM::PropertyIDOfUser propertyID) const = 0;

    quint32 updatePersonalContactGroupsInfoVersion();
    


    FriendshipApply getFriendshipApply() const
    {
        return friendshipApply;
    }

    IM::OnlineState getOnlineState() const
    {
        return onlineState;
    }

//    QHash<QString, QStringList> getPersonalContactGroupsHash() const
//    {
//        return personalContactGroupsHash;
//    }

    quint32 getPersonalMessageInfoVersion() const{
        return personalMessageInfoVersion;
    }
    QString getPersonalMessage() const{
        return personalMessage;
    }

    quint32 getPersonalContactGroupsVersion() const
    {
        return personalContactGroupsInfoVersion;
    }
    

    quint32 getInterestGroupInfoVersion() const
    {
        return interestGroupInfoVersion;
    }
    
//    QList<quint32> getInterestGroups() const
//    {
//        return interestGroups;
//    }
//    void setInterestGroups(const QStringList &groups){
//        this->interestGroups.clear();
//        foreach (QString groupID, groups) {
//            this->interestGroups.append(groupID.toUInt());
//        }
//    }
//    void addInterestGroup(quint32 groupID){
//        this->interestGroups.append(groupID);
//    }


    quint32 getPersonalSummaryInfoVersion() const
    {
        return personalSummaryInfoVersion;
    }
    quint32 getPersonalDetailInfoVersion() const
    {
        return personalDetailInfoVersion;
    }


    QByteArray getSessionEncryptionKey() const
    {
        return sessionEncryptionKey;
    }

    ShortTalk getShortTalk() const
    {
        return shortTalk;
    }

    //    QHash<quint32,QString> getSystemGroups() const
    //    {
    //        return systemGroupsHash;
    //    }

    int getUserRole() const
    {
        return userRole;
    }

    void setFriendshipApply(FriendshipApply friendshipApply)
    {
        this->friendshipApply = friendshipApply;
    }

    void setOnlineState(IM::OnlineState onlineState)
    {
        this->onlineState = onlineState;
    }

//    void setPersonalContactGroupsHash(QHash<QString, QStringList> personalContactGroups)
//    {
//        this->personalContactGroupsHash = personalContactGroups;
//    }

    void setPersonalMessageInfoVersion(quint32 personalMessageInfoVersion){
        this->personalMessageInfoVersion = personalMessageInfoVersion;
        addUpdatedPersonalInfoProperty(IM::PI_PersonalMessageInfoVersion, QString::number(personalMessageInfoVersion), true);
    }
    void setPersonalMessage(const QString &personalMessage){
        this->personalMessage = personalMessage;
        addUpdatedPersonalInfoProperty(IM::PI_PersonalMessage, "'" + personalMessage + "'", true);
    }

    void setPersonalContactGroupsVersion(quint32 personalContactGroupsVersion)
    {
        this->personalContactGroupsInfoVersion = personalContactGroupsVersion;
        addUpdatedPersonalInfoProperty(IM::PI_PersonalContactGroupsInfoVersion, QString::number(personalContactGroupsInfoVersion), true);

    }

    void setInterestGroupInfoVersion(quint32 interestGroupInfoVersion)
    {
        this->interestGroupInfoVersion = interestGroupInfoVersion;
        addUpdatedPersonalInfoProperty(IM::PI_InterestGroupsInfoVersion, QString::number(interestGroupInfoVersion), true);

    }

    
    void setPersonalSummaryInfoVersion(quint32 personalSummaryInfoVersion)
    {
        this->personalSummaryInfoVersion = personalSummaryInfoVersion;
    }
    void setPersonalDetailInfoVersion(quint32 personalDetailInfoVersion)
    {
        this->personalDetailInfoVersion = personalDetailInfoVersion;
    }

    void setSessionEncryptionKey(const QByteArray &sessionEncryptionKey)
    {
        this->sessionEncryptionKey = sessionEncryptionKey;
    }

    void setShortTalk(ShortTalk shortTalk)
    {
        this->shortTalk = shortTalk;
    }

    void setUserRole(int userRole)
    {
        this->userRole = userRole;
    }

    void setAccountState(AccountState state){
        this->accountState = state;
    }
    AccountState getAccountState() const{
        return accountState;
    }

    void setLastLoginDeviceInfo(const QString deviceInfo){
        this->lastLoginDeviceInfo = deviceInfo;
    }
    QString getLastLoginDeviceInfo() const{
        return this->lastLoginDeviceInfo;
    }

    void setSocketID(int socketID){this->m_socketID = socketID;}
    int getSocketID(){return m_socketID;}
    bool isConnected(){return m_socketID != INVALID_SOCK_ID;}




private:
    void init();
    


public slots:
    void setPersonalInfoString(const QString &personalInfoString, bool summaryInfo);
    QString getPersonalInfoString(bool requestSummaryInfo) const;

    void setContactGroupsInfoString(const QString &contactGroupsInfo);
    virtual QString getContactGroupsInfoString() const;


    QList<ContactGroupBase *> getContactGroups(bool noStrangers = true, bool noBlacklisted = true);
    QStringList friendContactGroupNames();
    ContactGroupBase * getContactGroup(int personalContactGroupID);
    ContactGroupBase * getContactGroup(const QString &groupName);
    quint32 getContactGroupID(const QString &groupName);
    QString getContactGroupName(int groupID) const;

    ContactGroupBase * addContactGroup(int contactGroupID, const QString &groupName);
    void deleteContactGroup(quint32 contactGroupID);
    void renameContactGroup(quint32 contactGroupID, const QString &newGroupName);

    QString groupNameThatContactBelongsTo(const QString &contactID) const;
    int groupIDThatContactBelongsTo(const QString &contactID);

    bool hasContactGroup(quint32 groupID);
    bool hasContactGroup(const QString &groupName);

    QStringList getContactGroupMembers(int groupID);
    int countOfContactGroupMembers(int groupID);


    virtual QStringList getAllContacts(bool noStrangers = true, bool noBlacklisted = true) const;
    bool hasFriendContact(const QString &contactID);
    bool hasContact(const QString &contactID);

    bool addNewContact(const QString &contactID, int groupID);
    bool deleteContact(const QString &contactID);
    bool moveFriendContact2(const QString &contactID, int oldGroupID, int newGroupID);
    void moveContactToAnotherGroup(const QString &contactID, int oldGroupID, int newGroupID);


//    void joinOrLeaveInterestGroup(quint32 interestGroupID, bool join = true);
//    bool isMemberOfInterestGroup(quint32 interestGroupID);
    quint32 updateInterestGroupInfoVersion();


    bool addOrDeleteBlacklistedContact(const QString &contactID, bool addToBlacklist = true);
    quint32 updatePersonalMessageInfoVersion();

    bool isContactBlacklisted(const QString &contactID);
    QStringList blacklistedContacts();

    int getUnusedContactGroupID();

    //    QString groupNameThatContactBelongsTo(const QString &contactID) const;
    
    void ensureDefaultGroups();
    ContactGroupBase * friendsGroup() const;
    ContactGroupBase * strangersGroup() const;
    ContactGroupBase * blacklistGroup() const;


private:


    ContactGroupBase *m_friendsGroup;
    ContactGroupBase *m_strangersGroup;
    ContactGroupBase *m_blacklistGroup;

    QHash<int/*Group ID*/, ContactGroupBase *> personalContactGroupsHash;



    quint32 personalMessageInfoVersion;
    QString personalMessage;


//    QList<quint32/*Group ID*/> interestGroups;
    quint32 interestGroupInfoVersion;
    
    quint32 personalSummaryInfoVersion;
    quint32 personalDetailInfoVersion;
    quint32 personalContactGroupsInfoVersion;


    IM::OnlineState onlineState;
    FriendshipApply friendshipApply;
    ShortTalk shortTalk;
    int userRole;
    AccountState accountState;

    QByteArray sessionEncryptionKey;
    
    QHash<IM::PropertyIDOfUser/*Property ID*/, QString/*SQL Update Statement*/> updatedSummaryInfoProperties;
    QHash<IM::PropertyIDOfUser/*Property ID*/, QString/*SQL Update Statement*/> updatedDetailInfoProperties;
    QMutex *updatedPropertiesMutex;

    QString lastLoginDeviceInfo;

    int m_socketID;



};

} //namespace HEHUI

#endif /* IMUSERBASE_H_ */
