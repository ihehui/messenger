
/*
 ***************************************************************************
 * Last Modified On: 2013-9-20
 * Last Modified By: 贺辉
 ***************************************************************************
 */


#ifndef INTERESTGROUPINFOMODEL_H
#define INTERESTGROUPINFOMODEL_H

#include <QAbstractTableModel>
#include <QList>


#include "contactsmanager/interestgroup.h"


namespace HEHUI {


struct GroupInfo{
    GroupInfo(quint32 groupID = 0, const QString groupName = "");
    bool isNull();

    quint32 groupID;
    QString groupName;
    QString creator;
    quint8 privacy;
};


class InterestGroupInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit InterestGroupInfoModel(QObject *parent = 0);
    ~InterestGroupInfoModel();


    void setGroupsInfo(const QString &groupsListString);

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const ;
    int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const ;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    GroupInfo getGroupInfo(int row);


signals:


private:
    QList<GroupInfo> groups;






};

} //namespace HEHUI

#endif // INTERESTGROUPINFOMODEL_H
