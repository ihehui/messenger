#include "impackets.h"

#include "constants_global_shared.h"
#include "HHSharedNetwork/PacketBase"
#include "HHSharedCore/hcryptography.h"


namespace HEHUI {


////////////////////////////////////////////////////////////////////////
IMPacket::IMPacket(quint8 packetType, const QByteArray &sessionEncryptionKey)
    :Packet(packetType)
{
    this->sessionEncryptionKey = sessionEncryptionKey;

    init();
}

IMPacket::IMPacket(const PacketBase &base, quint8 packetType, const QByteArray &sessionEncryptionKey)
    :Packet(packetType)
{
    this->sessionEncryptionKey = sessionEncryptionKey;

    init();
    convert(base);
}

IMPacket::~IMPacket(){

}

void IMPacket::setSessionEncryptionKey(const QByteArray &key){
    sessionEncryptionKey = key;
}

void IMPacket::init(){

}

QByteArray IMPacket::encrypt(const QByteArray &data){
    if(sessionEncryptionKey.isEmpty()){return data;}
    Cryptography cryptography;
    QByteArray encryptedData;
    if(cryptography.teaCrypto(&encryptedData, data, sessionEncryptionKey, true)){
        return encryptedData;
    }
    return QByteArray();
}

QByteArray IMPacket::decrypt(const QByteArray &encryptedData){
    if(sessionEncryptionKey.isEmpty()){return encryptedData;}
    Cryptography cryptography;
    QByteArray decryptedData;
    if(cryptography.teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false)){
        return decryptedData;
    }
    return QByteArray();
}


////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////
ServerDiscoveryPacket::ServerDiscoveryPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ServerDiscovery), sessionEncryptionKey)
{

}

ServerDiscoveryPacket::ServerDiscoveryPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_ServerDiscovery), sessionEncryptionKey)
{

}

ServerDiscoveryPacket::~ServerDiscoveryPacket(){

}

void ServerDiscoveryPacket::init(){
    responseFromServer = 0;
    version = "";
    udpPort = 0;
    rtpPort = 0;
    tcpPort = 0;
    serverInstanceID = 0;
}

void ServerDiscoveryPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> responseFromServer >> version;
    if(responseFromServer){
        in >>  rtpPort >> tcpPort >> serverInstanceID;
    }else{
        in >> udpPort;
    }

}

QByteArray ServerDiscoveryPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << responseFromServer << version;
    if(responseFromServer){
        out << rtpPort << tcpPort << serverInstanceID;
    }else{
        out << udpPort;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
DataForwardPacket::DataForwardPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_DataForward), sessionEncryptionKey)
{

}

DataForwardPacket::DataForwardPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_DataForward), sessionEncryptionKey)
{

}

DataForwardPacket::~DataForwardPacket(){

}

void DataForwardPacket::init(){
    data.clear();
    receiver = "";
    isRequest = 0;
}

void DataForwardPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> data >> receiver >> isRequest;
}

QByteArray DataForwardPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << data << receiver << isRequest;
    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
AnnouncementPacket::AnnouncementPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Announcement), sessionEncryptionKey)
{

}

AnnouncementPacket::AnnouncementPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_Announcement), sessionEncryptionKey)
{

}

AnnouncementPacket::~AnnouncementPacket(){

}

void AnnouncementPacket::init(){
    InfoType = ANNOUNCEMENT_UNKNOWN;
    JobID = 0;

    QueryInfo.announcementID = "";
    QueryInfo.keyword = "";
    QueryInfo.validity = "-1";
    QueryInfo.assetNO = "";
    QueryInfo.userName = "";
    QueryInfo.target = "-1";
    QueryInfo.startTime = "";
    QueryInfo.endTime = "";

    CreateInfo.localTempID = 0;
    CreateInfo.adminID = "";
    CreateInfo.type = quint8(ANNOUNCEMENT_CREATE);
    CreateInfo.content = "";
    CreateInfo.confirmationRequired = 1;
    CreateInfo.validityPeriod = 60;
    CreateInfo.targetType = quint8(ANNOUNCEMENT_TARGET_EVERYONE);
    CreateInfo.targets = "";

    UpdateInfo.adminName = "";
    UpdateInfo.announcementID = 0;
    UpdateInfo.targetType = quint8(ANNOUNCEMENT_TARGET_EVERYONE);
    UpdateInfo.active = 1;
    UpdateInfo.addedTargets = "";
    UpdateInfo.deletedTargets = "";

    ReplyInfo.announcementID = 0;
    ReplyInfo.sender = "";
    ReplyInfo.receiver = "";
    ReplyInfo.receiversAssetNO = "";
    ReplyInfo.replyMessage = "";

    QueryTargetsInfo.announcementID = 0;


    AnnouncementInfo.announcementID = 0;
    AnnouncementInfo.content = "";

}

void AnnouncementPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(ANNOUNCEMENT_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case ANNOUNCEMENT_QUERY:
    {
        in >> QueryInfo.announcementID >> QueryInfo.keyword >> QueryInfo.validity >> QueryInfo.assetNO >> QueryInfo.userName >> QueryInfo.target >> QueryInfo.startTime >> QueryInfo.endTime;
    }
        break;

    case ANNOUNCEMENT_CREATE:
    {
        in >> CreateInfo.localTempID >> CreateInfo.adminID >> CreateInfo.type >> CreateInfo.content >> CreateInfo.confirmationRequired >> CreateInfo.validityPeriod >> CreateInfo.targetType >> CreateInfo.targets;
    }
        break;

    case ANNOUNCEMENT_UPDATE:
    {
        in << UpdateInfo.adminName << UpdateInfo.announcementID << UpdateInfo.targetType << UpdateInfo.active << UpdateInfo.addedTargets << UpdateInfo.deletedTargets;
    }
        break;

    case ANNOUNCEMENT_REPLY:
    {
        in >> ReplyInfo.announcementID >> ReplyInfo.sender >> ReplyInfo.receiver >> ReplyInfo.receiversAssetNO >> ReplyInfo.replyMessage ;
    }
        break;

    case ANNOUNCEMENT_QUERY_TARGETS:
    {
        in >> QueryTargetsInfo.announcementID;
    }
        break;

    case ANNOUNCEMENT_INFO:
    {
        in >> AnnouncementInfo.announcementID >> AnnouncementInfo.content;
    }
        break;

    default:
        break;
    }

}

