/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
 *   yunfan_zg@163.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QTextStream>
#include <QDebug>


#include "evaresource.h"
#include "../../settings.h"

//#include "evasetting.h"
//#include "evaservers.h"
//#include "evasocket.h"
//#include "evamain.h"
//#include "evauser.h"

#include <qapplication.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qdatastream.h>

using namespace HEHUI;

EvaImageResource::EvaImageResource()
{

	themePath = "";
	//imageRoot = EvaGlobal::getDataDirPath() + "/images";
	imageRoot = Settings::instance()->getImageRootPath();
	qDebug()<<"----EvaImageResource::imageRoot()~~imageRoot:"<<imageRoot;

	isDownloadingQQShow = false;
	//http = new EvaHttp();
	//connect ( http, SIGNAL(requestFinished(bool)), SLOT(slotQQShowDone(bool)));
}

EvaImageResource::~EvaImageResource()
{
}

const QString EvaImageResource::getPortraitsPath() const
{
	return imageRoot + "/portraits";
}

const QString EvaImageResource::getIconPath()
{
	if(themePath == "" )
		themePath = imageRoot + "/theme";
	return themePath;
}

const QString EvaImageResource::getEmoticonsPath() const
{
	return imageRoot + "/emoticons";
}

/*const*/ int EvaImageResource::getFaceID(const int fileIndex) const
{
	if(fileIndex<=0 || fileIndex> MaxFaceNumber)
		return faceId[0];
	return faceId[fileIndex-1];
}

/*const*/ int EvaImageResource::getFaceFileIndex(const int faceId)
{
	int index = (faceId<0)?1:faceId/3 + 1;
	if(index<0 || index> MaxFaceNumber) index = 1;
	return index;
}

QPixmap *EvaImageResource::getFace(const int fileIndex, const bool on)
{
	QString key = QString::number((fileIndex>0)?fileIndex:1);
	if(!on) key+="off";
	QMap<QString, QPixmap>::Iterator iter = faceList.find(key);
	if(iter== faceList.end()) return NULL;
	return &(iter.value());
}

QPixmap *EvaImageResource::getFaceByID( const unsigned short faceId, const bool on)
{
	return getFace(getFaceFileIndex(faceId), on);
}

QPixmap *EvaImageResource::getIcon(QString name)
{
	QMap<QString, QPixmap>::Iterator iter = iconList.find(name);
	if(iter== iconList.end()) return NULL;
	return &(iter.value());
}

const QString EvaImageResource::getIconFullPath(QString name)
{
	QMap<QString, QString>::Iterator iter = iconFileNameMap.find(name);
	if(iter== iconFileNameMap.end()) return "";
	return getIconPath() + "/" + iter.value();
}

const QString EvaImageResource::getSmiley(const int fileIndex)
{

	//return QString(":/data/image/smiley/%1.gif").arg(fileIndex);
	return getEmoticonsPath() + QString("/%1.gif").arg(fileIndex);

//	QString key = QString::number(fileIndex);
//	QMap<QString, QMovie>::Iterator iter = smileyList.find(key);
//	if(iter== smileyList.end()) return NULL;
//	return &(iter.value());
}

const QMovie *EvaImageResource::getLoginMovie()
{
	return loginMng;
}

 bool EvaImageResource::loadImage()
{
	return loadIcon();
	//return (loadFace() && loadIcon() );
	//return (loadFace() && loadIcon() && loadSmiley());
}

bool EvaImageResource::loadFace(const QSize &size)
{
	QString path = getPortraitsPath();
	QFile file( path + "/face.theme");
	if(!file.open(QIODevice::ReadOnly)){
		return false;
	}
	faceList.clear();
	QTextStream stream(&file);
	QString line, imagePath;
	QStringList lineList;
	while(!stream.atEnd()){
		line = stream.readLine().trimmed();

		lineList = line.split(":"); //QStringList::split(":", line);
		if(lineList.size() != 2)
			continue;

		lineList[0].trimmed();
		imagePath = path + "/" + lineList[1].trimmed();
		QPixmap img(imagePath);
		if(!img.isNull()){
//			faceList[lineList[0]] = img.convertToImage().smoothScale(size);
			faceList[lineList[0]] = img;
                }
	}

	file.close();

	for(int i = 0; i < MaxFaceNumber ; i++) {
		faceId[i] = i * 3 + 1;
	}
	return true;
}

