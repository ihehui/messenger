#include <QDir>
#include <QFile>
#include <QMainWindow>

#include "settings.h"

#include "HHSharedCore/hcryptography.h"


namespace HEHUI {

Settings::Settings(const QString &appName, const QString &appVersion, const QString fileBaseName, const QString fileDirPath, QObject* o )
    : SettingsBase(appName, appVersion, fileBaseName, fileDirPath, o )
{

    //beginGroup( mProgramVersion );
    
    key = QString("HEHUI@HEHUI").toUtf8();
    
    m_currentUserID = "default";
    
}

Settings::~Settings()
{
    //endGroup();

}

void Settings::setDataRootPath(const QString &dataRootPath){
    setValue("MainWindow/DataRootPath", dataRootPath);

}

QString Settings::getDataRootPath() const{
    QString dataRootPath = QApplication::applicationDirPath() + "/data";
    if(!QFile::exists(dataRootPath)){
        QDir dir;
        dir.mkpath(dataRootPath);
    }
    return value("MainWindow/DataRootPath",dataRootPath).toString();
}



//void Settings::setImageRootPath(const QString &imageRootPath){
//    setValue("MainWindow/ImageRootPath", imageRootPath);
//}



//void Settings::setEmoticonsPath(const QString &emoticonsPath){
//	setValue("MainWindow/EmoticonsPath", emoticonsPath);

//}

//QString Settings::getEmoticonsPath() const{
//	QString emoticonsPath = getImageRootPath() + "/emoticons";
//	return value("MainWindow/EmoticonsPath", emoticonsPath).toString();

//}



//void Settings::setPortraiticonsPath(const QString &portraiticonsPath){
//	setValue("MainWindow/PortraiticonsPath", portraiticonsPath);

//}

//QString Settings::getPortraiticonsPath() const{
//	QString portraiticonsPath = getImageRootPath() + "/portraiticons";
//	return value("MainWindow/PortraiticonsPath", portraiticonsPath).toString();

//}

QString Settings::getCurrentUserPrivateDataDir() const{
    QString userPrivateDataDir = getDataRootPath() + "/" + m_currentUserID;
    if(!QFile::exists(userPrivateDataDir)){
        QDir dir;
        dir.mkpath(userPrivateDataDir);
    }
    return userPrivateDataDir;

}

QString Settings::getCurrentUserPrivateDataFilePath() const
{
    //QString userPrivateDataFilePath = getDataRootPath() + QDir::separator() + userName + QDir::separator() + QString(LOCAL_USERDATA_DB_NAME);
    QString userPrivateDataFilePath = getCurrentUserPrivateDataDir() + "/" + QString(LOCAL_USERDATA_DB_NAME);
    return userPrivateDataFilePath;
}

QString Settings::getImageRootPath() const{
    QString path = getCurrentUserPrivateDataDir() + QString("/images");
    if(!QFile::exists(path)){
        QDir dir;
        dir.mkpath(path);
    }
    return path;
}


QString Settings::getImageCacheDir(){
    QString pictureCacheDir = getImageRootPath() + QString("/cache");
    if(!QFile::exists(pictureCacheDir)){
        QDir dir;
        dir.mkpath(pictureCacheDir);
    }
    return pictureCacheDir;
}

QString Settings::getCustomFaceDir(){
    QString pictureCacheDir = getImageRootPath() + QString("/face");
    if(!QFile::exists(pictureCacheDir)){
        QDir dir;
        dir.mkpath(pictureCacheDir);
    }
    return pictureCacheDir;
}

QString Settings::getCustomEmoticonsDir(){

    QString emoticonsDir = getImageRootPath() + QString("/customemoticons");
    if(!QFile::exists(emoticonsDir)){
        QDir dir;
        dir.mkpath(emoticonsDir);
    }
    return emoticonsDir;

}






void Settings::setRecentUser(const QString &userName){
    setValue("MainWindow/RecentUser", userName);
}

QString Settings::getRecentUser() const{
    return value("MainWindow/RecentUser").toString();

}

void Settings::setRecentUserPassword(const QString &password){

    QByteArray *destination = new QByteArray();
    Cryptography cryptography;
    cryptography.teaCrypto(destination, password.toUtf8(), key, true);
    setValue("MainWindow/RecentUserPassword", *destination);
    delete destination;

}

QString Settings::getRecentUserPassword() const{

    QByteArray passwordArray = value("MainWindow/RecentUserPassword").toByteArray();
    if(!passwordArray.isEmpty()){

        QByteArray *destination = new QByteArray();
        Cryptography cryptography;
        cryptography.teaCrypto(destination, passwordArray, key, false);
        passwordArray = *destination;
        delete destination;

    }

    return QString(passwordArray);

}

void Settings::setInvisibleLogin(bool invisible){
    setValue("MainWindow/InvisibleLogin", invisible);

}

bool Settings::getInvisibleLogin(){
    return value("MainWindow/InvisibleLogin", false).toBool();

}

void Settings::setAutoLogin(bool autoLogin){
    setValue("MainWindow/AutoLogin", autoLogin);

}

bool Settings::getAutoLogin() const{
    return value("MainWindow/AutoLogin", false).toBool();

}


//网络配置----
void Settings::setNetworkType(int index)
{
    setValue("Network/NetworkType", index);
}

int Settings::getNetworkType() const
{
    return value("Network/NetworkType", 0).toInt();


}

void Settings::setProtocol(int index)
{
    setValue("Network/Protocol", index);
}

int Settings::getProtocol() const
{
    return value("Network/Protocol", 0).toInt();

}


void Settings::setServers(QStringList &serversList)
{
    setValue("Network/Servers", serversList);
}

QStringList Settings::getServers() const
{
    return value("Network/Servers").toStringList();

}

//格式： IP:Port (eg. 192.168.1.1:12345)
void Settings::setLastServer(const QString &server)
{
    setValue("Network/LastServer", server);

}

QString Settings::getLastServer() const
{

    return value("Network/LastServer").toString();
}

void Settings::setSaveConfig(bool save)
{
    setValue("Network/SaveConfig", save);

}

bool Settings::getSaveConfig() const
{
    return value("Network/SaveConfig", true).toBool();
}
//----网络配置


void Settings::setCurrentUser(const QString &userID){
    this->m_currentUserID = userID;
}








} //namespace HEHUI
