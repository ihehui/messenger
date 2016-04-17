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
}

void DataForwardPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> data >> receiver;
}

QByteArray DataForwardPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << data << receiver;
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

    CaptchaInfo.type = quint8(CAPTCHA_REQUEST);
    CaptchaInfo.captchaImage = QByteArray();
    CaptchaInfo.captcha = "";
    CaptchaInfo.authResult = 0;

    AuthInfo.password = "";
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
    case INFO_TYPE_CAPTCHA:
    {
        in >> CaptchaInfo.type >> CaptchaInfo.captchaImage >> CaptchaInfo.captcha >> CaptchaInfo.authResult;
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
    case INFO_TYPE_CAPTCHA:
    {
        out << CaptchaInfo.type << CaptchaInfo.captchaImage << CaptchaInfo.captcha << CaptchaInfo.authResult;
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
        in >> GroupsList.groupsInfo >> GroupsList.version;
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
        out << GroupsList.groupsInfo << GroupsList.version;
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
    :IMPacket(base, quint8(IM::InterestCMD_GroupsInfo), sessionEncryptionKey)
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

    GroupInfo.id = 0;
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
    GroupCreationInfo.id = 0;

    GroupDeletionInfo.id = 0;
    GroupDeletionInfo.deleted = 0;

    MemberApplicationInfo.userID = "";
    MemberApplicationInfo.message = "";
    MemberApplicationInfo.approved = 0;
    MemberApplicationInfo.admin = "";

    MemberDeletionInfo.userID = "";
    MemberDeletionInfo.blockForever = "";
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
        in >> GroupInfo.id >> GroupInfo.infoString;
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
        in >> GroupCreationInfo.name >> GroupCreationInfo.type >> GroupCreationInfo.id;
    }
        break;
    case PIT_GROUP_DELETION:
    {
        in >> GroupDeletionInfo.id >> GroupDeletionInfo.deleted;
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
        out << GroupInfo.id << GroupInfo.infoString;
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
        out << GroupCreationInfo.name << GroupCreationInfo.type << GroupCreationInfo.id;
    }
        break;
    case PIT_GROUP_DELETION:
    {
        out << GroupDeletionInfo.id << GroupDeletionInfo.deleted;
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

    ContactFriendingRequest.message = 0;
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
        in >> ContactFriendingRequest.message >> ContactFriendingRequest.groupID;
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
        out << ContactFriendingRequest.message >> ContactFriendingRequest.groupID;
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

    out << quint8(InfoType) << JobID << ContactID;

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
    ChatImage.name = "";
    ChatImage.image = QByteArray();
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
        in >> ChatImage.isRequest >> ChatImage.name >> ChatImage.image;
    }
        break;


    default:
        break;
    }

}

QByteArray ChatMessagePacket::packBodyData(){
    if(InfoType == PIT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream (&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType) << JobID << ContactID;

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
        out << ChatImage.isRequest >> ChatImage.name << ChatImage.image;
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


////////////////////////////////////////////////////////////////////////
ClientInfoPacket::ClientInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ClientInfo), sessionEncryptionKey)
{

}

ClientInfoPacket::ClientInfoPacket(const PacketBase &base, const QByteArray &sessionEncryptionKey)
    :IMPacket(base, quint8(IM::CMD_ClientInfo), sessionEncryptionKey)
{


}

ClientInfoPacket::~ClientInfoPacket(){
    data.clear();
}

void ClientInfoPacket::init(){
    IsRequest = 0;
    assetNO = "";
    infoType = quint8(IM::SYSINFO_UNKNOWN);
    data = QByteArray();
}

void ClientInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> IsRequest >> assetNO >> infoType >> data;
}

QByteArray ClientInfoPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << IsRequest << assetNO << infoType << data;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
SystemInfoFromServerPacket::SystemInfoFromServerPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_SystemInfoFromServer))
{

}

SystemInfoFromServerPacket::SystemInfoFromServerPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_SystemInfoFromServer))
{


}

SystemInfoFromServerPacket::~SystemInfoFromServerPacket(){
    data.clear();
}

void SystemInfoFromServerPacket::init(){
    infoType = quint8(IM::SYSINFO_UNKNOWN);
    data = QByteArray();
    extraInfo = "";
}

void SystemInfoFromServerPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> infoType >> data >> extraInfo;
}

QByteArray SystemInfoFromServerPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << infoType << data << extraInfo;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
SysAdminInfoPacket::SysAdminInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_SysAdminInfo))
{

}

SysAdminInfoPacket::SysAdminInfoPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_SysAdminInfo))
{


}

SysAdminInfoPacket::~SysAdminInfoPacket(){
    data.clear();
}

void SysAdminInfoPacket::init(){
    adminID = "";
    data = QByteArray();
    deleteAdmin = 0;
}

void SysAdminInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> adminID >> data >> deleteAdmin;

}

QByteArray SysAdminInfoPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out<< adminID << data << deleteAdmin;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
SystemAlarmsPacket::SystemAlarmsPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_SystemAlarms))
{

}

SystemAlarmsPacket::SystemAlarmsPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_SystemAlarms))
{


}

SystemAlarmsPacket::~SystemAlarmsPacket(){

}

void SystemAlarmsPacket::init(){
    InfoType = SYSTEMALARMS_UNKNOWN;

    QueryInfo.assetNO = "";
    QueryInfo.type = "-1";
    QueryInfo.acknowledged = "-1";
    QueryInfo.startTime = "";
    QueryInfo.endTime = "";

    ACKInfo.alarms = "";
    ACKInfo.deleteAlarms = 0;
}

void SystemAlarmsPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    quint8 type = quint8(SYSTEMALARMS_UNKNOWN);
    in >> type;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case SYSTEMALARMS_QUERY:
    {
        in >> QueryInfo.assetNO >> QueryInfo.type >> QueryInfo.acknowledged >> QueryInfo.startTime >> QueryInfo.endTime;
    }
        break;

    case SYSTEMALARMS_ACK:
    {
        in >> ACKInfo.alarms >> ACKInfo.deleteAlarms;
    }
        break;

    default:
        break;
    }

}

QByteArray SystemAlarmsPacket::packBodyData(){
    if(InfoType == SYSTEMALARMS_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType);

    switch (InfoType) {
    case SYSTEMALARMS_QUERY:
    {
        out<< QueryInfo.assetNO << QueryInfo.type << QueryInfo.acknowledged << QueryInfo.startTime << QueryInfo.endTime;
    }
        break;

    case SYSTEMALARMS_ACK:
    {
        out << ACKInfo.alarms << ACKInfo.deleteAlarms;
    }
        break;

    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
RemoteConsolePacket::RemoteConsolePacket()
    :IMPacket(quint8(IM::CMD_RemoteConsole))
{

}

RemoteConsolePacket::RemoteConsolePacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_RemoteConsole))
{


}

void RemoteConsolePacket::init(){
    InfoType = REMOTECONSOLE_UNKNOWN;

    OpenConsole.applicationPath = "";
    OpenConsole.startProcess = 1;

    ConsoleState.isRunning = 0;
    ConsoleState.message = "";
    ConsoleState.messageType = quint8(IM::MSG_Information);

    Command.command = "";

    Output.output = "";
}

void RemoteConsolePacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    quint8 type = quint8(REMOTECONSOLE_UNKNOWN);
    in >> type;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case REMOTECONSOLE_OPEN:
    {
        in >> OpenConsole.applicationPath >> OpenConsole.startProcess;
    }
        break;

    case REMOTECONSOLE_STATE:
    {
        in >> ConsoleState.isRunning >> ConsoleState.message >> ConsoleState.messageType;
    }
        break;

    case REMOTECONSOLE_COMMAND:
    {
        in >> Command.command;
    }
        break;

    case REMOTECONSOLE_OUTPUT:
    {
        in >> Output.output;
    }
        break;

    default:
        break;
    }

}

QByteArray RemoteConsolePacket::packBodyData(){
    if(InfoType == REMOTECONSOLE_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType);

    switch (InfoType) {
    case REMOTECONSOLE_OPEN:
    {
        out << OpenConsole.applicationPath << OpenConsole.startProcess;
    }
        break;

    case REMOTECONSOLE_STATE:
    {
        out << ConsoleState.isRunning << ConsoleState.message << ConsoleState.messageType;
    }
        break;

    case REMOTECONSOLE_COMMAND:
    {
        out << Command.command ;
    }
        break;

    case REMOTECONSOLE_OUTPUT:
    {
        out << Output.output;
    }
        break;

    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ClientLogPacket::ClientLogPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ClientLog))
{

}

