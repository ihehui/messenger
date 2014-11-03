/*
 ****************************************************************************
 * contact.h
 *
 * Created On: 2010-7-3
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
 * Last Modified On: 2010-7-3
 * Last Modified By: 贺辉
 ***************************************************************************
 */



#ifndef CONTACT_H_
#define CONTACT_H_

#include <QObject>

#include "../../sharedim/constants_global_shared.h"
#include "../../sharedim/imuserbase.h"



namespace HEHUI {

class Contact: public HEHUI::IMUserBase {
    Q_OBJECT
public:
    Contact(QObject *parent = 0);
    Contact(const QString &contactID = "", QObject *parent = 0);
    Contact(const QString &contactID = "", const QString &nickname = "", QObject *parent = 0);
    virtual ~Contact();

    QString databaseColumnName(IM::PropertyIDOfUser propertyID) const;




    int getContactGroupID() const
    {
        return contactGroupID;
    }
    void setContactGroupID(int contactGroupID)
    {
        this->contactGroupID = contactGroupID;
        addUpdatedPersonalInfoProperty(IM::PI_ContactGroupID, QString::number(contactGroupID), true);
    }

    //    quint32 getInterestGroupID() const
    //    {
    //    	return interestGroupID;
    //    }
    //    void setInterestGroupID(quint32 interestGroupID)
    //    {
    //    	this->interestGroupID = interestGroupID;
    //        addUpdatedProperty(IM::PI_InterestGroupID, QString::number(interestGroupID));

    //    }

    QString getRemarkName(){
        return m_remarkName;
    }
    void setRemarkName(const QString &remarkName){
        //qDebug()<<"---setRemarkName()";
        this->m_remarkName = remarkName;
        addUpdatedPersonalInfoProperty(IM::PI_RemarkName, "'" + m_remarkName + "'", true);
    }


    void setShowRemarkName(bool show){m_showRemarkName = show;}
    QString displayName() const{
        if(m_showRemarkName && !m_remarkName.trimmed().isEmpty()){
            return m_remarkName;
        }
        QString nickName = getNickName();
        if(nickName.trimmed().isEmpty()){
            return getUserID();
        }else{
            return nickName;
        }
    }

    void setSocketID(int socketID){this->m_socketID = socketID;}
    int getSocketID(){return m_socketID;}


    bool isFriend();
    bool isStranger();
    bool isBlacklisted();

    void appandUnreadMessage(const QString &time, const QString &message);
    QMap<QString/*Time String*/, QString/*Message*/> takeUnreadMessages();


private:

    int contactGroupID;
    quint32 interestGroupID;
    quint32 systemGroupID;
    
    QString m_remarkName;
    bool m_showRemarkName;

    int m_socketID;

    QMap<QString/*Time String*/, QString/*Message*/> unreadMessages;




};

}

#endif /* CONTACT_H_ */