QByteArray AnnouncementPacket::packBodyData(){
    if(InfoType == ANNOUNCEMENT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    switch (InfoType) {
    case ANNOUNCEMENT_QUERY:
    {
        out << QueryInfo.announcementID << QueryInfo.keyword << QueryInfo.validity << QueryInfo.assetNO << QueryInfo.userName << QueryInfo.target << QueryInfo.startTime << QueryInfo.endTime;
    }
        break;

    case ANNOUNCEMENT_CREATE:
    {
        out << CreateInfo.localTempID << CreateInfo.adminID << CreateInfo.type << CreateInfo.content << CreateInfo.confirmationRequired << CreateInfo.validityPeriod << CreateInfo.targetType << CreateInfo.targets;
    }
        break;

    case ANNOUNCEMENT_UPDATE:
    {
        out << UpdateInfo.adminName << UpdateInfo.announcementID << UpdateInfo.targetType << UpdateInfo.active << UpdateInfo.addedTargets << UpdateInfo.deletedTargets;
    }
        break;

    case ANNOUNCEMENT_REPLY:
    {
        out << ReplyInfo.announcementID << ReplyInfo.sender << ReplyInfo.receiver << ReplyInfo.receiversAssetNO << ReplyInfo.replyMessage ;
    }
        break;

    case ANNOUNCEMENT_QUERY_TARGETS:
    {
        out << QueryTargetsInfo.announcementID;
    }
        break;

    case ANNOUNCEMENT_INFO:
    {
        out << AnnouncementInfo.announcementID << AnnouncementInfo.content;
    }
        break;

    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
RgeistrationPacket::RgeistrationPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Rgeistration), sessionEncryptionKey)
{

}

RgeistrationPacket::RgeistrationPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_Rgeistration), sessionEncryptionKey)
{

}

RgeistrationPacket::~RgeistrationPacket(){

}

void RgeistrationPacket::init(){
    InfoType = REGISTRATION_UNKNOWN;
    JobID = 0;

    ServerInfo.requestServerInfo = 0;
    ServerInfo.version = "";
    ServerInfo.regMode = quint8(REG_MODE_USER_CREATE_ALL);
    ServerInfo.message = "";
    ServerInfo.address = "";

    RgeistrationInfo.userID = "";
    RgeistrationInfo.password = "";
    RgeistrationInfo.email = "";
    RgeistrationInfo.captcha = "";
    RgeistrationInfo.securityQuestion = "";
    RgeistrationInfo.securityAnswer = "";

    RgeistrationResult.sysID = 0;
    RgeistrationResult.userID = "";
    RgeistrationResult.password = "";
    RgeistrationResult.errorCode = 0;
    RgeistrationResult.activationMode = quint8(ACT_MODE_AUTO);

}

void RgeistrationPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(REGISTRATION_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case REGISTRATION_SERVER_INFO:
    {
        in >> ServerInfo.requestServerInfo >> ServerInfo.version ;
        if(!ServerInfo.requestServerInfo){
            in >> ServerInfo.regMode >> ServerInfo.message >> ServerInfo.address;
        }
    }
        break;

    case REGISTRATION_INFO:
    {
        in >> RgeistrationInfo.userID >> RgeistrationInfo.password >> RgeistrationInfo.email >> RgeistrationInfo.captcha >> RgeistrationInfo.securityQuestion >> RgeistrationInfo.securityAnswer;
    }
        break;

    case REGISTRATION_RESULT:
    {
        in >> RgeistrationResult.sysID >> RgeistrationResult.userID >> RgeistrationResult.password >> RgeistrationResult.errorCode >> RgeistrationResult.activationMode;
    }
        break;


    default:
        break;
    }

}

QByteArray RgeistrationPacket::packBodyData(){
    if(InfoType == REGISTRATION_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    switch (InfoType) {
    case REGISTRATION_SERVER_INFO:
    {
        out << ServerInfo.requestServerInfo << ServerInfo.version;
        if(!ServerInfo.requestServerInfo){
            out << ServerInfo.regMode << ServerInfo.message << ServerInfo.address;
        }
    }
        break;

    case REGISTRATION_INFO:
    {
        out << RgeistrationInfo.userID << RgeistrationInfo.password << RgeistrationInfo.email << RgeistrationInfo.captcha << RgeistrationInfo.securityQuestion << RgeistrationInfo.securityAnswer;
    }
        break;

    case REGISTRATION_RESULT:
    {
        out << RgeistrationResult.sysID << RgeistrationResult.userID << RgeistrationResult.password << RgeistrationResult.errorCode << RgeistrationResult.activationMode;
    }
        break;


    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
UpdatePasswordPacket::UpdatePasswordPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_UpdatePassword), sessionEncryptionKey)
{

}

UpdatePasswordPacket::UpdatePasswordPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_UpdatePassword), sessionEncryptionKey)
{

}

UpdatePasswordPacket::~UpdatePasswordPacket(){

}

void UpdatePasswordPacket::init(){
    InfoType = INFO_TYPE_UNKNOWN;
    JobID = 0;

    AuthInfo.authMode = quint8(AUTH_OLD_PWD_ONLY);
    AuthInfo.captchaImage = QByteArray();
    AuthInfo.captcha = "";

    AuthInfo.userID = "";
    AuthInfo.oldPassword = "";
    AuthInfo.newPassword = "";
    AuthInfo.securityQuestion = "";
    AuthInfo.securityAnswer = "";
    AuthInfo.email = "";
    AuthInfo.cellphoneNO = 0;
    AuthInfo.smsCaptcha = "";
    AuthInfo.url = "";
    AuthInfo.authResult = 0;
    AuthInfo.updateResult = 0;

}

void UpdatePasswordPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(INFO_TYPE_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    in >> AuthInfo.authMode >> AuthInfo.captchaImage >> AuthInfo.captcha >> AuthInfo.userID;
    in >> AuthInfo.oldPassword >> AuthInfo.newPassword >> AuthInfo.securityQuestion >> AuthInfo.securityAnswer;
    in >> AuthInfo.email >> AuthInfo.cellphoneNO >> AuthInfo.smsCaptcha >> AuthInfo.url;
    in >> AuthInfo.authResult >> AuthInfo.updateResult ;

}

