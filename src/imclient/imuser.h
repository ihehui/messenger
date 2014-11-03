/*
 ****************************************************************************
 * imuser.h
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

#ifndef IMUSER_H_
#define IMUSER_H_

#include "../sharedim/imuserbase.h"


namespace HEHUI {

class  IMUser: public IMUserBase {
public:
    static IMUser * instance();
    virtual ~IMUser();


    QString databaseColumnName(IM::PropertyIDOfUser propertyID) const;


    bool loadMyInfoFromLocalDatabase();
    bool saveMyInfoToLocalDatabase();

    bool setContactInfoString(const QString &contactID, const QString &contactInfoString, bool summaryInfo);
    bool saveContactInfoToLocalDatabase(const QString &contactID);

    IM::OnlineState getStateAfterLoggedin() const;
    void setStateAfterLoggedin(IM::OnlineState state);


    void setLoginServerAddress(const QString &serverAddress);
    QString getLoginServerAddress() const;
    void setLoginServerPort(quint16 serverPort);
    quint16 getLoginServerPort() const;
    void setRegistrationServerAddressInfo(const QString &regServerAddress);
    QString getRegistrationServerAddressInfo() const;

    void setFileServerAddress(const QString &serverAddress);
    QString getFileServerAddress();
    void setFileServerPort(quint16 serverPort);
    quint16 getFileServerPort();

    void showStrangers(bool show);
    bool isStrangersShown();

    void setAutoDownloadImageFromContact(bool autoDownload);
    bool isAutoDownloadImageFromContact();

    void setSyncAllChatMessagesToServer(bool sync);
    bool isSyncAllChatMessagesToServer();



private:
    IMUser(const QString & userID = "", QObject *parent = 0);


public slots:
    //        bool addGroup(const QString &groupName, const QStringList &members = QStringList());
    //        bool removeGroup(const QString &groupName);
    //        bool updateGroupName(const QString &oldGroupName, const QString &newGroupName);
    //        bool addContactToGroup(const QString &contactID, const QString &groupName);


    bool isFriendContact(const QString &contactID);


private slots:


private:
    static IMUser *imUserInstance;

    IM::OnlineState stateAfterLoggedin;

    QString m_loginServerAddress;
    quint16 m_loginServerPort;
    QString m_regServerAddress;

    QString m_fileServerAddress;
    quint16 m_fileServerPort;

    bool m_showStrangers;
    bool m_autoDownloadImageFromContact;
    bool m_syncAllChatMessagesToServer;



};

}

#endif /* IMUSER_H_ */
