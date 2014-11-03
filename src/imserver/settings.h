#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QSettings>
#include <QCoreApplication>
#include <QObject>
#include <QDir>

#include "app_constants_server.h"


#include "HHSharedCore/hsettingscore.h"
#include "HHSharedCore/hcryptography.h"

#include "serverlib.h"
 

namespace HEHUI{


class SERVERLIB_API Settings : public SettingsCore
{
	Q_OBJECT

public:
    Settings( const QString &appName = APP_NAME, const QString &appVersion = APP_VERSION, const QString fileBaseName = APP_NAME, const QString fileDirPath = QCoreApplication::applicationDirPath(), QObject *parent = 0 );
    ~Settings();


    void setDataRootPath(const QString &dataRootPath);

    QString getDataRootPath() const;

    void setLocalDatabaseFilePath(const QString &databaseFilePath);

    QString getLocalDatabaseFilePath() const;


    void setDBType(unsigned int databaseType);
    DatabaseType getDBType() const;

    void setDBDriver(const QString &driverName);
    QString getDBDriver() const;

    void setDBServerHost(const QString &host);
    QString getDBServerHost() const;

    void setDBServerPort(quint16 port);
    quint16 getDBServerPort();

    void setDBServerUserName(const QString &userName);
    QString getDBServerUserName() const;

    void setDBServerUserPassword(const QString &userPassword);
    QString getDBServerUserPassword() const;

    void setDBName(const QString &databaseName);
    QString getDBName() const;

    void setChatImageCacheDir(const QString &dirPath);
    QString getChatImageCacheDir();

    void setUserIconDir(const QString &dirPath);
    QString getUserIconDir();

    void setOfflineFileMainDirPath(const QString &dirPath);
    QString getOfflineFileMainDirPath() const;



private:



private:
    QByteArray key;


};

} //namespace HEHUI

#endif // SERVERSETTINGS_H