QByteArray UpdatePasswordPacket::packBodyData(){
    if(InfoType == INFO_TYPE_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    out << AuthInfo.authMode << AuthInfo.captchaImage << AuthInfo.captcha << AuthInfo.userID;
    out << AuthInfo.oldPassword << AuthInfo.newPassword << AuthInfo.securityQuestion << AuthInfo.securityAnswer;
    out << AuthInfo.email << AuthInfo.cellphoneNO << AuthInfo.smsCaptcha << AuthInfo.url;
    out << AuthInfo.authResult << AuthInfo.updateResult ;



    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
LoginPacket::LoginPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Login), sessionEncryptionKey)
{

}

LoginPacket::LoginPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_Login), sessionEncryptionKey)
{

}

LoginPacket::~LoginPacket(){

}

void LoginPacket::init(){
    InfoType = INFO_TYPE_UNKNOWN;
    JobID = 0;

    LoginServerInfo.version = APP_VERSION;
    LoginServerInfo.serverAddress = "";
    LoginServerInfo.serverPort = 0;

    AuthInfo.password = QByteArray();
    AuthInfo.stateAfterLoggedin = quint8(IM::ONLINESTATE_ONLINE);
    AuthInfo.deviceInfo = "";

    AuthResultInfo.loggedin = 0;
    AuthResultInfo.errorType = quint8(IM::ERROR_NoError);
    AuthResultInfo.sessionEncryptionKey = QByteArray();
    AuthResultInfo.personalSummaryInfoVersionOnServer = 1;
    AuthResultInfo.personalDetailInfoVersionOnServer = 1;
    AuthResultInfo.personalContactGroupsInfoVersionOnServer = 1;
    AuthResultInfo.interestGroupsInfoVersionOnServer = 1;
    AuthResultInfo.personalMessageInfoVersionOnServer = 1;
    AuthResultInfo.serverTime = 0;

    PreviousLoginInfo.loginIP = "";
    PreviousLoginInfo.loginTime = 0;
    PreviousLoginInfo.logoutTime = 0;
    PreviousLoginInfo.clientVersion = "";
    PreviousLoginInfo.deviceInfo = "";

}

void LoginPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(INFO_TYPE_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case INFO_TYPE_LOGIN_SERVER_INFO:
    {
        in >> LoginServerInfo.version >> LoginServerInfo.serverAddress >> LoginServerInfo.serverPort;
    }
        break;

    case INFO_TYPE_AUTH_INFO_FROM_CLIENT:
    {
        in >> AuthInfo.password >> AuthInfo.stateAfterLoggedin >> AuthInfo.deviceInfo;
    }
        break;

    case INFO_TYPE_LOGIN_RESULT:
    {
        in >> AuthResultInfo.loggedin >> AuthResultInfo.errorType
           >> AuthResultInfo.sessionEncryptionKey
           >> AuthResultInfo.personalSummaryInfoVersionOnServer
           >> AuthResultInfo.personalDetailInfoVersionOnServer
           >> AuthResultInfo.personalContactGroupsInfoVersionOnServer
           >> AuthResultInfo.interestGroupsInfoVersionOnServer
           >> AuthResultInfo.personalMessageInfoVersionOnServer
           >> AuthResultInfo.serverTime
                ;
    }
        break;

    case INFO_TYPE_PREVIOUS_LOGIN_INFO:
    {
        in >> PreviousLoginInfo.loginIP >> PreviousLoginInfo.loginTime >> PreviousLoginInfo.logoutTime>> PreviousLoginInfo.clientVersion >> PreviousLoginInfo.deviceInfo;
    }
        break;

    default:
        break;
    }

}

QByteArray LoginPacket::packBodyData(){
    if(InfoType == INFO_TYPE_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    switch (InfoType) {
    case INFO_TYPE_LOGIN_SERVER_INFO:
    {
        out << LoginServerInfo.version << LoginServerInfo.serverAddress << LoginServerInfo.serverPort;
    }
        break;
    case INFO_TYPE_AUTH_INFO_FROM_CLIENT:
    {
        out << AuthInfo.password << AuthInfo.stateAfterLoggedin << AuthInfo.deviceInfo;
    }
        break;

    case INFO_TYPE_LOGIN_RESULT:
    {
        out << AuthResultInfo.loggedin << AuthResultInfo.errorType
            << AuthResultInfo.sessionEncryptionKey
            << AuthResultInfo.personalSummaryInfoVersionOnServer
            << AuthResultInfo.personalDetailInfoVersionOnServer
            << AuthResultInfo.personalContactGroupsInfoVersionOnServer
            << AuthResultInfo.interestGroupsInfoVersionOnServer
            << AuthResultInfo.personalMessageInfoVersionOnServer
            << AuthResultInfo.serverTime
               ;
    }
        break;

    case INFO_TYPE_PREVIOUS_LOGIN_INFO:
    {
        out << PreviousLoginInfo.loginIP << PreviousLoginInfo.loginTime << PreviousLoginInfo.logoutTime<< PreviousLoginInfo.clientVersion << PreviousLoginInfo.deviceInfo;
    }
        break;

    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
OnlineStateChangedPacket::OnlineStateChangedPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_OnlineStateChanged), sessionEncryptionKey)
{

}

OnlineStateChangedPacket::OnlineStateChangedPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_OnlineStateChanged), sessionEncryptionKey)
{

}

OnlineStateChangedPacket::~OnlineStateChangedPacket(){

}

void OnlineStateChangedPacket::init(){
    stateCode = quint8(IM::ONLINESTATE_OFFLINE);
    contactID = "";
    contactIP = "";
    contactPort = 0;
}

void OnlineStateChangedPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> stateCode >> contactID >> contactIP >> contactPort;
}

QByteArray OnlineStateChangedPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << stateCode << contactID << contactIP << contactPort;
    return ba;
}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
OnlineContacts::OnlineContacts(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_OnlineContacts), sessionEncryptionKey)
{
}

OnlineContacts::OnlineContacts(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_OnlineContacts), sessionEncryptionKey)
{
}

OnlineContacts::~OnlineContacts(){
}

