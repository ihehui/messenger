/*
 ****************************************************************************
 * serverservice.cpp
 *
 * Created on: 2010-7-12
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
 * Last Modified on: 2010-9-3
 * Last Modified by: 贺辉
 ***************************************************************************
 */




#include <QtCore>
#include "serverservice.h"

#include "../app_constants_imserverservice.h"
//#include "../settings.h"





namespace HEHUI {


ServerService::ServerService(int argc, char **argv, const QString &serviceName, const QString &description )
    :Service(argc, argv, serviceName, description)
{


    setStartupType(QtServiceController::AutoStartup);
    //    setServiceFlags(CanBeSuspended);

    m_server = new IMServer(this);

    //    mainServiceStarted = false;

}

ServerService::~ServerService(){
    //    mainServiceStarted = false;
    if(m_server){
        delete m_server;
        m_server = 0;
    }

}

void ServerService::setServerTye(ServerType serverType){
    m_server->setServerTye(serverType);
}

//bool ServerService::startMainService(){
//    qDebug()<<"----ServerService::startMainService()";

//    if(mainServiceStarted){
//        qWarning()<<"Main service has already started!";
//        return true;
//    }


//    mainServiceStarted = true;

//    return true;
//}


void ServerService::start()
{
    //    mainServiceStarted = false;
    m_server->start();
}

void ServerService::stop()
{
    m_server->stop();
    
}

void ServerService::pause()
{

}

void ServerService::resume()
{

}

void ServerService::processCommand(int code)
{

    qDebug()<<"----ServerService::processCommand(int code)";
    qDebug()<<"code:"<<code;


    switch(code){
    case 0:

        break;
    case 1:

        break;
    case 2:

        break;
    default:
        qWarning()<<QString("Unknown Command Code '%1'!").arg(code);
        break;

    }


}





























} //namespace HEHUI









