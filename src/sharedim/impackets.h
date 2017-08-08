#ifndef IMPACKETS_H_
#define IMPACKETS_H_

#include "HHSharedNetwork/PacketBase"

#include "sharedimlib.h"

namespace HEHUI
{


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API IMPacket : public Packet
{
public:
    IMPacket(const QByteArray &sessionEncryptionKey = QByteArray());
    IMPacket(quint8 packetType, quint8 packetSubType, const QByteArray &sessionEncryptionKey);
//    IMPacket(const PacketBase &base, quint8 packetType, const QByteArray &sessionEncryptionKey);
    ~IMPacket();

public:
    void setSessionEncryptionKey(const QByteArray &key);
    QByteArray getSessionEncryptionKey() const;

    bool decryptionSucceeded() const;

private:
//    virtual void init(); //Overwride Required
//    virtual void parsePacketBody(QByteArray &packetBody); //Overwride Required
    virtual QByteArray packBodyData() = 0;



protected:
    QByteArray encrypt(const QByteArray &data);
    QByteArray decrypt(const QByteArray &encryptedData);

private:
    QByteArray sessionEncryptionKey;
    bool m_decryptionSucceeded;

};
////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API ServerDiscoveryPacket : public IMPacket
{
public:
    enum ServerType{
        SERVER_GATE = UserDefinedPacket + 1,
        SERVER_LOGIN,
        SERVER_CHAT,
        SERVER_IMAGE,
        SERVER_FILE,

        SERVER_ALL_IN_ONE = 255
    };

    ServerDiscoveryPacket(ServerType packetSubType = SERVER_ALL_IN_ONE);
    ServerDiscoveryPacket(ServerType packetSubType, const QByteArray &sessionEncryptionKey);
    ServerDiscoveryPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey = QByteArray());
    ~ServerDiscoveryPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:


    quint8 serverAnnouncement;