void OnlineContacts::init(){
    contactsOnlineInfoString = "";

}

void OnlineContacts::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> contactsOnlineInfoString;
}

QByteArray OnlineContacts::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << contactsOnlineInfoString;
    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ContactGroupsInfoPacket::ContactGroupsInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ContactGroupsInfo), sessionEncryptionKey)
{
}

ContactGroupsInfoPacket::ContactGroupsInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_ContactGroupsInfo), sessionEncryptionKey)
{
}

ContactGroupsInfoPacket::~ContactGroupsInfoPacket(){
}

void ContactGroupsInfoPacket::init(){
    InfoType = PIT_UNKNOWN;
    JobID = 0;

    GroupsList.groupsInfo = "";
    GroupsList.version = 0;
    GroupsList.contactInfoVersionList = "";

    GroupParentInfo.groupID = 0;
    GroupParentInfo.parentID = 0;

    GroupCreationInfo.name = "";
    GroupCreationInfo.parentID = 0;
    GroupCreationInfo.groupID = 0;
    GroupCreationInfo.errorCode = quint8(IM::ERROR_NoError);

    GroupDeletionInfo.groupID = 0;
    GroupDeletionInfo.errorCode = quint8(IM::ERROR_NoError);

    GroupRenamingInfo.groupID = 0;
    GroupRenamingInfo.newName = "";


}

void ContactGroupsInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(PIT_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case PIT_GROUPS_LIST:
    {
        in >> GroupsList.groupsInfo >> GroupsList.version >> GroupsList.contactInfoVersionList;
    }
        break;
    case PIT_GROUP_CHANGE_PARENT:
    {
        in >> GroupParentInfo.groupID >> GroupParentInfo.parentID;
    }
        break;
    case PIT_GROUP_CREATION:
    {
        in >> GroupCreationInfo.name >> GroupCreationInfo.parentID >> GroupCreationInfo.groupID >> GroupCreationInfo.errorCode;
    }
        break;
    case PIT_GROUP_DELETION:
    {
        in >> GroupDeletionInfo.groupID >> GroupDeletionInfo.errorCode;
    }
        break;
    case PIT_GROUP_RENAMING:
    {
        in >> GroupRenamingInfo.groupID >> GroupRenamingInfo.newName;
    }
        break;


    default:
        break;
    }

}

QByteArray ContactGroupsInfoPacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    switch (InfoType) {
    case PIT_GROUPS_LIST:
    {
        out << GroupsList.groupsInfo << GroupsList.version >> GroupsList.contactInfoVersionList;
    }
        break;
    case PIT_GROUP_CHANGE_PARENT:
    {
        out << GroupParentInfo.groupID << GroupParentInfo.parentID;
    }
        break;
    case PIT_GROUP_CREATION:
    {
        out << GroupCreationInfo.name << GroupCreationInfo.parentID << GroupCreationInfo.groupID << GroupCreationInfo.errorCode;
    }
        break;
    case PIT_GROUP_DELETION:
    {
        out << GroupDeletionInfo.groupID << GroupDeletionInfo.errorCode;
    }
        break;
    case PIT_GROUP_RENAMING:
    {
        out << GroupRenamingInfo.groupID << GroupRenamingInfo.newName;
    }
        break;



    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
InterestGroupsInfoPacket::InterestGroupsInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_InterestGroupsInfo), sessionEncryptionKey)
{

}

InterestGroupsInfoPacket::InterestGroupsInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_InterestGroupsInfo), sessionEncryptionKey)
{

}

InterestGroupsInfoPacket::~InterestGroupsInfoPacket(){

}

void InterestGroupsInfoPacket::init(){
    InfoType = PIT_UNKNOWN;
    JobID = 0;
    GroupID = 0;

    GroupsList.groups = "";
    GroupsList.version = 0;

//    GroupInfo.id = 0;
    GroupInfo.infoString = "";
//    GroupInfo.name = QByteArray();
//    GroupInfo.creator = "";
//    GroupInfo.creationTime = 0;
//    GroupInfo.type = 0;
//    GroupInfo.infoVersion = 0;
//    GroupInfo.members = "";
//    GroupInfo.admins = "";

    GroupAnnouncementInfo.content = "";
    GroupAnnouncementInfo.admin = "";

    GroupCreationInfo.name = "";
    GroupCreationInfo.type = 0;

    GroupDeletionInfo.deleted = 0;

    MemberApplicationInfo.userID = "";
    MemberApplicationInfo.message = "";
    MemberApplicationInfo.approved = 0;
    MemberApplicationInfo.admin = "";

    MemberDeletionInfo.userID = "";
    MemberDeletionInfo.blockForever = 0;
    MemberDeletionInfo.admin = "";

    MemberPromotionInfo.userID = "";
    MemberPromotionInfo.promoted = 0;

}

void InterestGroupsInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(PIT_UNKNOWN);
    in >> type >> JobID >> GroupID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case PIT_GROUPS_LIST:
    {
        in >> GroupsList.groups >> GroupsList.version;
    }
        break;
    case PIT_GROUP_INFO:
    {
        in >> GroupInfo.infoString;
        //in >> GroupInfo.id >> GroupInfo.name >> GroupInfo.creator >> GroupInfo.creationTime
        //   >> GroupInfo.type >> GroupInfo.infoVersion >> GroupInfo.members >> GroupInfo.admins;
    }
        break;
    case PIT_GROUP_UPDATE_ANNOUNCEMENT:
    {
        in >> GroupAnnouncementInfo.content >> GroupAnnouncementInfo.admin;
    }
        break;
    case PIT_GROUP_CREATION:
    {
        in >> GroupCreationInfo.name >> GroupCreationInfo.type;
    }
        break;
    case PIT_GROUP_DELETION:
    {
        in >> GroupDeletionInfo.deleted;
    }
        break;
    case PIT_GROUP_MEMBER_APPLICATION:
    {
        in >> MemberApplicationInfo.userID >> MemberApplicationInfo.message >> MemberApplicationInfo.approved >> MemberApplicationInfo.admin;
    }
        break;

    case PIT_GROUP_MEMBER_DELETION:
    {
        in >> MemberDeletionInfo.userID >> MemberDeletionInfo.blockForever >> MemberDeletionInfo.admin;
    }
        break;

    case PIT_GROUP_MEMBER_PROMOTION:
    {
        in >> MemberPromotionInfo.userID >> MemberPromotionInfo.promoted;
    }
        break;


    default:
        break;
    }

}

