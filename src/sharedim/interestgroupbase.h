/*
 * groupbase.h
 *
 *  Created on: 2011-2-28
 *      Author: hehui
 */

#ifndef INTERESTGROUPBASE_H_
#define INTERESTGROUPBASE_H_


#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QHash>
#include <QMutex>


#include "constants_global_shared.h"
#include "sharedimlib.h"
#include "imgroupbase.h"



namespace HEHUI {

class SHAREDIMLIB_API InterestGroupBase : public IMGroupBase {
    Q_OBJECT
public:

    enum GroupType{Group_SystemDefault=1, Group_UserCreated=2};
    enum MemberRole{Role_Creator =1, Role_Administrator=2, Role_Member=3};

    enum LogType{
        LOG_GroupCreated=1,
        Log_GroupDisbanded,
        LOG_ContactInvitedToJoin,
        Log_ContactRequestToJoin,
        LOG_ContactJoined,
        LOG_ContactQuit,
        LOG_ContactFired,
        LOG_ContactBanned,
        LOG_ContactPromoted,
        LOG_ContactDemoted,
        LOG_ContactRenamed
    };

    enum Privacy{
        Allow_Anyone_To_Join=1,
        Request_Verfication_To_Join,
        Only_Invited_Can_Join
    };


    InterestGroupBase(quint32 groupID = 0, const QString &groupName = "", QObject *parent = 0);
    virtual ~InterestGroupBase();
    
    void setGroupInfoString(const QString &infoString);
    QString getGroupInfoString();
    
    void addMember(const QString &memberuserID, MemberRole memberRole);
    void deleteMember(const QString &memberuserID);
    bool hasMember(const QString &memberUserID);
    QStringList members() const;
    MemberRole memberRole(const QString &memberUserID);
    QStringList getAdmins(bool includeCreator = true);
    bool isAdmin(const QString &memberUserID);
    bool isCreator(const QString &memberUserID);

    
    
    
    

    QString getAnnouncement() const
    {
        return announcement;
    }

    QDateTime getCreationTime() const
    {
        return creationTime;
    }

    QString getDescription() const
    {
        return description;
    }

    GroupType getGroupTypeID() const
    {
        return m_groupType;
    }

    QHash<QString,MemberRole> getMembersHash() const
    {
        return membersHash;
    }

    quint32 getParentGroupID() const
    {
        return parentGroupID;
    }

    QString getRemark() const
    {
        return remark;
    }

    quint8 getState(){
        return state;
    }

//    QDateTime getLastUpdateTime() const{
//        return lastUpdateTime;
//    }

    void setAnnouncement(QString announcement)
    {
        this->announcement = announcement;
        addUpdatedProperty(IM::PIG_Announcement, "'"+announcement+"'");
        
    }

    void setCreationTime(QDateTime creationTime)
    {
        this->creationTime = creationTime;
        addUpdatedProperty(IM::PIG_CreationTime, "'"+creationTime.toString("yyyy.MM.dd hh:mm:ss")+"'");
        
    }

    void setDescription(QString description)
    {
        this->description = description;
        addUpdatedProperty(IM::PIG_Description, "'"+description+"'");
        
    }


    void setGroupTypeID(quint32 groupTypeID)
    {
        this->m_groupType = GroupType(groupTypeID);
        addUpdatedProperty(IM::PIG_GroupTypeID, QString::number(groupTypeID));
        
    }


    void setMembersHash(QHash<QString,MemberRole> membersHash)
    {
        this->membersHash = membersHash;
    }

    void setParentGroupID(quint32 parentGroupID)
    {
        this->parentGroupID = parentGroupID;
        addUpdatedProperty(IM::PIG_ParentGroupID, QString::number(parentGroupID) );
        
    }

    void setRemark(QString remark)
    {
        this->remark = remark;
        addUpdatedProperty(IM::PIG_Remark, "'"+remark+"'");
        
    }

    void setState(quint8 state){
        this->state = state;
        addUpdatedProperty(IM::PIG_State, QString::number(state) );
    }

    void setPrivacy(quint8 privacy){
        this->privacy = Privacy(privacy);
        addUpdatedProperty(IM::PIG_Privacy, QString::number(privacy) );
    }
    quint8 getPrivacy(){
        return quint8(privacy);
    }



protected:

private:
    
    GroupType m_groupType;
    quint32 parentGroupID;
    QDateTime creationTime;
    
    QString description;
    QString announcement;
    QString remark;
    quint8 state;
    Privacy privacy;

//    QDateTime lastUpdateTime;


    QHash <QString/*Member's ID*/, MemberRole/*Member's Role*/> membersHash;


};

}

#endif /* INTERESTGROUPBASE_H_ */
