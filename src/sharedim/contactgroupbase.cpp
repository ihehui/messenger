#include "contactgroupbase.h"



namespace HEHUI {

QString ContactGroupBase::Group_Blacklist_Name = QString(tr("Blacklist"));
QString ContactGroupBase::Group_Friends_Name = QString(tr("Friends"));
QString ContactGroupBase::Group_Strangers_Name = QString(tr("Strangers"));

ContactGroupBase::ContactGroupBase(quint32 groupID, const QString &groupName, QObject *parent)
    :IMGroupBase(groupID, groupName, parent)
{


}

bool ContactGroupBase::isUserCreatedGroup(int groupID){

    if(groupID == Group_Blacklist_ID || (groupID == Group_Strangers_ID) || (groupID == Group_Friends_ID)){
        return false;
    }

    return true;

}

//bool ContactGroupBase::setGroupInfoFromString(const QString &infoString, const QString &fieldSepartor){
//    if(infoString.trimmed().isEmpty()){
//        return false;
//    }

//    QStringList infoList = infoString.split(fieldSepartor);
//    if(infoList.size() != 2){
//        return false;
//    }

//    setGroupID(infoList.at(0).toUInt());
//    setGroupName(infoList.at(1));
////    setGroupInfoVersion(infoList.at(2).toUInt());
////    setGroupMemberListInfoVersion(infoList.at(3).toUInt());

//    return true;
//}

//QString ContactGroupBase::getGroupInfoAsString(const QString &fieldSepartor){
//    QStringList infoList;
//    infoList << QString::number(getGroupID())
//             << getGroupName()
////             << QString::number(getGroupInfoVersion())
////             <<QString::number(getGroupMemberListInfoVersion())
//                ;

//    return infoList.join(fieldSepartor);

//}

void ContactGroupBase::setMembersFromString(const QString &infoString, const QString &fieldSepartor){
    membersList = infoString.split(fieldSepartor);
}

QString ContactGroupBase::getMembersAsString(const QString &fieldSepartor){
    return membersList.join(fieldSepartor);
}

//QStringList ContactGroupBase::getMembers(){
//    return membersList;
//}

void ContactGroupBase::addMember(const QString &memberuserID){
    if(membersList.contains(memberuserID)){return;}
    membersList.append(memberuserID);
    updateMemberListInfoVersion();
}

void ContactGroupBase::deleteMember(const QString &memberuserID){
    if(!membersList.contains(memberuserID)){return;}
    membersList.removeAll(memberuserID);
    updateMemberListInfoVersion();
}

bool ContactGroupBase::hasMember(const QString &memberUserID){

    return membersList.contains(memberUserID, Qt::CaseInsensitive);
}

QStringList ContactGroupBase::members() const{
    return membersList;
}

void ContactGroupBase::setMembers(const QStringList &members){
    membersList.clear();
    membersList.append(members);
}

int ContactGroupBase::countOfMembers(){
    return membersList.size();
}










} //namespace HEHUI