QByteArray InterestGroupsInfoPacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID << GroupID;

    switch (InfoType) {
    case PIT_GROUPS_LIST:
    {
        out << GroupsList.groups << GroupsList.version;
    }
        break;
    case PIT_GROUP_INFO:
    {
        out << GroupInfo.infoString;
        //out << GroupInfo.id << GroupInfo.name << GroupInfo.creator << GroupInfo.creationTime
        //   << GroupInfo.type << GroupInfo.infoVersion << GroupInfo.members << GroupInfo.admins;
    }
        break;
    case PIT_GROUP_UPDATE_ANNOUNCEMENT:
    {
        out << GroupAnnouncementInfo.content << GroupAnnouncementInfo.admin;
    }
        break;
    case PIT_GROUP_CREATION:
    {
        out << GroupCreationInfo.name << GroupCreationInfo.type;
    }
        break;
    case PIT_GROUP_DELETION:
    {
        out << GroupDeletionInfo.deleted;
    }
        break;
    case PIT_GROUP_MEMBER_APPLICATION:
    {
        out << MemberApplicationInfo.userID << MemberApplicationInfo.message << MemberApplicationInfo.approved << MemberApplicationInfo.admin;
    }
        break;

    case PIT_GROUP_MEMBER_DELETION:
    {
        out << MemberDeletionInfo.userID << MemberDeletionInfo.blockForever << MemberDeletionInfo.admin;
    }
        break;

    case PIT_GROUP_MEMBER_PROMOTION:
    {
        out << MemberPromotionInfo.userID << MemberPromotionInfo.promoted;
    }
        break;



    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ContactInfoPacket::ContactInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ContactInfo), sessionEncryptionKey)
{
}

ContactInfoPacket::ContactInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_ContactInfo), sessionEncryptionKey)
{
}

ContactInfoPacket::~ContactInfoPacket(){
}

void ContactInfoPacket::init(){
    InfoType = PIT_UNKNOWN;
    JobID = 0;
    ContactID = "";

    info.isSummaryInfo = 1;
    info.infoString = "";

    ContactChangeGroup.oldGroupID = 0;
    ContactChangeGroup.newGroupID = 0;

    ContactFriendingRequest.nickName = "";
    ContactFriendingRequest.userFace = "";
    ContactFriendingRequest.message = "";
    ContactFriendingRequest.groupID = 0;

    ContactFriendingResult.errorCode = quint8(IM::ERROR_NoError);
    ContactFriendingResult.message = "";

    ContactDeletionInfo.deleteMeFromOpposition = 1;
    ContactDeletionInfo.blockForever = 0;
    ContactDeletionInfo.errorCode = quint8(IM::ERROR_NoError);

    ContactRemarkInfo.newRemarkName = "";

}

void ContactInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(PIT_UNKNOWN);
    in >> type >> JobID >> ContactID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case PIT_CONTACT_INFO:
    {
        in >> info.isSummaryInfo >> info.infoString;
    }
        break;
    case PIT_GROUP_CHANGE:
    {
        in >> ContactChangeGroup.oldGroupID >> ContactChangeGroup.newGroupID;
    }
        break;
    case PIT_FRIENDING_REQUEST:
    {
        in >> ContactFriendingRequest.nickName >> ContactFriendingRequest.userFace >> ContactFriendingRequest.message >> ContactFriendingRequest.groupID;
    }
        break;
    case PIT_FRIENDING_RESULT:
    {
        in >> ContactFriendingResult.errorCode >> ContactFriendingResult.message;
    }
        break;
    case PIT_CONTACT_DELETION:
    {
        in >> ContactDeletionInfo.deleteMeFromOpposition >> ContactDeletionInfo.blockForever >> ContactDeletionInfo.errorCode;
    }
        break;
    case PIT_CONTACT_REMARK:
    {
        in >> ContactRemarkInfo.newRemarkName;
    }
        break;


    default:
        break;
    }

}

QByteArray ContactInfoPacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID << ContactID;

    switch (InfoType) {
    case PIT_CONTACT_INFO:
    {
        out << info.isSummaryInfo << info.infoString;
    }
        break;
    case PIT_GROUP_CHANGE:
    {
        out << ContactChangeGroup.oldGroupID << ContactChangeGroup.newGroupID;
    }
        break;

    case PIT_FRIENDING_REQUEST:
    {
        out << ContactFriendingRequest.nickName << ContactFriendingRequest.userFace << ContactFriendingRequest.message >> ContactFriendingRequest.groupID;
    }
        break;
    case PIT_FRIENDING_RESULT:
    {
        out << ContactFriendingResult.errorCode << ContactFriendingResult.message;
    }
        break;
    case PIT_CONTACT_DELETION:
    {
        out << ContactDeletionInfo.deleteMeFromOpposition << ContactDeletionInfo.blockForever << ContactDeletionInfo.errorCode;
    }
        break;
    case PIT_CONTACT_REMARK:
    {
        out << ContactRemarkInfo.newRemarkName;
    }
        break;



    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
SearchInfoPacket::SearchInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_SearchInfo), sessionEncryptionKey)
{
}

SearchInfoPacket::SearchInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_SearchInfo), sessionEncryptionKey)
{
}

SearchInfoPacket::~SearchInfoPacket(){
}

void SearchInfoPacket::init(){
    InfoType = PIT_UNKNOWN;
    JobID = 0;

    SearchContactConditions.keyword = "";
    SearchContactConditions.isOnline = 0;
    SearchContactConditions.hasWebcam = 0;
    SearchContactConditions.location = 0;
    SearchContactConditions.hometown = 0;
    SearchContactConditions.gender = 0;
    SearchContactConditions.age = 0;
    SearchContactConditions.matchExactly = 1;
    SearchContactConditions.startIndex = 0;

    SearchContactResult.result = "";

    SearchInterestGroupConditions.keyword = "";
    SearchInterestGroupConditions.startIndex = 0;

    SearchInterestGroupResult.result = "";

}

void SearchInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(PIT_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case PIT_SEARCH_CONTACT_CONDITIONS:
    {
        in >> SearchContactConditions.keyword >> SearchContactConditions.isOnline
                >> SearchContactConditions.hasWebcam >> SearchContactConditions.location
                >> SearchContactConditions.hometown >> SearchContactConditions.gender
                >> SearchContactConditions.age >> SearchContactConditions.matchExactly
                >> SearchContactConditions.startIndex
                ;
    }
        break;
    case PIT_SEARCH_CONTACT_RESULT:
    {
        in >> SearchContactResult.result;
    }
        break;
    case PIT_SEARCH_INTEREST_GROUP_CONDITIONS:
    {
        in >> SearchInterestGroupConditions.keyword >> SearchInterestGroupConditions.startIndex;
    }
        break;
    case PIT_SEARCH_INTEREST_GROUP_RESULT:
    {
        in >> SearchInterestGroupResult.result;
    }
        break;


    default:
        break;
    }

}

QByteArray SearchInfoPacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    switch (InfoType) {
    case PIT_SEARCH_CONTACT_CONDITIONS:
    {
        out << SearchContactConditions.keyword << SearchContactConditions.isOnline
                << SearchContactConditions.hasWebcam << SearchContactConditions.location
                << SearchContactConditions.hometown << SearchContactConditions.gender
                << SearchContactConditions.age << SearchContactConditions.matchExactly
                << SearchContactConditions.startIndex
                ;
    }
        break;
    case PIT_SEARCH_CONTACT_RESULT:
    {
        out << SearchContactResult.result;
    }
        break;
    case PIT_SEARCH_INTEREST_GROUP_CONDITIONS:
    {
        out << SearchInterestGroupConditions.keyword << SearchInterestGroupConditions.startIndex;
    }
        break;
    case PIT_SEARCH_INTEREST_GROUP_RESULT:
    {
        out << SearchInterestGroupResult.result;
    }
        break;



    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
ChatMessagePacket::ChatMessagePacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ChatMessage), sessionEncryptionKey)
{
}

ChatMessagePacket::ChatMessagePacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_ChatMessage), sessionEncryptionKey)
{
}

ChatMessagePacket::~ChatMessagePacket(){
}

void ChatMessagePacket::init(){
    InfoType = PIT_UNKNOWN;
    JobID = 0;

    ContactChatMessage.contactID = "";
    ContactChatMessage.message = "";
    ContactChatMessage.imageNames = "";

    ContactChatMessagesCachedOnServer.messages = "";

    ContactChatHistoryMessages.contactID = "";
    ContactChatHistoryMessages.messages = "";
    ContactChatHistoryMessages.startime = 0;

    GroupChatMessage.groupID = 0;
    GroupChatMessage.memberID = "";
    GroupChatMessage.time = 0;
    GroupChatMessage.message = "";
    GroupChatMessage.imageNames = "";

    GroupChatMessagesCachedOnServer.messages = "";

    GroupChatHistoryMessages.groupID = 0;
    GroupChatHistoryMessages.messages = "";
    GroupChatHistoryMessages.startime = 0;

    ChatImage.isRequest = 0;
    ChatImage.contactID = "";
    ChatImage.name = "";
    ChatImage.image = QByteArray();

    SessionEncryptionKeyWithContact.contactID = "";
    SessionEncryptionKeyWithContact.key = QByteArray();
}

void ChatMessagePacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(PIT_UNKNOWN);
    in >> type >> JobID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case PIT_CONTACT_CHAT_MESSAGE:
    {
        in >> ContactChatMessage.contactID >> ContactChatMessage.message >> ContactChatMessage.imageNames;
    }
        break;
    case PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER:
    {
        in >> ContactChatMessagesCachedOnServer.messages;
    }
        break;
    case PIT_CONTACT_CHAT_HISTORY_MESSAGES:
    {
        in >> ContactChatHistoryMessages.contactID >> ContactChatHistoryMessages.messages >> ContactChatHistoryMessages.startime;
    }
        break;
    case PIT_GROUP_CHAT_MESSAGE:
    {
        in >> GroupChatMessage.groupID >> GroupChatMessage.memberID >> GroupChatMessage.time >> GroupChatMessage.message >> GroupChatMessage.imageNames;
    }
        break;
    case PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER:
    {
        in >> GroupChatMessagesCachedOnServer.messages;
    }
        break;
    case PIT_GROUP_CHAT_HISTORY_MESSAGES:
    {
        in >> GroupChatHistoryMessages.groupID >> GroupChatHistoryMessages.messages >> GroupChatHistoryMessages.startime;
    }
        break;
    case PIT_CHAT_IMAGE:
    {
        in >> ChatImage.isRequest >> ChatImage.contactID >> ChatImage.name >> ChatImage.image;
    }
        break;
    case PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT:
    {
        in >> SessionEncryptionKeyWithContact.contactID >> SessionEncryptionKeyWithContact.key;
    }
        break;



    default:
        break;
    }

}

QByteArray ChatMessagePacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID;

    switch (InfoType) {
    case PIT_CONTACT_CHAT_MESSAGE:
    {
       out << ContactChatMessage.contactID << ContactChatMessage.message << ContactChatMessage.imageNames;
    }
        break;
    case PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER:
    {
        out << ContactChatMessagesCachedOnServer.messages;
    }
        break;
    case PIT_CONTACT_CHAT_HISTORY_MESSAGES:
    {
        out << ContactChatHistoryMessages.contactID << ContactChatHistoryMessages.messages << ContactChatHistoryMessages.startime;
    }
        break;
    case PIT_GROUP_CHAT_MESSAGE:
    {
       out << GroupChatMessage.groupID << GroupChatMessage.memberID << GroupChatMessage.time << GroupChatMessage.message << GroupChatMessage.imageNames;
    }
        break;
    case PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER:
    {
        out << GroupChatMessagesCachedOnServer.messages;
    }
        break;
    case PIT_GROUP_CHAT_HISTORY_MESSAGES:
    {
        out << GroupChatHistoryMessages.groupID << GroupChatHistoryMessages.messages << GroupChatHistoryMessages.startime;
    }
        break;
    case PIT_CHAT_IMAGE:
    {
        out << ChatImage.isRequest << ChatImage.contactID << ChatImage.name << ChatImage.image;
    }
        break;
    case PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT:
    {
        out << SessionEncryptionKeyWithContact.contactID << SessionEncryptionKeyWithContact.key;
    }
        break;

    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
CaptchaInfoPacket::CaptchaInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Captcha), sessionEncryptionKey)
{
}

