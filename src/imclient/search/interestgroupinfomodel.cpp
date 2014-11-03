
/*
 ***************************************************************************
 * Last Modified On: 2013-9-20
 * Last Modified By: 贺辉
 ***************************************************************************
*/


#include <QStringList>

#include "interestgroupinfomodel.h"
#include "../imageresource.h"



namespace HEHUI{


GroupInfo::GroupInfo(quint32 groupID, const QString groupName){
    this->groupID = groupID;
    this->groupName =groupName;
}

bool GroupInfo::isNull(){
    return !groupID;
}

InterestGroupInfoModel::InterestGroupInfoModel(QObject *parent) :
    QAbstractTableModel(parent)
{


}

InterestGroupInfoModel::~InterestGroupInfoModel(){
    groups.clear();
}

void InterestGroupInfoModel::setGroupsInfo(const QString &groupsListString)
{
    beginResetModel();

    groups.clear();

    QStringList groupsList = groupsListString.split(QChar(SEPARTOR_GROUP));
    foreach (QString groupInfoString, groupsList) {
        QStringList list = groupInfoString.split(QChar(SEPARTOR_RECORD));
        if(list.size() < 4){continue;}
        GroupInfo info;
        info.groupID = list.at(0).toUInt();
        info.groupName = list.at(1);
        info.creator = list.at(2);
        info.privacy = list.at(3).toUInt();

        groups.append(info);
    }

    endResetModel();
}

int InterestGroupInfoModel::rowCount ( const QModelIndex & parent) const {
    if(parent.isValid()){
        return 0;
    }
    return groups.size();

}

int InterestGroupInfoModel::columnCount ( const QModelIndex & parent) const{
    if(parent.isValid()){
        return 0;
    }

    return 3;

}

QVariant InterestGroupInfoModel::data ( const QModelIndex & index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }

    int row = index.row();
    if((row < 0) || (row >= groups.size())){
        return QVariant();
    }

    GroupInfo info = groups.at(row);

    if(role == Qt::DisplayRole || role == Qt::EditRole){
        switch (index.column()) {
        case 0:
            return info.groupID;
            break;
        case 1:
            return info.groupName;
            break;
        case 2:
            return info.creator;
            break;
        default:
            return QVariant();
        }
    }else if(role == Qt::DecorationRole){
        switch (index.column()) {
        case 0:
            return ImageResource::createIconForInterestGroup();
            break;
        default:
            return QVariant();
        }

    }


    return QVariant();



}

QVariant InterestGroupInfoModel::headerData ( int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    if(orientation ==  Qt::Horizontal){
        switch (section) {
        case 0:
            return QString(tr("Group ID"));
            break;
        case 1:
            return QString(tr("Group Name"));
            break;
        case 2:
            return QString(tr("Creator"));
            break;
        case 3:
            return QString(tr(""));
            break;
        default:
            return QVariant();
            break;
        }

    }

    return QVariant();

}

GroupInfo InterestGroupInfoModel::getGroupInfo(int row){
    return groups.at(row);
}








} //namespace HEHUI
