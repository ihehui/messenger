#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QApplication>

#include "app_constants_client.h"

//#include "../../shared/core/singleton.h"
//#include "../../shared/gui/settingsbase.h"
#include "HHSharedCore/hsingleton.h"
#include "HHSharedGUI/hsettingsbase.h"


 //#include "mysharedlib_global.h"
	//MYSHAREDLIB_API



namespace HEHUI {

class  Settings : public SettingsBase, public Singleton<Settings>
{
    Q_OBJECT
    friend class Singleton<Settings>;

public:
    Settings(const QString &appName = APP_NAME, const QString &appVersion = APP_VERSION, const QString fileBaseName = APP_NAME, const QString fileDirPath = QCoreApplication::applicationDirPath(), QObject* = QApplication::instance() );
    ~Settings();


    void setDataRootPath(const QString &dataRootPath);
    QString getDataRootPath() const;

//    void setImageRootPath(const QString &imageRootPath);

    //	void setEmoticonsPath(const QString &emoticonsPath);
    //	QString getEmoticonsPath() const;

    //	void setPortraiticonsPath(const QString &portraiticonsPath);
    //	QString getPortraiticonsPath() const;


    QString getCurrentUserPrivateDataDir() const;
    QString getCurrentUserPrivateDataFilePath() const;

    QString getImageRootPath() const;
    QString getImageCacheDir();
    QString getCustomFaceDir();
    QString getCustomEmoticonsDir();



    //登陆框
    void setRecentUser(const QString &userName);
    QString getRecentUser() const;
    void setRecentUserPassword(const QString &password);
    QString getRecentUserPassword() const;

    void setInvisibleLogin(bool invisible);
    bool getInvisibleLogin();

    void setAutoLogin(bool autoLogin);
    bool getAutoLogin() const;
    //----登陆框
    //网络配置----
    void setNetworkType(int index);
    int getNetworkType() const;

    void setProtocol(int index);
    int getProtocol() const;

    void setServers(QStringList &serversList);
    QStringList getServers() const;

    void setLastServer(const QString &server);
    QString getLastServer() const;

    void setSaveConfig(bool save);
    bool getSaveConfig() const;
    //----网络配置

    void setCurrentUser(const QString &userID);

    //	virtual void restoreState( QMainWindow* );
    //	virtual void saveState( QMainWindow* );
    //
    //	void setRestoreWindowStateOnStartup(bool restore);
    //	bool getRestoreWindowStateOnStartup();

private:
    QByteArray key;
    //QString userPrivateDataFilePath;
    //static QString mProgramName;
    //static QString mProgramVersion;

    //ResourcesManager *resourcesManager;

    QString m_currentUserID;

};

} //namespace HEHUI

#endif // SETTINGS_H