CaptchaInfoPacket::CaptchaInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_Captcha), sessionEncryptionKey)
{
}

CaptchaInfoPacket::~CaptchaInfoPacket(){
}

void CaptchaInfoPacket::init(){
    InfoType = PIT_UNKNOWN;
    JobID = 0;

    captchaID = 0;
    captchaImage = QByteArray();
    captchaCode = "";
    approved = 0;

}

void CaptchaInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);


    quint8 type = quint8(PIT_UNKNOWN);
    in >> type >> JobID >> captchaID;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case CAPTCHA_REQUEST:
    {
    }
        break;
    case CAPTCHA_IMAGE:
    {
        in >> captchaImage;
    }
        break;
    case CAPTCHA_CODE:
    {
        in >> captchaCode;
    }
        break;
    case CAPTCHA_AUTH_RESULT:
    {
        in >> approved;
    }
        break;


    default:
        break;
    }

}

QByteArray CaptchaInfoPacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID << captchaID;

    switch (InfoType) {
    case CAPTCHA_REQUEST:
    {
    }
        break;
    case CAPTCHA_IMAGE:
    {
        out << captchaImage;
    }
        break;
    case CAPTCHA_CODE:
    {
        out << captchaCode;
    }
        break;
    case CAPTCHA_AUTH_RESULT:
    {
        out << approved;
    }
        break;



    default:
        break;
    }


    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
FileTransferPacket::FileTransferPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_FileTransfer), sessionEncryptionKey)
{

}

FileTransferPacket::FileTransferPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_FileTransfer), sessionEncryptionKey)
{


}

void FileTransferPacket::init(){
    InfoType = FT_UNKNOWN;
    ContactID = "";

    FileServerInfo.address = "";
    FileServerInfo.port = 0;

    FileSystemInfoRequest.parentDirPath = "";

    FileSystemInfoResponse.baseDirPath = "";
    FileSystemInfoResponse.fileSystemInfoData = QByteArray();

    FileDeletingRequest.baseDirPath = "";
    FileDeletingRequest.files.clear();

    FileDeletingResponse.baseDirPath = "";
    FileDeletingResponse.failedFiles.clear();

    FileRenamingRequest.baseDirPath = "";
    FileRenamingRequest.oldFileName = "";
    FileRenamingRequest.newFileName = "";

    FileRenamingResponse.baseDirPath = "";
    FileRenamingResponse.oldFileName = "";
    FileRenamingResponse.renamed = 0;
    FileRenamingResponse.message = "";


    FileDownloadingRequest.baseDir = "";
    FileDownloadingRequest.fileName = "";
    FileDownloadingRequest.dirToSaveFile = "./";

    FileDownloadingResponse.baseDir = "";
    FileDownloadingResponse.fileName = "";
    FileDownloadingResponse.accepted = 1;
    FileDownloadingResponse.fileMD5Sum = QByteArray();
    FileDownloadingResponse.size = 0;
    FileDownloadingResponse.errorCode = quint8(IM::ERROR_NoError);

    FileUploadingRequest.fileName = "";
    FileUploadingRequest.fileMD5Sum = QByteArray();
    FileUploadingRequest.size = 0;
    FileUploadingRequest.fileSaveDir = "./";

    FileUploadingResponse.fileMD5Sum = QByteArray();
    FileUploadingResponse.accepted = 1;
    FileUploadingResponse.message = "";

    FileDataRequest.fileMD5 = QByteArray();
    FileDataRequest.startPieceIndex = 0;
    FileDataRequest.endPieceIndex = 0;

    FileDataResponse.fileMD5 = QByteArray();
    FileDataResponse.pieceIndex = 0;
    FileDataResponse.data = QByteArray();
    FileDataResponse.pieceMD5 = QByteArray();

    FileTXStatus.fileMD5 = QByteArray();
    FileTXStatus.status = quint8(IM::File_TX_Preparing);

    FileTXError.fileName = "";
    FileTXError.fileMD5 = QByteArray();
    FileTXError.errorCode = 0;
    FileTXError.message = "";

}

void FileTransferPacket::parsePacketBody(QByteArray &packetBody){

    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    quint8 type = quint8(FT_UNKNOWN);
    in >> type >> ContactID;

    InfoType = PacketInfoType(type);
    switch (InfoType) {
    case FT_FILE_SERVER_INFO:
    {
        in >> FileServerInfo.address >> FileServerInfo.port;
    }
        break;

    case FT_FileSystemInfoRequest:
    {
        in >> FileSystemInfoRequest.parentDirPath;
    }
        break;

    case FT_FileSystemInfoResponse:
    {
        in >> FileSystemInfoResponse.baseDirPath >> FileSystemInfoResponse.fileSystemInfoData;
    }
        break;

    case FT_FileDeletingRequest:
    {
        in >> FileDeletingRequest.baseDirPath >> FileDeletingRequest.files;
    }
        break;

    case FT_FileDeletingResponse:
    {
        in >> FileDeletingResponse.baseDirPath >> FileDeletingResponse.failedFiles;
    }
        break;

    case FT_FileRenamingRequest:
    {
        in >> FileRenamingRequest.baseDirPath >> FileRenamingRequest.oldFileName >> FileRenamingRequest.newFileName;
    }
        break;

    case FT_FileRenamingResponse:
    {
        in >> FileRenamingResponse.baseDirPath >> FileRenamingResponse.oldFileName >> FileRenamingResponse.renamed >> FileRenamingResponse.message;
    }
        break;

    case FT_FileDownloadingRequest:
    {
        in >> FileDownloadingRequest.baseDir >> FileDownloadingRequest.fileName >> FileDownloadingRequest.dirToSaveFile;
    }
        break;

    case FT_FileDownloadingResponse:
    {
        in >> FileDownloadingResponse.accepted >> FileDownloadingResponse.baseDir >> FileDownloadingResponse.fileName >> FileDownloadingResponse.fileMD5Sum >> FileDownloadingResponse.size >> FileDownloadingResponse.errorCode;
    }
        break;

    case FT_FileUploadingRequest:
    {
        in >> FileUploadingRequest.fileName >> FileUploadingRequest.fileMD5Sum >> FileUploadingRequest.size >> FileUploadingRequest.fileSaveDir;
    }
        break;

    case FT_FileUploadingResponse:
    {
        in >> FileUploadingResponse.accepted >> FileUploadingResponse.fileMD5Sum >> FileUploadingResponse.message;
    }
        break;

    case FT_FileDataRequest:
    {
        in >> FileDataRequest.fileMD5 >> FileDataRequest.startPieceIndex >> FileDataRequest.endPieceIndex;
    }
        break;

    case FT_FileData:
    {
        in >> FileDataResponse.fileMD5 >> FileDataResponse.pieceIndex >> FileDataResponse.data >> FileDataResponse.pieceMD5;
    }
        break;

    case FT_FileTXStatus:
    {
        in >> FileTXStatus.fileMD5 >> FileTXStatus.status;
    }
        break;

    case FT_FileTXError:
    {
        in >> FileTXError.fileName >> FileTXError.fileMD5 >> FileTXError.errorCode >> FileTXError.message;
    }
        break;

    default:
        break;
    }

}