ClientLogPacket::ClientLogPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_ClientLog))
{


}

void ClientLogPacket::init(){
    log = "";
    logType = quint8(IM::LOG_Unknown) ;
}

void ClientLogPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> logType >> log;
}

QByteArray ClientLogPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << logType << log;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
USBDevPacket::USBDevPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_FileTransfer))
{

}

USBDevPacket::USBDevPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_FileTransfer))
{


}

void USBDevPacket::init(){
    usbSTORStatus = quint8(IM::USBSTOR_Unknown);
}

void USBDevPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> usbSTORStatus;
}

QByteArray USBDevPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << usbSTORStatus;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
AdminConnectionToClientPacket::AdminConnectionToClientPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_AdminConnectionToClient))
{

}

AdminConnectionToClientPacket::AdminConnectionToClientPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_AdminConnectionToClient))
{


}

void AdminConnectionToClientPacket::init(){
    computerName = "";
    adminID = "";
    verified = 0;
    errorCode = quint8(IM::ERROR_NO_ERROR);
}

void AdminConnectionToClientPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> adminID >> computerName >> verified >> errorCode;
}

QByteArray AdminConnectionToClientPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << adminID << computerName << verified << errorCode;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
AdminSearchClientPacket::AdminSearchClientPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_AdminSearchClient))
{

}

AdminSearchClientPacket::AdminSearchClientPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_AdminSearchClient))
{


}

void AdminSearchClientPacket::init(){
    computerName = "";
    userName = "";
    workgroup = "";
    macAddress = "";
    ipAddress = "";
    osVersion = "";
    adminID = "";
}

void AdminSearchClientPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> computerName >> userName >> workgroup >> macAddress >> ipAddress >> osVersion >> adminID;
}

QByteArray AdminSearchClientPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << computerName << userName << workgroup << macAddress << ipAddress << osVersion << adminID;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
LocalUserOnlineStatusChangedPacket::LocalUserOnlineStatusChangedPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_LocalUserOnlineStatusChanged))
{

}

LocalUserOnlineStatusChangedPacket::LocalUserOnlineStatusChangedPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_LocalUserOnlineStatusChanged))
{


}

void LocalUserOnlineStatusChangedPacket::init(){
    userName = "";
    online = 0;
}

void LocalUserOnlineStatusChangedPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> userName >> online;
}

QByteArray LocalUserOnlineStatusChangedPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << userName << online;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
FileTransferPacket::FileTransferPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_FileTransfer))
{

}

FileTransferPacket::FileTransferPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_FileTransfer))
{


}

void FileTransferPacket::init(){
    InfoType = FT_UNKNOWN;

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
    FileDownloadingResponse.pathToSaveFile = "";

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
    in >> type;

    InfoType = PacketInfoType(type);
    switch (InfoType) {
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
        in >> FileDownloadingResponse.accepted >> FileDownloadingResponse.baseDir >> FileDownloadingResponse.fileName >> FileDownloadingResponse.fileMD5Sum >> FileDownloadingResponse.size >> FileDownloadingResponse.pathToSaveFile;
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
    out << quint8(InfoType);

    switch (InfoType) {
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
        out << FileDownloadingResponse.accepted << FileDownloadingResponse.baseDir << FileDownloadingResponse.fileName << FileDownloadingResponse.fileMD5Sum << FileDownloadingResponse.size << FileDownloadingResponse.pathToSaveFile;
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
ModifyAssetNOPacket::ModifyAssetNOPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ModifyAssetNO))
{

}

ModifyAssetNOPacket::ModifyAssetNOPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_ModifyAssetNO))
{


}

void ModifyAssetNOPacket::init(){
    isRequest = 1;
    oldAssetNO = "";
    newAssetNO = "";
    adminID = "";
}

void ModifyAssetNOPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> isRequest >> oldAssetNO >> newAssetNO >> adminID;
}

QByteArray ModifyAssetNOPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << isRequest << oldAssetNO << newAssetNO << adminID;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
RenameComputerPacket::RenameComputerPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_RenameComputer))
{

}

RenameComputerPacket::RenameComputerPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_RenameComputer))
{


}

void RenameComputerPacket::init(){
    assetNO = "";
    newComputerName = "";
    domainAdminName = "";
    domainAdminPassword = "";
}

void RenameComputerPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> assetNO >> newComputerName >> domainAdminName >> domainAdminPassword;
}

QByteArray RenameComputerPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << assetNO << newComputerName << domainAdminName << domainAdminPassword;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
JoinOrUnjoinDomainPacket::JoinOrUnjoinDomainPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_JoinOrUnjoinDomain))
{

}

JoinOrUnjoinDomainPacket::JoinOrUnjoinDomainPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_JoinOrUnjoinDomain))
{


}

void JoinOrUnjoinDomainPacket::init(){
    assetNO = "";
    domainOrWorkgroupName = "";
    domainAdminName = "";
    domainAdminPassword = "";
    joinWorkgroup = 1;
}

void JoinOrUnjoinDomainPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> assetNO >> domainOrWorkgroupName >> domainAdminName >> domainAdminPassword >> joinWorkgroup;
}

QByteArray JoinOrUnjoinDomainPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << assetNO << domainOrWorkgroupName << domainAdminName << domainAdminPassword << joinWorkgroup;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
TemperaturesPacket::TemperaturesPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Temperatures))
{

}

TemperaturesPacket::TemperaturesPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_Temperatures))
{


}

void TemperaturesPacket::init(){
    InfoType = TEMPERATURES_UNKNOWN;

    TemperaturesRequest.requestCPU = true;
    TemperaturesRequest.requestHD = false;

    TemperaturesResponse.cpuTemperature = "";
    TemperaturesResponse.harddiskTemperature = "";
}

void TemperaturesPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    quint8 type = quint8(TEMPERATURES_UNKNOWN);
    in >> type;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case TEMPERATURES_REQUEST:
    {
        in >> TemperaturesRequest.requestCPU >> TemperaturesRequest.requestCPU;
    }
        break;

    case TEMPERATURES_RESPONSE:
    {
        in >> TemperaturesResponse.cpuTemperature >> TemperaturesResponse.harddiskTemperature;
    }
        break;


    default:
        break;
    }

}

QByteArray TemperaturesPacket::packBodyData(){
    if(InfoType == TEMPERATURES_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType);

    switch (InfoType) {
    case TEMPERATURES_REQUEST:
    {
        out << TemperaturesRequest.requestCPU << TemperaturesRequest.requestCPU;
    }
        break;

    case TEMPERATURES_RESPONSE:
    {
        out << TemperaturesResponse.cpuTemperature << TemperaturesResponse.harddiskTemperature;
    }
        break;


    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ScreenshotPacket::ScreenshotPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Screenshot))
{

}

ScreenshotPacket::ScreenshotPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_Screenshot))
{


}

void ScreenshotPacket::init(){
    InfoType = SCREENSHOT_UNKNOWN;

    ScreenshotRequest.adminID = "";
    ScreenshotRequest.userName = "";
    ScreenshotRequest.adminListeningPort = 0;

    DesktopInfo.desktopWidth = 0;
    DesktopInfo.desktopHeight = 0;
    DesktopInfo.blockWidth = 0;
    DesktopInfo.blockHeight = 0;
}

void ScreenshotPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    quint8 type = quint8(SCREENSHOT_UNKNOWN);
    in >> type ;
    InfoType = PacketInfoType(type);

    switch (InfoType) {
    case SCREENSHOT_REQUEST:
    {
        in >> ScreenshotRequest.adminID >> ScreenshotRequest.userName >> ScreenshotRequest.adminListeningPort;
    }
        break;

    case SCREENSHOT_DESKTOP_INFO:
    {
        in >> DesktopInfo.desktopWidth >> DesktopInfo.desktopHeight >> DesktopInfo.blockWidth >> DesktopInfo.blockHeight;
    }
        break;

    case SCREENSHOT_DATA:
    {
        while (!in.atEnd()) {
            int x = 0, y = 0;
            QByteArray image;

            in >> x >> y >> image;

            ScreenshotData.locations.append(QPoint(x, y));
            ScreenshotData.images.append(image);
        }
    }
        break;


    default:
        break;
    }

}

