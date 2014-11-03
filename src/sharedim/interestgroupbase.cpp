/*
 * groupbase.cpp
 *
 *  Created on: 2011-2-28
 *      Author: hehui
 */


#include <QVariant>
#include <QDebug>

#include "interestgroupbase.h"

namespace HEHUI {

InterestGroupBase::InterestGroupBase(quint32 groupID, const QString &groupName, QObject *parent)
    :IMGroupBase(groupID, groupName, parent)
{
	
    m_groupType = Group_UserCreated;
    parentGroupID = 0;
    creationTime = QDateTime();

    description = "";
    announcement = "";
    remark = "";
    state = 1;
    privacy = Allow_Anyone_To_Join;
        
}

InterestGroupBase::~InterestGroupBase() {
	// TODO Auto-generated destructor stub
}

void InterestGroupBase::setGroupInfoString(const QString &infoString){
    if(infoString.trimmed().isEmpty()){
        return;
    }
    
    QStringList infoList = infoString.split(QString(PACKET_DATA_SEPARTOR));

    setGroupID(infoList.at(0).toUInt());
    setGroupTypeID(infoList.at(1).toUInt());
    setParentGroupID(infoList.at(2).toUInt());
    setCreatorID(infoList.at(3));
    setGroupName(infoList.at(4));
    setCreationTime(QDateTime::fromString(infoList.at(5), "yyyy.MM.dd hh:mm:ss"));
    setGroupInfoVersion(infoList.at(6).toUInt());
    setGroupMemberListInfoVersion(infoList.at(7).toUInt());
    setDescription(infoList.at(8));
    setAnnouncement(infoList.at(9));
    setRemark(infoList.at(10));

    
}

QString InterestGroupBase::getGroupInfoString(){
    QStringList infoList;
    infoList << QString::number(getGroupID())
             << QString::number(m_groupType) << QString::number(parentGroupID)
             << getCreatorID() << getGroupName() << creationTime.toString("yyyy.MM.dd hh:mm:ss")
             <<QString::number(getGroupInfoVersion()) <<QString::number(getGroupMemberListInfoVersion())
             << getDescription() << getAnnouncement() <<getRemark()
                ;
    
    return infoList.join(QString(PACKET_DATA_SEPARTOR));    
    
}

void InterestGroupBase::addMember(const QString &memberuserID, MemberRole memberRole){
    qDebug()<<"--InterestGroupBase::addMember(...) "<<" memberuserID:"<<memberuserID;

    membersHash.insert(memberuserID, memberRole);
}

void InterestGroupBase::deleteMember(const QString &memberuserID){

    membersHash.remove(memberuserID);
}

bool InterestGroupBase::hasMember(const QString &memberUserID){
    QList<QString> members = membersHash.keys();
    return members.contains(memberUserID);
}

QStringList InterestGroupBase::members() const{
    return membersHash.keys();
}

InterestGroupBase::MemberRole InterestGroupBase::memberRole(const QString &memberUserID){
    return membersHash.value(memberUserID);
}

QStringList InterestGroupBase::getAdmins(bool includeCreator){
    QStringList admins = membersHash.keys(Role_Administrator);
    if(includeCreator){
        admins.append(membersHash.keys(Role_Creator));
    }

    return admins;
}

bool InterestGroupBase::isAdmin(const QString &memberUserID){
    MemberRole role = membersHash.value(memberUserID);
    return ( (role == Role_Administrator) || role == Role_Creator );
}

bool InterestGroupBase::isCreator(const QString &memberUserID){
    MemberRole role = membersHash.value(memberUserID);
    return ( role == Role_Creator );
}







}
