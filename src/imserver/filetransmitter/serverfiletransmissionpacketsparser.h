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

#ifndef SERVERFILETXPACKETSPARSER_H_
#define SERVERFILETXPACKETSPARSER_H_



#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QFile>


#include "../sharedim/constants_global_shared.h"
#include "../sharedim/filetransmitter/filetransmissionpacketsparser.h"

#include "../sharedim/resourcesmanagerinstance.h"
#include "../usersmanager/userinfo.h"

#include "HHSharedCore/hcryptography.h"
#include "HHSharedNetwork/hnetworkutilities.h"



namespace HEHUI {


class ServerFileTransmissionPacketsParser : public FileTransmissionPacketsParserBase{
    Q_OBJECT
public:
    ServerFileTransmissionPacketsParser(const QString &myID, QObject *parent = 0);
    virtual ~ServerFileTransmissionPacketsParser();



public slots:


private slots:




signals:



public:


private:







};

}

#endif /* SERVERFILETXPACKETSPARSER_H_ */