bool EvaImageResource::loadIcon()
{
	QString path = getIconPath() + "/eva.theme";
	QDir d;
	if(!d.exists(path)){
		themePath = ""; // if theme not exist, load default one
		path = getIconPath() + "/eva.theme";
	}
	QFile file( path);
	if(!file.open(QIODevice::ReadOnly)){
		return false;
	}

	QTextStream stream(&file);
	QString line, imagePath;
	QStringList lineList;

	while(!stream.atEnd()){
		line = stream.readLine().trimmed();

		lineList = line.split(":");

		if(lineList.size() != 2)
			continue;

		lineList[0].trimmed();
		imagePath = getIconPath() + "/" + lineList[1].trimmed();
		if(lineList[0] == "LOGIN_MNG"){
			loginMng = new QMovie(imagePath);
			continue;
		}
		QPixmap img(imagePath);
		if(!img.isNull()){
			iconList[lineList[0]] = img;
			iconFileNameMap[lineList[0]] = lineList[1].trimmed();
		}
	}

	file.close();
	return true;
}

bool EvaImageResource::loadSmiley()
{
	QString path = getEmoticonsPath();
	QFile file( path + "/smiley.theme");
	if(!file.open(QIODevice::ReadOnly)){
		return false;
	}

	QTextStream stream(&file);
	QString line, imagePath;
	QStringList lineList;

	while(!stream.atEnd()){
		line = stream.readLine().trimmed();

		lineList = line.split(":");

		if(lineList.size() != 2)
			continue;

		lineList[0].trimmed();
		imagePath = path + "/" + lineList[1].trimmed();
		QMovie *m = new QMovie(imagePath);
		if(m->isValid ())
			smileyList[lineList[0]] = m;
	}

	file.close();
	return true;
}

const QString EvaImageResource::getQQShowPath() const
{
	QString evaHome = QDir::homePath() + "/.eva";
	QDir d;
	if(!d.exists(evaHome)){
		return "";
	}
	if(!d.cd(evaHome)) return "";
	if(!d.exists("QQShow")){
		if(!d.mkdir("QQShow"))	return "";
	}
	return evaHome + "/QQShow";
}

QPixmap *EvaImageResource::getQQShow(const unsigned int id)
{
	QString path = getQQShowPath() +"/"+ QString::number(id) + ".gif";
	QDir d;
	if(d.exists(path))
		return new QPixmap(path);
	return NULL;
}

void EvaImageResource::requestQQShow(const unsigned int id)
{

/*
	if(isDownloadingQQShow) return;
	QString path = getQQShowPath();
	if(path == "") return;
	EvaUser *user = EvaMain::user;
	if(!user) return;

	qqshowFile.setName(path +"/"+ QString::number(id) + ".gif" );
	if(qqshowFile.exists()){
		qqshowFile.remove();
	}

	if ( !qqshowFile.open( IO_WriteOnly ) ) {
		printf("cannot create the image\n");
		return;
	}

	EvaSetting *sysSetting = EvaMain::global->getEvaSetting();
	if(sysSetting->getConnectType(user->getQQ()) == 2){
		http->setProxyServer( QHostAddress(sysSetting->getServer( user->getQQ())).toString(),
					sysSetting->getPort( user->getQQ() ));
		http->setBase64AuthParam( sysSetting->getProxyParam( user->getQQ()));
	}
	http->setHost( "qqshow-user.tencent.com" );
	unsigned int picNum = rand()*100;
	QString remoteFile = "/"+QString::number(id) + "/10/00/" + QString::number(picNum) +".gif";
	downloadID = id;
	http->get( remoteFile, &qqshowFile  );
*/

}

void EvaImageResource::slotQQShowDone( bool error )
{
	qqshowFile.close();
	if(error){
		printf("download error: %d\n",error);
		isDownloadingQQShow = false;
		return;
	}
	isDownloadingQQShow = false;
	emit qqShowReady(downloadID);
}

void EvaImageResource::setUserHeadImage(QMap<unsigned int, QImage> imageOnList, QMap<unsigned int, QImage> imageOffList)
{
	QMap<unsigned int, QImage>::Iterator it = imageOnList.begin();
	while(it != imageOnList.end()){
		imgOnList[it.key()] = QPixmap::fromImage(it.value());
		++it;
	}
	it = imageOffList.begin();
	while(it != imageOffList.end()){
		imgOffList[it.key()] = QPixmap::fromImage(it.value());
		++it;
	}
}

