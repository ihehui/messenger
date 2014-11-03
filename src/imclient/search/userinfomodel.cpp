/*
 ****************************************************************************
 * userinfomodel.cpp
 *
 * Created On: 2010-5-24
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
 * Last Modified On: 2010-5-24
 * Last Modified By: 贺辉
 ***************************************************************************
 */




#include "userinfomodel.h"
#include "../imageresource.h"




namespace HEHUI {

ContactInfo::ContactInfo(QString userID){
    this->userID = userID;
}

bool ContactInfo::isNull(){
    return userID.trimmed().isEmpty();
}




UserInfoModel::UserInfoModel(QObject *parent)
	:QAbstractTableModel(parent)
{
	// TODO Auto-generated constructor stub

}

UserInfoModel::~UserInfoModel() {
    setUsersInfoString("");
}

void UserInfoModel::setUsersInfoString(const QString &usersInfoString){

    //TODO
    //FORMAT:UserID,NickName,Gender,Age,Face,FriendshipApply,BusinessAddress,OnlineState



    beginResetModel();

    contacts.clear();

    QStringList usersList = usersInfoString.split(QChar(SEPARTOR_GROUP));
    foreach (QString userInfo, usersList) {
        QStringList userInfoList = userInfo.split(QChar(SEPARTOR_RECORD));
        if(userInfoList.size() != 8){continue;}

        ContactInfo contactInfo;
        contactInfo.userID = userInfoList.at(0);
        contactInfo.nickName = userInfoList.at(1);
        contactInfo.gender = Contact::Gender(userInfoList.at(2).toUInt());
        contactInfo.age = userInfoList.at(3).toUInt();
        contactInfo.face = userInfoList.at(4);
        contactInfo.friendshipApply = IMUserBase::FriendshipApply(userInfoList.at(5).toUInt());
        contactInfo.businessAddress = userInfoList.at(6);
        contactInfo.onlineState = IM::OnlineState(userInfoList.at(7).toUInt());

        contacts.append(contactInfo);
    }


    endResetModel();

}

ContactInfo UserInfoModel::getContactInfo(int row){
    return contacts.at(row);
}

int UserInfoModel::rowCount ( const QModelIndex & parent) const {
    if(parent.isValid()){
        return 0;
    }
    return contacts.size();
    
}

int UserInfoModel::columnCount ( const QModelIndex & parent) const{
    if(parent.isValid()){
        return 0;
    }
    
    return 5;
    
}

QVariant UserInfoModel::data ( const QModelIndex & index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    
    int row = index.row();
    if((row < 0) || (row >= contacts.size())){
        return QVariant();
    }
    
    ContactInfo info = contacts.at(row);
    if(info.isNull()){
        return QVariant();
    }
    
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        switch (index.column()) {
        case 0:
            return info.userID;
            break;
        case 1:
            return info.nickName;
            break;
        case 2:
            return info.age;
            break;
        case 3:
            return info.gender;
            break;
        case 4:
            return info.businessAddress;
            break;
        default:
            return QVariant();
        }
    }else if(role == Qt::DecorationRole){
        switch (index.column()) {
        case 0:
            return ImageResource::createIconForContact(info.face, info.onlineState);
            break;
        default:
            return QVariant();
        }
    
    }
    
    
    return QVariant();



}

QVariant UserInfoModel::headerData ( int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    
    if(orientation ==  Qt::Horizontal){
        switch (section) {
        case 0:
            return QString(tr("User ID"));
            break;
        case 1:
            return QString(tr("Nick Name"));
            break;
        case 2:
            return QString(tr("Age"));
            break;
        case 3:
            return QString(tr("Gender"));
            break;
        case 4:
            return QString(tr("Location"));
            break;
        default:
            return QVariant();
            break;
        }
        
    }
    
    return QVariant();

}








}
