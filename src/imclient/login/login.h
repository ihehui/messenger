/*
 * login.h
 *
 *  Created on: 2009-9-11
 *      Author: 贺辉
 */

#ifndef LOGIN_H_
#define LOGIN_H_


//#include <QLocalServer>
#include "../imuser.h"

//#include "../../shared/gui/login/loginbase.h"
#include "HHSharedGUI/hloginbase.h"


namespace HEHUI {

class Login :public LoginBase{

public:
        Login(IMUser *user, QObject *parent = 0);
	virtual ~Login();

	bool isVerified();

private:
	bool verifyUser();
	 //bool canLogin();

        IMUser *user;

	//QLocalServer *localServer;



};

} //namespace HEHUI

#endif /* LOGIN_H_ */