QByteArray FileTransferPacket::packBodyData(){
    if(InfoType == FT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint8(InfoType) << ContactID;

    switch (InfoType) {
    case FT_FILE_SERVER_INFO:
    {
        out << FileServerInfo.address << FileServerInfo.port;
    }
        break;

    case FT_FileSystemInfoRequest:
    {
        out << FileSystemInfoRequest.parentDirPath;
    }
        break;

    case FT_FileSystemInfoResponse:
    {
        out << FileSystemInfoResponse.baseDirPath << FileSystemInfoResponse.fileSystemInfoData;
    }
        break;

    case FT_FileDeletingRequest:
    {
        out << FileDeletingRequest.baseDirPath << FileDeletingRequest.files;
    }
        break;

    case FT_FileDeletingResponse:
    {
        out << FileDeletingResponse.baseDirPath << FileDeletingResponse.failedFiles;
    }
        break;

    case FT_FileRenamingRequest:
    {
        out << FileRenamingRequest.baseDirPath << FileRenamingRequest.oldFileName << FileRenamingRequest.newFileName;
    }
        break;

    case FT_FileRenamingResponse:
    {
        out << FileRenamingResponse.baseDirPath << FileRenamingResponse.oldFileName << FileRenamingResponse.renamed << FileRenamingResponse.message;
    }
        break;

    case FT_FileDownloadingRequest:
    {
        out << FileDownloadingRequest.baseDir << FileDownloadingRequest.fileName << FileDownloadingRequest.dirToSaveFile;
    }
        break;

    case FT_FileDownloadingResponse:
    {
        out << FileDownloadingResponse.accepted << FileDownloadingResponse.baseDir << FileDownloadingResponse.fileName << FileDownloadingResponse.fileMD5Sum << FileDownloadingResponse.size << FileDownloadingResponse.errorCode;
    }
        break;

    case FT_FileUploadingRequest:
    {
        out << FileUploadingRequest.fileName << FileUploadingRequest.fileMD5Sum << FileUploadingRequest.size << FileUploadingRequest.fileSaveDir;
    }
        break;

    case FT_FileUploadingResponse:
    {
        out << FileUploadingResponse.accepted << FileUploadingResponse.fileMD5Sum << FileUploadingResponse.message;
    }
        break;

    case FT_FileDataRequest:
    {
        out << FileDataRequest.fileMD5 << FileDataRequest.startPieceIndex << FileDataRequest.endPieceIndex;
    }
        break;

    case FT_FileData:
    {
        out << FileDataResponse.fileMD5 << FileDataResponse.pieceIndex << FileDataResponse.data << FileDataResponse.pieceMD5;
    }
        break;

    case FT_FileTXStatus:
    {
        out << FileTXStatus.fileMD5 << FileTXStatus.status;
    }
        break;

    case FT_FileTXError:
    {
        out << FileTXError.fileName << FileTXError.fileMD5 << FileTXError.errorCode << FileTXError.message;
    }
        break;

    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////






























////////////////////////////////////////////////////////////////////////
JobProgressPacket::JobProgressPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_JobProgress), sessionEncryptionKey)
{

}

JobProgressPacket::JobProgressPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_JobProgress), sessionEncryptionKey)
{


}

JobProgressPacket::~JobProgressPacket(){

}

void JobProgressPacket::init(){
    jobID = 0;
    result = 0;
    extraData = QVariant();
}

void JobProgressPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> jobID >> result >> extraData;
}

QByteArray JobProgressPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << jobID << result << extraData;
    return ba;
}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
AdminLoginPacket::AdminLoginPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_AdminLogin), sessionEncryptionKey)
{

}

AdminLoginPacket::AdminLoginPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_AdminLogin), sessionEncryptionKey)
{


}

AdminLoginPacket::~AdminLoginPacket(){

}

void AdminLoginPacket::init(){
    InfoType = LOGIN_REQUEST;

    LoginInfo.adminID = "";
    LoginInfo.password = "";
    LoginInfo.computerName = "";

    LoginResult.loggedIn = 0;
    LoginResult.message = "";
    LoginResult.readonly = 1;
}

void AdminLoginPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    quint8 type = quint8(LOGIN_UNKNOWN);
    in >> type;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case LOGIN_REQUEST:
    {
        in >> LoginInfo.adminID >> LoginInfo.password >> LoginInfo.computerName;
    }
        break;

    case LOGIN_RESULT:
    {
        in >> LoginResult.loggedIn >> LoginResult.message >> LoginResult.readonly;
    }
        break;

    default:
        break;
    }

}

QByteArray AdminLoginPacket::packBodyData(){
    if(InfoType == LOGIN_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType);

    switch (InfoType) {
    case LOGIN_REQUEST:
    {
        out << LoginInfo.adminID << LoginInfo.password << LoginInfo.computerName;
    }
        break;

    case LOGIN_RESULT:
    {
        out << LoginResult.loggedIn << LoginResult.message << LoginResult.readonly;
    }
        break;

    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////














} //namespace HEHUI