    QString version;
    quint32 ip;
    quint16 udpPort;
    quint16 rtpPort;
    quint16 tcpPort;
    int serverInstanceID;

};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API DataForwardPacket : public IMPacket
{
public:
    DataForwardPacket();
    DataForwardPacket(const QByteArray &sessionEncryptionKey);
    DataForwardPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~DataForwardPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    QByteArray data;
    QString receiver;
    quint8 isRequest;

};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API AnnouncementPacket : public IMPacket
{
public:
    enum PacketInfoType {
        ANNOUNCEMENT_UNKNOWN = UserDefinedPacket + 1,
        ANNOUNCEMENT_QUERY,
        ANNOUNCEMENT_CREATE,
        ANNOUNCEMENT_UPDATE,
        ANNOUNCEMENT_REPLY,
        ANNOUNCEMENT_QUERY_TARGETS,
        ANNOUNCEMENT_INFO
    };

    AnnouncementPacket(PacketInfoType packetSubType = ANNOUNCEMENT_UNKNOWN);
    AnnouncementPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    AnnouncementPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~AnnouncementPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    enum AnnouncementTarget {
        ANNOUNCEMENT_TARGET_EVERYONE = 0,
        ANNOUNCEMENT_TARGET_SPECIFIC = 1,
        ANNOUNCEMENT_TARGET_ALL = 255
    };

    quint32 JobID;

    struct QueryInfoStruct {
        QString announcementID;
        QString keyword;
        QString validity;
        QString assetNO;
        QString userName;
        QString target;
        QString startTime;
        QString endTime;
    } QueryInfo;

    struct CreateInfoStruct {
        unsigned int localTempID;
        QString adminID;
        quint8 type;
        QString content;
        quint8 confirmationRequired;
        int validityPeriod;
        quint8 targetType;
        QString targets;
    } CreateInfo;

    struct UpdateInfoStruct {
        QString adminName;
        unsigned int announcementID;
        quint8 targetType;
        quint8 active;
        QString addedTargets;
        QString deletedTargets;
    } UpdateInfo;

    struct ReplyInfoStruct {
        unsigned int announcementID;
        QString sender;
        QString receiver;
        QString receiversAssetNO;
        QString replyMessage;
    } ReplyInfo;

    struct QueryTargetsInfoStruct {
        unsigned int announcementID;
    } QueryTargetsInfo;

    struct AnnouncementInfoStruct {
        unsigned int announcementID;
        QString content;
    } AnnouncementInfo;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API RgeistrationPacket : public IMPacket
{
public:
    enum PacketInfoType {
        REGISTRATION_UNKNOWN = UserDefinedPacket + 1,
        REGISTRATION_SERVER_INFO,
        REGISTRATION_INFO,
        REGISTRATION_RESULT
    };

    RgeistrationPacket(PacketInfoType packetSubType = REGISTRATION_UNKNOWN);
    RgeistrationPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    RgeistrationPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~RgeistrationPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    enum RgeistrationMode {
        REG_MODE_USER_CREATE_ALL,
        REG_MODE_SERVER_CREATE_ALL,
        REG_MODE_SERVER_CREATE_ID,
        REG_MODE_SERVER_CREATE_PASSWORD,
        REG_MODE_SERVER_HTTP,
        REG_MODE_CLOSED
    };
    enum ActivtionMode {ACT_MODE_AUTO, ACT_MODE_MANUAL, ACT_MODE_EMAIL};

    quint32 JobID;

    struct ServerInfoStruct {
        quint8 requestServerInfo;
        QString version;
        quint8 regMode;
        QString message;
        QString address;
    } ServerInfo;

    struct RgeistrationInfoStruct {
        QString userID;
        QString password;
        QString email;
        QString captcha;
        QString securityQuestion;
        QString securityAnswer;
    } RgeistrationInfo;

    struct RgeistrationResultStruct {
        quint32 sysID;
        QString userID;
        QString password;
        quint8 errorCode;
        quint8 activationMode;
    } RgeistrationResult;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API UpdatePasswordPacket : public IMPacket
{
public:
    enum PacketInfoType {
        INFO_TYPE_UNKNOWN = UserDefinedPacket + 1,
        INFO_TYPE_INIT_REQUEST,

        INFO_TYPE_CAPTCHA_REQUEST,
        INFO_TYPE_CAPTCHA_IMAGE,

        INFO_TYPE_AUTH_INFO_FROM_SERVER,
        INFO_TYPE_AUTH_INFO_FROM_CLIENT,
        INFO_TYPE_AUTH_RESULT,
        INFO_TYPE_UPDATE_RESULT
    };

    UpdatePasswordPacket(PacketInfoType packetSubType = INFO_TYPE_UNKNOWN);
    UpdatePasswordPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    UpdatePasswordPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~UpdatePasswordPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    enum AuthMode {
        AUTH_OLD_PWD_ONLY = 0,
        AUTH_SECURITY_QUESTION,
        AUTH_EMAIL,
        AUTH_SMS,
        AUTH_HTTP
    };

    quint32 JobID;

    struct AuthInfoStruct {
        quint8 authMode;
        QByteArray captchaImage;
        QString captcha;

        QString userID;
        QString oldPassword;
        QString newPassword;
        QString securityQuestion;
        QString securityAnswer;
        QString email;
        quint32 cellphoneNO;
        QString smsCaptcha;
        QString url;

        quint8 authResult;
        quint8 updateResult;
    } AuthInfo;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API LoginPacket : public IMPacket
{
public:
    enum PacketInfoType {
        INFO_TYPE_UNKNOWN = UserDefinedPacket + 1,

//        INFO_TYPE_LOGIN_SERVER_INFO,

        INFO_TYPE_AUTH_INFO_FROM_CLIENT,

        INFO_TYPE_LOGIN_SUCCEEDED,
        INFO_TYPE_LOGIN_FAILED,

        INFO_TYPE_PREVIOUS_LOGIN_INFO
    };

    LoginPacket(PacketInfoType packetSubType = INFO_TYPE_UNKNOWN);
    LoginPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    LoginPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~LoginPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:

    quint32 JobID;

    struct LoginServerInfoStruct {
        QString version;
        QString serverAddress;
        quint16 serverPort;
    } LoginServerInfo;

    struct AuthInfoStruct {
        QByteArray password;
        quint8 stateAfterLoggedin;
        QString deviceInfo;
    } AuthInfo;

    struct AuthResultStruct {
        quint8 loggedin;
        quint8 errorType;

        QByteArray sessionEncryptionKey;
        quint32 personalSummaryInfoVersionOnServer;
        quint32 personalDetailInfoVersionOnServer;
        quint32 personalContactGroupsInfoVersionOnServer;
        quint32 interestGroupsInfoVersionOnServer;
        quint32 personalMessageInfoVersionOnServer;
        uint serverTime;
    } AuthResultInfo;

    struct PreviousLoginInfoStruct {
        QString loginIP;
        uint loginTime;
        uint logoutTime;
        QString clientVersion;
        QString deviceInfo;
    } PreviousLoginInfo;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API OnlineStateChangedPacket : public IMPacket
{
public:
    OnlineStateChangedPacket();
    OnlineStateChangedPacket(const QByteArray &sessionEncryptionKey);
    OnlineStateChangedPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~OnlineStateChangedPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint8 stateCode;
    QString contactID;
    QString contactIP;
    quint16 contactPort;

};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API OnlineContacts : public IMPacket
{
public:
    OnlineContacts();
    OnlineContacts(const QByteArray &sessionEncryptionKey);
    OnlineContacts(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~OnlineContacts();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    QString contactsOnlineInfoString;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API ContactGroupsInfoPacket : public IMPacket
{
public:
    enum PacketInfoType {
        PIT_UNKNOWN = UserDefinedPacket + 1,

        PIT_GROUPS_LIST,

        PIT_GROUP_CHANGE_PARENT,
        PIT_GROUP_CREATION,
        PIT_GROUP_DELETION,
        PIT_GROUP_RENAMING,

    };

    ContactGroupsInfoPacket(PacketInfoType packetSubType = PIT_UNKNOWN);
    ContactGroupsInfoPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    ContactGroupsInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~ContactGroupsInfoPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint32 JobID;

    struct GroupsListStruct {
        QString groupsInfo;
        quint32 version;
        QString contactInfoVersionList;
    } GroupsList;

    struct GroupParentInfoStruct {
        quint32 groupID;
        quint32 parentID;
    } GroupParentInfo;

    struct GroupCreationInfoStruct {
        QString  name;
        quint32 parentID;
        quint32 groupID;
        quint8 errorCode;
    } GroupCreationInfo;

    struct GroupDeletionInfoStruct {
        quint32 groupID;
        quint8 errorCode;
    } GroupDeletionInfo;

    struct GroupRenamingInfoStruct {
        quint32 groupID;
        QString  newName;
    } GroupRenamingInfo;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API InterestGroupsInfoPacket : public IMPacket
{
public:
    enum PacketInfoType {
        PIT_UNKNOWN = UserDefinedPacket + 1,

        PIT_GROUPS_LIST,
        PIT_GROUP_INFO,

        PIT_GROUP_UPDATE_ANNOUNCEMENT,

        PIT_GROUP_CREATION,
        PIT_GROUP_DELETION,

        PIT_GROUP_MEMBER_APPLICATION,
        PIT_GROUP_MEMBER_DELETION,
        PIT_GROUP_MEMBER_PROMOTION
    };

    InterestGroupsInfoPacket(PacketInfoType packetSubType = PIT_UNKNOWN);
    InterestGroupsInfoPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    InterestGroupsInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~InterestGroupsInfoPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint32 JobID;
    quint32 GroupID;

    struct GroupsListStruct {
        QString groups;
        quint32 version;
    } GroupsList;

    struct GroupInfoStruct {
//        quint32 id;
        QString infoString;
//        QString name;
//        QString creator;
//        uint creationTime;
//        quint8 type;
//        quint32 infoVersion;
//        QString members;
//        QString admins;
    } GroupInfo;

    struct GroupAnnouncementInfoStruct {
        QString content;
        QString admin;
    } GroupAnnouncementInfo;

    struct GroupCreationInfoStruct {
        QString  name;
        quint8 type;
    } GroupCreationInfo;

    struct GroupDeletionInfoStruct {
        quint8 deleted;
    } GroupDeletionInfo;


    struct MemberApplicationInfoStruct {
        QString userID;
        QString message;

        quint8 approved;
        QString admin;
    } MemberApplicationInfo;

    struct MemberDeletionInfoStruct {
        QString userID;
        quint8 blockForever;
        QString admin;
    } MemberDeletionInfo;

    struct MemberPromotionInfoStruct {
        QString userID;
        quint8 promoted;
    } MemberPromotionInfo;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API ContactInfoPacket : public IMPacket
{
public:
    enum PacketInfoType {
        PIT_UNKNOWN = UserDefinedPacket + 1,

        PIT_CONTACT_INFO,

        PIT_GROUP_CHANGE,

        PIT_FRIENDING_REQUEST,
        PIT_FRIENDING_RESULT,
        PIT_CONTACT_DELETION,
        PIT_CONTACT_REMARK,
    };

    ContactInfoPacket(PacketInfoType packetSubType = PIT_UNKNOWN);
    ContactInfoPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    ContactInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~ContactInfoPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint32 JobID;
    QString ContactID;


    struct ContactInfoStruct {
        quint8 isSummaryInfo;
        QString infoString;
    } info;

    struct ContactChangeGroupStruct {
        quint32 oldGroupID;
        quint32 newGroupID;
    } ContactChangeGroup;

    struct ContactGroupCreationOrDeletionStruct {
        quint32 groupID;
        QString groupName;
        quint8 errorCode;
        quint8 isCreation;
    } ContactGroupCreationOrDeletion;


    struct ContactFriendingRequestStruct {
        QString nickName;
        QString userFace;
        QString message;
        quint32 groupID;
    } ContactFriendingRequest;

    struct ContactFriendingResultStruct {
        quint8 errorCode;
        QString message;
    } ContactFriendingResult;

    struct ContactDeletionInfoStruct {
        quint8 deleteMeFromOpposition;
        quint8 blockForever;
        quint8 errorCode;
    } ContactDeletionInfo;

    struct ContactRemarkInfoStruct {
        QString  newRemarkName;
    } ContactRemarkInfo;


};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API SearchInfoPacket : public IMPacket
{
public:
    enum PacketInfoType {
        PIT_UNKNOWN = UserDefinedPacket + 1,

        PIT_SEARCH_CONTACT_CONDITIONS,
        PIT_SEARCH_CONTACT_RESULT,

        PIT_SEARCH_INTEREST_GROUP_CONDITIONS,
        PIT_SEARCH_INTEREST_GROUP_RESULT,

    };

    SearchInfoPacket(PacketInfoType packetSubType = PIT_UNKNOWN);
    SearchInfoPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    SearchInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~SearchInfoPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint32 JobID;

    struct SearchContactConditionsStruct {
        QString keyword;
        quint8 isOnline;
        quint8 hasWebcam;
        quint16 location;
        quint16 hometown;
        quint8 gender;
        quint8 age;

        quint8 matchExactly;
        int startIndex;

    } SearchContactConditions;

    struct SearchContactResultStruct {
        QString result;
    } SearchContactResult;

    struct SearchInterestGroupConditionsStruct {
        QString keyword;
        int startIndex;
    } SearchInterestGroupConditions;

    struct SearchInterestGroupResultStruct {
        QString result;
    } SearchInterestGroupResult;


};
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API ChatMessagePacket : public IMPacket
{
public:
    enum PacketInfoType {
        PIT_UNKNOWN = UserDefinedPacket + 1,

        PIT_CONTACT_CHAT_MESSAGE,
        PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER,
        PIT_CONTACT_CHAT_HISTORY_MESSAGES,

        PIT_GROUP_CHAT_MESSAGE,
        PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER,
        PIT_GROUP_CHAT_HISTORY_MESSAGES,

        PIT_CHAT_IMAGE,

        PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT

    };

    ChatMessagePacket(PacketInfoType packetSubType = PIT_UNKNOWN);
    ChatMessagePacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    ChatMessagePacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~ChatMessagePacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint32 JobID;

    struct ContactChatMessageStruct {
        QString contactID;
        QString message;
        QString imageNames;
    } ContactChatMessage;

    struct ContactChatMessageCachedOnServerStruct {
        QString messages;
    } ContactChatMessagesCachedOnServer;

    struct ContactChatHistoryMessagesStruct {
        QString contactID;
        QString messages;
        uint startime;
    } ContactChatHistoryMessages;

    struct GroupChatMessageStruct {
        quint32 groupID;
        QString memberID;
        uint time;
        QString message;
        QString imageNames;
    } GroupChatMessage;

    struct GroupChatMessagesCachedOnServerStruct {
        QString messages;
    } GroupChatMessagesCachedOnServer;

    struct GroupChatHistoryMessagesStruct {
        quint32 groupID;
        QString messages;
        uint startime;
    } GroupChatHistoryMessages;

    struct ChatImagesStruct {
        quint8 isRequest;
        QString contactID;
        QString name;
        QByteArray image;
    } ChatImage;

    struct SessionEncryptionKeyWithContactStruct {
        QString contactID;
        QByteArray key;
    } SessionEncryptionKeyWithContact;



};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API CaptchaInfoPacket : public IMPacket
{
public:
    enum PacketInfoType {
        PIT_UNKNOWN = UserDefinedPacket + 1,

        CAPTCHA_REQUEST,
        CAPTCHA_IMAGE,
        CAPTCHA_CODE,

        CAPTCHA_AUTH_RESULT,

    };

    CaptchaInfoPacket(PacketInfoType packetSubType = PIT_UNKNOWN);
    CaptchaInfoPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    CaptchaInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~CaptchaInfoPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public: 
    quint32 JobID;

    quint8 captchaID;
    QByteArray captchaImage;
    QString captchaCode;
    quint8 approved;



};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API FileTransferPacket : public IMPacket
{
public:
    enum PacketInfoType {
        FT_UNKNOWN = UserDefinedPacket + 1,

        FT_FILE_SERVER_INFO,

        FT_FileSystemInfoRequest,
        FT_FileSystemInfoResponse,

        FT_FileDeletingRequest,
        FT_FileDeletingResponse,

        FT_FileRenamingRequest,
        FT_FileRenamingResponse,

        FT_FileDownloadingRequest,
        FT_FileDownloadingResponse,

        FT_FileUploadingRequest,
        FT_FileUploadingResponse,

        FT_FileDataRequest,
        FT_FileData,

        FT_FileTXStatus,
        FT_FileTXError,

        FT_FileTXStop
    };

    FileTransferPacket(PacketInfoType packetSubType = FT_UNKNOWN);
    FileTransferPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    FileTransferPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    QString ContactID;

    struct FileServerInfoStruct {
        QString address;
        quint16 port;
    } FileServerInfo;

    struct FileSystemInfoRequestStruct {
        QString parentDirPath;
    } FileSystemInfoRequest;
    struct FileSystemInfoResponseStruct {
        QString baseDirPath;
        QByteArray fileSystemInfoData;
    } FileSystemInfoResponse;

    struct FileDeletingRequestStruct {
        QString baseDirPath;
        QStringList files;
    } FileDeletingRequest;
    struct FileDeletingResponseStruct {
        QString baseDirPath;
        QStringList failedFiles;
    } FileDeletingResponse;

    struct FileRenamingRequestStruct {
        QString baseDirPath;
        QString oldFileName;
        QString newFileName;
    } FileRenamingRequest;
    struct FileRenamingResponseStruct {
        QString baseDirPath;
        QString oldFileName;
        quint8 renamed;
        QString message;
    } FileRenamingResponse;

    struct FileDownloadingRequestStruct {
        QString baseDir;
        QString fileName;
        QString dirToSaveFile;
    } FileDownloadingRequest;
    struct FileDownloadingResponseStruct {
        quint8 accepted;
        QString baseDir;
        QString fileName;
        QByteArray fileMD5Sum;
        quint64 size;
        quint8 errorCode;
    } FileDownloadingResponse;

    struct FileUploadingRequestStruct {
        QString fileName;
        QByteArray fileMD5Sum;
        quint64 size;
        QString fileSaveDir;
    } FileUploadingRequest;
    struct FileUploadingResponseStruct {
        QByteArray fileMD5Sum;
        quint8 accepted;
        QString message;
    } FileUploadingResponse;

    struct FileDataRequestStruct {
        QByteArray fileMD5;
        int startPieceIndex;
        int endPieceIndex;
    } FileDataRequest;
    struct FileDataResponseStruct {
        QByteArray fileMD5;
        int pieceIndex;
        QByteArray data;
        QByteArray pieceMD5;
    } FileDataResponse;

    struct FileTXStatusStruct {
        QByteArray fileMD5;
        quint8 status;
    } FileTXStatus;

    struct FileTXErrorStruct {
        QString fileName;
        QByteArray fileMD5;
        quint8 errorCode;
        QString message;
    } FileTXError;


};
////////////////////////////////////////////////////////////////////////
























////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API JobProgressPacket : public IMPacket
{
public:
    JobProgressPacket(const QByteArray &sessionEncryptionKey);
    JobProgressPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~JobProgressPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    quint32 jobID;
    quint8 result;
    QVariant extraData;
};
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
class SHAREDIMLIB_API AdminLoginPacket : public IMPacket
{
public:
    enum PacketInfoType {
        LOGIN_UNKNOWN = UserDefinedPacket + 1,
        LOGIN_REQUEST,
        LOGIN_RESULT
    };


    AdminLoginPacket(PacketInfoType packetSubType, const QByteArray &sessionEncryptionKey);
    AdminLoginPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey);
    ~AdminLoginPacket();

private:
    void init();
    void parsePacketBody(QByteArray &packetBody);
    QByteArray packBodyData();

public:
    struct LoginInfoStruct {
        QString adminID;
        QString password;
        QString computerName;
    } LoginInfo;

    struct LoginResultStruct {
        quint8 loggedIn;
        QString message;
        quint8 readonly;
    } LoginResult;

};
////////////////////////////////////////////////////////////////////////








} //namespace HEHUI

#endif // IMPACKETS_H_
