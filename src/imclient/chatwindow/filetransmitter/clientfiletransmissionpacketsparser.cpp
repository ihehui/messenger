/*
 ****************************************************************************
 * filetransmissionpacketsparser.cpp
 *
 * Created On: 2010-7-13
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
 * Last Modified On: 2012-3-4
 * Last Modified By: 贺辉
 ***************************************************************************
 */

#include "clientfiletransmissionpacketsparser.h"


#include <QNetworkInterface>


namespace HEHUI {


ClientFileTransmissionPacketsParser::ClientFileTransmissionPacketsParser(const QString &myID, QObject *parent)
    :FileTransmissionPacketsParserBase(myID, parent)
{

    m_socketConnectedToServer = INVALID_SOCK_ID;


}

ClientFileTransmissionPacketsParser::~ClientFileTransmissionPacketsParser() {
    // TODO Auto-generated destructor stub




}

int ClientFileTransmissionPacketsParser::connectToServer(){
    IMUser *myself = IMUser::instance();
    m_socketConnectedToServer = connectToPeer(QHostAddress(myself->getFileServerAddress()), myself->getFileServerPort());
    return m_socketConnectedToServer;
}










} //namespace HEHUI
