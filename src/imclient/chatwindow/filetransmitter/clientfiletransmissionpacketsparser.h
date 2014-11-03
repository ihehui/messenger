/*
 ****************************************************************************
 * filetransmissionpacketsparser.h
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

#ifndef CLIENTFILETXPACKETSPARSER_H_
#define CLIENTFILETXPACKETSPARSER_H_



#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QFile>


#include "../sharedim/constants_global_shared.h"
#include "../sharedim/filetransmitter/filetransmissionpacketsparser.h"


#include "../networkmanager/clientresourcesmanager.h"
#include "imuser.h"
#include "../../contactsmanager/contact.h"

#include "HHSharedCore/hcryptography.h"
#include "HHSharedNetwork/hnetworkutilities.h"



namespace HEHUI {


class ClientFileTransmissionPacketsParser : public FileTransmissionPacketsParserBase{
    Q_OBJECT
public:
    ClientFileTransmissionPacketsParser(const QString &myID, QObject *parent = 0);
    virtual ~ClientFileTransmissionPacketsParser();



public slots:
    int connectToServer();

//    virtual void parseOtherIncomingPacketData(Packet *packet);



private slots:




signals:



public:




private:

    int m_socketConnectedToServer;



};

}

#endif /* CLIENTFILETXPACKETSPARSER_H_ */
