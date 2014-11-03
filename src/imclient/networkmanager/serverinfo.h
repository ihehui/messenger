/*
 ****************************************************************************
 * serverinfo.h
 *
 * Created On: 2010-5-24
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
 * Last Modified On: 2010-5-24
 * Last Modified By: 贺辉
 ***************************************************************************
 */

#ifndef SERVERINFO_H_
#define SERVERINFO_H_

#include <QObject>
#include <QString>


namespace HEHUI {

class ServerInfo : public QObject{

public:
	enum State{NotTested, Testing, TestOK, TestFailed};

	ServerInfo(const QString &ip, uint port, QObject *parent = 0);
	virtual ~ServerInfo();


    State getCurState() const
    {
        return curState;
    }

    QString getErrorInfo() const
    {
        return errorInfo;
    }

    QString getIp() const
    {
        return ip;
    }

    uint getPort() const
    {
        return port;
    }

    bool getValid() const
    {
        return valid;
    }

    void setCurState(State curState)
    {
        this->curState = curState;
    }

    void setErrorInfo(QString errorInfo)
    {
        this->errorInfo = errorInfo;
    }

    void setIp(QString ip)
    {
        this->ip = ip;
    }

    void setPort(uint port)
    {
        this->port = port;
    }

    void setValid(bool valid)
    {
        this->valid = valid;
    }

private:
	QString ip;
	uint port;
	bool valid;

	State curState;

	QString errorInfo;


};

}

#endif /* SERVERINFO_H_ */
