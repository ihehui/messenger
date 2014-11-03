/*
 * groupbase.cpp
 *
 *  Created on: 2011-2-28
 *      Author: hehui
 */


#include <QVariant>
#include <QDebug>

#include "imgroupbase.h"

namespace HEHUI {

IMGroupBase::IMGroupBase(int groupID, const QString &groupName, QObject *parent)
    :QObject(parent), groupID(groupID), groupName(groupName)
{
	

    creatorID = "";
    groupInfoVersion = 0;
    memberListInfoVersion = 0;
    
    
    updatedPropertiesMutex =new QMutex();
    
}

IMGroupBase::~IMGroupBase() {
	// TODO Auto-generated destructor stub
}

bool IMGroupBase::isNull(){
    return ( (groupName.isEmpty()) || (creatorID.isEmpty()) );
}

QString IMGroupBase::getUpdateSQLStatement() const{
    QMutexLocker locker(updatedPropertiesMutex);
    
    QStringList sqlstatements;
    QList<IM::PropertyIDOfGroup> keys = updatedProperties.keys();
    foreach (IM::PropertyIDOfGroup propertyID, keys) {
        QString propertyName = databaseColumnName(propertyID);
        if(propertyName.isEmpty()){
            qCritical()<<QString("Unknown Property ID '%1'!").arg(propertyID);
            continue;
        }
        sqlstatements.append(QString(propertyName + "=" + updatedProperties.value(propertyID)));
    }
    
    return sqlstatements.join(" , ");
    
}

void IMGroupBase::addUpdatedProperty(IM::PropertyIDOfGroup propertyID, const QString &value){

    QMutexLocker locker(updatedPropertiesMutex);
    updatedProperties.insert(propertyID, value);
    
}

void IMGroupBase::clearUpdatedProperties(){
    QMutexLocker locker(updatedPropertiesMutex);
    
    updatedProperties.clear();
    
}

QString IMGroupBase::databaseColumnName(IM::PropertyIDOfGroup propertyID) const{
    
    QString columnName = "";
    switch(propertyID){
    case IM::PIG_GroupID:
        columnName = "GroupID";
        break;
    case IM::PIG_GroupTypeID:
        columnName = "TypeID";
        break;
    case IM::PIG_ParentGroupID:
        columnName = "ParentGroup";
        break;
    case IM::PIG_CreatorID:
        columnName = "Creator";
        break;
    case IM::PIG_GroupName:
        columnName = "GroupName";
        break;
    case IM::PIG_CreationTime:
        columnName = "CreationTime";
        break;
    case IM::PIG_GroupInfoVersion:
        columnName = "GroupInfoVersion";
        break;
        
    case IM::PIG_MemberListInfoVersion:
        columnName = "MemberListVersion";
        break;
    case IM::PIG_Description:
        columnName = "Description";
        break;
    case IM::PIG_Announcement:
        columnName = "Announcement";
        break;
    case IM::PIG_Remark :
        columnName = "Remark";
        break;
    case IM::PIG_State :
        columnName = "State";
        break;
    case IM::PIG_Privacy :
        columnName = "Privacy";
        break;

        
    default:
        columnName = "";

    }

    return columnName;

}














}
