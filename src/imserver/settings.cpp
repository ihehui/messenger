


#include "settings.h"

#include <QDate>


namespace HEHUI {

Settings::Settings( const QString& pName, const QString& pVersion, const QString fileBaseName, const QString fileDirPath, QObject* parent )
    : SettingsCore(pName, pVersion, fileBaseName, fileDirPath, parent )
{

    key = QString("HEHUI").toUtf8();

}

Settings::~Settings()
{

}

void Settings::setDataRootPath(const QString &dataRootPath){
    setValue("Server/DataRootPath", dataRootPath);

}

QString Settings::getDataRootPath() const{
    QString dataRootPath = QCoreApplication::applicationDirPath() + QDir::separator() + QString(LOCAL_SERVER_DATA_DIR_NAME);
    return value("Server/DataRootPath",dataRootPath).toString();

}

void Settings::setLocalDatabaseFilePath(const QString &databaseFilePath){
    setValue("Server/DatabaseFilePath", databaseFilePath);

}

QString Settings::getLocalDatabaseFilePath() const{
    QString databaseFilePath = getDataRootPath() + QDir::separator() + QString(LOCAL_SERVER_DB_NAME);
    return value("Server/DatabaseFilePath", databaseFilePath).toString();

}


void Settings::setDBType(unsigned int databaseType){
    setValue("Database/Type", databaseType);
}
DatabaseType Settings::getDBType() const{
    return DatabaseType(value("Database/Type", HEHUI::MYSQL).toUInt());
}

void Settings::setDBDriver(const QString &driverName)
{
    setValue("Database/Driver", driverName);
}
QString Settings::getDBDriver() const{
    return value("Database/Driver", REMOTE_SITOY_COMPUTERS_DB_DRIVER).toString();
}

void Settings::setDBServerHost(const QString &host){
    setValue("Database/Host", host);
}
QString Settings::getDBServerHost() const{
    return value("Database/Host", REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST).toString();
}

void Settings::setDBServerPort(quint16 port){
    setValue("Database/Port", port);
}
quint16 Settings::getDBServerPort(){
    return value("Database/Port", REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT).toUInt();
}

void Settings::setDBServerUserName(const QString &userName){
    QByteArray *destination = new QByteArray();
    Cryptography cryptography;
    cryptography.teaCrypto(destination, userName.toUtf8(), key, true);
    setValue("Database/UserName", *destination);
    delete destination;
}
QString Settings::getDBServerUserName() const{
    QString userName = "";
    QByteArray userNameArray = value("Database/UserName").toByteArray();
    if(userNameArray.isEmpty()){
        userName = REMOTE_SITOY_COMPUTERS_DB_USER_NAME;
    }else{
        QByteArray *destination = new QByteArray();
        Cryptography cryptography;
        cryptography.teaCrypto(destination, userNameArray, key, false);
        userName = QString(*destination);
        delete destination;

    }
    return userName;
}

void Settings::setDBServerUserPassword(const QString &userPassword){
    QByteArray *destination = new QByteArray();
    Cryptography cryptography;
    cryptography.teaCrypto(destination, userPassword.toUtf8(), key, true);
    setValue("Database/Password", *destination);
    delete destination;
}
QString Settings::getDBServerUserPassword() const{
    QString password = "";
    QByteArray passwordArray = value("Database/Password").toByteArray();
    if(passwordArray.isEmpty()){
        password = REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD;
    }else{
        QByteArray *destination = new QByteArray();
        Cryptography cryptography;
        cryptography.teaCrypto(destination, passwordArray, key, false);
        password = QString(*destination);
        delete destination;

    }
    return password;
}

void Settings::setDBName(const QString &databaseName){
    setValue("Database/DatabaseName", databaseName);
}
QString Settings::getDBName() const{
    return value("Database/DatabaseName", "im").toString();
}


void Settings::setChatImageCacheDir(const QString &dirPath){
    setValue("Image", dirPath);
    QDir dir;
    dir.mkpath(dirPath);
}
QString Settings::getChatImageCacheDir(){
    QString path = value("Image", QCoreApplication::applicationDirPath() + "/images").toString() ;
    if(!QFile::exists(path)){
        QDir dir;
        dir.mkpath(path);
    }
    return path;
}

void Settings::setUserIconDir(const QString &dirPath){
    setValue("UserIcon", dirPath);
    QDir dir;
    dir.mkpath(dirPath);
}
QString Settings::getUserIconDir(){
    QString path = value("UserIcon", QCoreApplication::applicationDirPath() + "/icons").toString();
    if(!QFile::exists(path)){
        QDir dir;
        dir.mkpath(path);
    }
    return path;
}

void Settings::setOfflineFileMainDirPath(const QString &dirPath){
    setValue("OfflineFilesDir", dirPath);
    QDir dir;
    dir.mkpath(dirPath);
}
QString Settings::getOfflineFileMainDirPath() const{
    QString path = value("OfflineFilesDir", QCoreApplication::applicationDirPath() + "/offline_files").toString();
    if(!QFile::exists(path)){
        QDir dir;
        dir.mkpath(path);
    }
    return path;
}











} //namespace HEHUI

