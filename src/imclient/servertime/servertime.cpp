/*
 ****************************************************************************
 * filename
 *
 * Created on: 2013-10-10
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
 * Last Modified on: 2013-10-10
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#include "servertime.h"



namespace HEHUI {


ServerTime * ServerTime::m_instance = 0;


ServerTime * ServerTime::instance(){
    if(m_instance == 0){
        m_instance = new ServerTime();
    }

    return m_instance;

}

void ServerTime::destoryInstance(){
    delete m_instance;
    m_instance = 0;
}

void ServerTime::startSync(uint time_t){

    if(!m_syncTimer){
        m_syncTimer = new QTimer(this);
        m_syncTimer->setInterval(1000);
        connect(m_syncTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    }

    m_syncTimer->start();
    count = 0;

    m_startTime = QDateTime::fromTime_t(time_t);

}

QDateTime ServerTime::time() {
    return m_startTime.addSecs(count);
}

QString ServerTime::timeString(const QString &format) const{
    return m_startTime.addSecs(count).toString(format);
}

ServerTime::ServerTime()
    :QObject()
{

    m_syncTimer = 0;
    count = 0;

}

ServerTime::~ServerTime()
{

    if(m_syncTimer){
        m_syncTimer->stop();
        delete m_syncTimer;
    }

    m_syncTimer = 0;
    count = 0;

}


void ServerTime::timeout(){
    count++;
}





} //namespace HEHUI
