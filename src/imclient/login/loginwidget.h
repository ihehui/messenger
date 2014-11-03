#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QSqlError>
#include <QDialog>
#include <QMessageBox>
#include <QLocalServer>

#include "ui_loginwidget.h"
#include "../imuser.h"
//#include "../packetmanager/clientpacketsparser.h"


//#include "../../shared/core/singleton.h"
//#include "../../shared/core/utilities.h"
//#include "../../shared/gui/databaseconnecter/databaseconnecterdialog.h"
#include "HHSharedCore/hsingleton.h"
#include "HHSharedCore/hutilities.h"
//#include "HHSharedGUI/hdatabaseconnecter.h"


namespace HEHUI {


class LoginWidget: public QWidget {
    Q_OBJECT

public:
    enum State {NORMAL, VERIFYING, LOCKED};

    LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

    //	void setUser(IMUser *u) {user = u;};
    //bool isInfoAccepted(){return infoAccepted;};

    State getState() const;

    void switchUI(State state = NORMAL);
    void lockUI(const QString &key = "");

protected:
    void keyPressEvent(QKeyEvent *);
    void languageChange();

private:
    void setupNetworkConfig();
    void loadServers();

    inline QString userName() const;
    inline QString passWord() const;

    inline QString serverAddress();
    inline quint16 serverPort();

    void showNormalUI();
    void showVerifyingUI();
    void showLockedUI();

    void unLockUI(const QString &key);

    bool checkServerAddress();
    bool canLogin();
    void verifyUser();



signals:
    void requestRegistrationServerInfo();
    void registration();
    void signalRegistrationServerInfoReceived(quint8 errorTypeCode, bool canRegister, const QString &extraMessage, quint8 regMode, const QString &regServerAddress, bool requireActivation);
    void signalRegistrationResultReceived(quint8 errorTypeCode, quint32 sysID, const QString &message);

    void signalRequestLogin(const QHostAddress &serverHostAddress, quint16 serverHostPort);
    void signalLookForServer(const QHostAddress &targetAddress, quint16 targetPort);
    void signalUserVerified();
    void signalServerFound(const QString &serverAddress, quint16 serverRTPListeningPort, const QString &serverName, const QString &version);

    void signalKickedOff();

public slots:

    //        void serverFound(const QString &serverAddress, quint16 serverUDPListeningPort, quint16 serverTCPListeningPort, const QString &serverName, const QString &version);
    void slotProcessLoginResult(quint8 errorTypeCode, const QString &errorMessage = "");

    void slotCheckAutoLogin();

private slots:
    void on_toolButtonApplyForRegistration_clicked();
    void on_invisibleCheckBox_clicked();
    void on_autoLoginCheckBox_clicked();
    void on_toolButtonConfig_clicked(bool checked);
    void on_loginToolButton_clicked();
    void on_cancelToolButton_clicked();

    void on_comboBoxNetworkType_currentIndexChanged(int index);
    void on_comboBoxProtocol_currentIndexChanged(const QString & text);
    void on_comboBoxServerIP_currentIndexChanged(int index);

    void on_toolButtonServersManager_clicked();


    void on_unlockToolButton_clicked();

    void slotServersUpdated();
    void slotServerSelected(const QString &serverInfoString);
    void updateUserLoginServerInfo();

    void loginTimeout();


private:
    Ui::LoginWidgetClass ui;

    IMUser *user;

    State currentState;
    //bool infoAccepted;

    QString key;

    QMovie *movie;

    bool invisibleLogin;
    bool autoLogin;

    QLocalServer *localServer;

    //ClientPacketsParser *clientPacketsParser;



};

} //namespace HEHUI

#endif // LOGINWIDGET_H