QPixmap *EvaImageResource::getUserHeadPixmap(const unsigned int id, bool isGrayscale)
{
	QPixmap * result;
	QMap<int, QPixmap>::Iterator it;
	if(isGrayscale){
		it = imgOffList.find(id);
		if(it == imgOffList.end())
			result = NULL;
		else
			result = &(it.value());
	}else{
		it = imgOnList.find(id);
		if(it == imgOnList.end()){
			result = NULL;
		}else
			result = &(it.value());
	}
	return result;
}

void EvaImageResource::addUserHeadImage(const unsigned int id, QImage imgOn, QImage imgOff)
{
	imgOnList[id] = QPixmap::fromImage(imgOn);
	imgOffList[id] = QPixmap::fromImage(imgOff);
}

/*  ---------------------------------------------------------------------------------------------- */

EvaSoundResource::EvaSoundResource()
{
	soundRoot = EvaGlobal::getDataDirPath() + "/sound";
}

void EvaSoundResource::playNewMessage()
{
	playSound("msg.wav");
}

void EvaSoundResource::playSysMessage()
{
	playSound("system.wav");
}

void EvaSoundResource::playOnlineSound()
{
	playSound("online.wav");
}

void EvaSoundResource::playSound(const QString &filename)
{
	QString absPath = soundRoot + "/" + filename;
	QDir d;
	if(!d.exists(absPath)){
		absPath = EvaGlobal::getDataDirPath() + "/sound" + "/" + filename;
	}

	if(!d.exists(absPath))	return;

	//KAudioPlayer snd(absPath);
	//snd.play();
}

/*  ---------------------------------------------------------------------------------------------- */

//TODO:
//QString EvaGlobal::dataDirPath = "~/eva";
QString EvaGlobal::dataDirPath = "data";
//QString EvaGlobal::dataDirPath = QApplication::applicationDirPath()+"/data";


EvaGlobal::EvaGlobal()
{
	//dataDirPath = QApplication::applicationDirPath()+"/data";
	//qDebug()<<"EvaGlobal::initialize()~~dataDirPath:"<<dataDirPath;
	initialize();
}

EvaGlobal::~EvaGlobal()
{
	delete imgResource;
	delete sndResource;
	//delete system;
	//delete servers;
}

QString EvaGlobal::getDataDirPath()
{

	//如果使用相对路径，在Linux下可能会找不到目录
	//dataDirPath = QApplication::applicationDirPath()+"/data";

	return Settings::instance()->getDataRootPath();

}

 bool EvaGlobal::loadImage() const
{
	if(!imgResource) return false;
	return imgResource->loadImage();
}

// const bool EvaGlobal::loadFace()
// {
//         imgResource->loadFace();
// }

/*
const bool EvaGlobal::loadSound()
{
	if(!sndResource) return false;
	return sndResource->loadSound();
}*/

 bool EvaGlobal::loadEvaSetting() const
{
	//if(!system) return false;
	//return system->loadSetting();
	return true;
}

void EvaGlobal::initialize()
{
	/*
	dataDirPath = KGlobal::dirs()->findResource("data", QString::fromLatin1("eva/servers"));

	if(dataDirPath == QString::null){
		QFileInfo fi;
		fi.setFile(QString(getenv("_")));
		dataDirPath = fi.dataDirPath(true);
	}else
		dataDirPath = dataDirPath.left(dataDirPath.length() - strlen("/servers"));
*/
	//printf("found data path: %s\n", dataDirPath.ascii());


	initImage();
	initSound();
	initEvaSetting();
	initServers();
}

void EvaGlobal::initImage()
{
	imgResource = new EvaImageResource();
}

void EvaGlobal::initSound()
{
	sndResource = new EvaSoundResource();
}

void EvaGlobal::initEvaSetting()
{
	//system = new EvaSetting();
}

void EvaGlobal::initServers( )
{
	//servers = new EvaServers(dataDirPath);
}

const QSize &EvaGlobal::getFaceSize() const
{
    return faceSize;
}

void EvaGlobal::setFaceSize( const QSize size)
{
    faceSize = size;
    if(imgResource)
        imgResource->loadFace(faceSize);
}