QByteArray ScreenshotPacket::packBodyData(){
    if(InfoType == SCREENSHOT_UNKNOWN){return QByteArray();}

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint8(InfoType);

    switch (InfoType) {
    case SCREENSHOT_REQUEST:
    {
        out << ScreenshotRequest.adminID << ScreenshotRequest.userName << ScreenshotRequest.adminListeningPort;
    }
        break;

    case SCREENSHOT_DESKTOP_INFO:
    {
        out << DesktopInfo.desktopWidth << DesktopInfo.desktopHeight << DesktopInfo.blockWidth << DesktopInfo.blockHeight;
    }
        break;

    case SCREENSHOT_DATA:
    {
        for(int i=0; i<ScreenshotData.locations.size(); i++){
            QPoint point = ScreenshotData.locations.at(i);
            out <<  point.x() << point.y() << ScreenshotData.images.at(i);
        }
    }
        break;


    default:
        break;
    }

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ShutdownPacket::ShutdownPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_Shutdown))
{

}

ShutdownPacket::ShutdownPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_Shutdown))
{


}

void ShutdownPacket::init(){
    message = "";
    waitTime = 0;
    force = 1;
    reboot = 0 ;
}

void ShutdownPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> message >> waitTime >> force >> reboot ;
}

QByteArray ShutdownPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << message << waitTime << force << reboot ;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
LockWindowsPacket::LockWindowsPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_LockWindows))
{

}

LockWindowsPacket::LockWindowsPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_LockWindows))
{


}

void LockWindowsPacket::init(){
    userName = "";
    logoff = 0;
}

void LockWindowsPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> userName >> logoff ;
}

QByteArray LockWindowsPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << userName << logoff ;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
WinUserPacket::WinUserPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_WinUser))
{

}

WinUserPacket::WinUserPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_WinUser))
{


}

void WinUserPacket::init(){
    userData = 0;
}

void WinUserPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> userData ;
}

QByteArray WinUserPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << userData ;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ServiceConfigPacket::ServiceConfigPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ServiceConfig))
{

}

ServiceConfigPacket::ServiceConfigPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_ServiceConfig))
{


}

void ServiceConfigPacket::init(){
    serviceName = "";
    startupType = 0xFFFFFFFF;
    startService = 0;
    processID = 0;
}

void ServiceConfigPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> serviceName >> startupType >> startService >> processID;
}

QByteArray ServiceConfigPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << serviceName << startupType << startService << processID;

    return ba;
}
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
ProcessMonitorInfoPacket::ProcessMonitorInfoPacket(const QByteArray &sessionEncryptionKey)
    :IMPacket(quint8(IM::CMD_ProcessMonitorInfo))
{

}

ProcessMonitorInfoPacket::ProcessMonitorInfoPacket(const PacketBase &base)
    :IMPacket(quint8(IM::CMD_ProcessMonitorInfo))
{


}

void ProcessMonitorInfoPacket::init(){
    localRules = QByteArray();
    globalRules = QByteArray();
    enableProcMon = 0;
    enablePassthrough = 1;
    enableLogAllowedProcess = 0;
    enableLogBlockedProcess = 1;
    useGlobalRules = 1;
    assetNO = "";
}

void ProcessMonitorInfoPacket::parsePacketBody(QByteArray &packetBody){
    QDataStream in(&packetBody, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    in >> localRules >> globalRules >> enableProcMon >> enablePassthrough >> enableLogAllowedProcess >> enableLogBlockedProcess >>useGlobalRules >>assetNO;
}

QByteArray ProcessMonitorInfoPacket::packBodyData(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << localRules << globalRules << enableProcMon << enablePassthrough << enableLogAllowedProcess << enableLogBlockedProcess << useGlobalRules << assetNO;

    return ba;
}
////////////////////////////////////////////////////////////////////////




} //namespace HEHUI
