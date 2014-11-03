 /***************************************************************************
 *   Copyright (C) 2006 by yunfan                                          *
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

#include "customface.h"

#include <QDebug>

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>




#define XML_FACE_FILE                   "face.xml"

#define XML_NAME                          "EVA_CUSTOM_FACE_CONFIG"
#define XML_ROOT                           "FACESETTING"
#define TAG_CUSTOMFACE               "CUSTOMFACE"
#define TAG_CUSTOMFACEGROUP    "CUSTOMFACEGROUP"
#define TAG_FACE                            "FACE"
#define TAG_FILE_ORG                      "FILE_ORG"
#define __FILE_ORG                           "FILE ORG"
#define TAG_FILE_FIXED                   "FILE_FIXED"
#define __FILE_FIXED                        "FILE FIXED"
#define TAG_DEFAULTFACE              "DEFAULTFACE"
#define TAG_SORT                            "SORT"

#define ATTR_version          "version"
#define ATTR_name            "name"
#define ATTR_count            "count"
#define ATTR_showall         "showall"
#define ATTR_line               "line"
#define ATTR_id                  "id"
#define ATTR_shortcut         "shortcut"
#define ATTR_tip                 "tip"
#define ATTR_multiframe       "multiframe"

#define FACE_GROUP_VERSION     "1003"


CustomFace::CustomFace()
	: OrgName(""),
	Shortcut(""),
	Tip(""),
	FramesCount(0),
	GroupId(0)
{
}

CustomFace::CustomFace(const CustomFace&rhs)
{
	(*this) = rhs;
}

CustomFace::CustomFace(const QString &name, const int groupId)
	: OrgName(name), FramesCount(0), GroupId(groupId)
{
	Shortcut = OrgName.left(6);
	Tip = OrgName.left(OrgName.lastIndexOf('.'));
}

CustomFace::CustomFace(const QString &name,
				const QString &shortcut,
				const QString &tip,
				const int frames,
				const int groupId)
	: OrgName(name),
	Shortcut(shortcut),
	Tip(tip),
	FramesCount(frames),
	GroupId(groupId)
{
}

CustomFace & CustomFace::operator =( const CustomFace & rhs )
{
	OrgName = rhs.org(),
	Shortcut = rhs.shortcut(),
	Tip = rhs.tip(),
	FramesCount = rhs.numFrames(),
	GroupId = rhs.groupID();
	return *this;
}

const QString CustomFace::id()  const
{
	return OrgName.left(OrgName.lastIndexOf('.'));
}

const QString CustomFace::fixed()  const
{
	return id() + "fixed.bmp";
}


/* ================================================================= */

CustomFaceConfig::CustomFaceConfig(const QString &dir)
	: m_Dir(dir), m_Doc(0)
{
}

CustomFaceConfig::~CustomFaceConfig()
{
	if(m_Doc) delete m_Doc;
}

void CustomFaceConfig::createConfig()
{
	if(m_Doc ) delete m_Doc;
	m_Doc = new QDomDocument(XML_NAME);
	// add a root node
	QDomElement root = m_Doc->createElement(XML_ROOT);
	m_Doc->appendChild(root);

	///NOTE: do we follow Tencent way? Yes, we do.
	QDomElement g = m_Doc->createElement(TAG_CUSTOMFACE);
	g.setAttribute(ATTR_count, 0);
	g.setAttribute(ATTR_version, FACE_GROUP_VERSION);
	g.setAttribute(ATTR_name, QObject::tr("Default"));
	m_Doc->documentElement().appendChild(g);
}

bool CustomFaceConfig::addFace(const CustomFace &face)
{
	// if not created, just do it
	if(!m_Doc) createConfig();

	int gId = face.groupID();
	if(gId <0 || gId >= numGroups())
		return false; // out of range

	// create a new node & set attributes
	QDomElement f = m_Doc->createElement(TAG_FACE);
	f.setAttribute(ATTR_id, face.id());
	f.setAttribute(ATTR_shortcut, face.shortcut());
	f.setAttribute(ATTR_tip, face.tip());
	f.setAttribute(ATTR_multiframe, face.numFrames());

	// sub item: org
	QDomElement org = m_Doc->createElement(TAG_FILE_ORG);
	f.appendChild(org);  // append it to face node
	// data of org node
	QDomText orgData = m_Doc->createTextNode(face.org());
	org.appendChild(orgData); // append it to the node

	// sub item: fixed
	QDomElement fixed = m_Doc->createElement(TAG_FILE_FIXED);
	f.appendChild(fixed); // add it the end of face node
	// data of fixed node
	QDomText fixedData = m_Doc->createTextNode(face.fixed());
	fixed.appendChild(fixedData); // add data to fixed node

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();
	// add the face to groupID
	group.appendChild(f);

	//don't forget update groupID count attribute
	group.setAttribute(ATTR_count, group.childNodes().count());
	return true;
}

CustomFace CustomFaceConfig::getFace(const int gId, const int fNo)
{
	CustomFace face;
	if(!m_Doc) return face;

	if(gId <0 || gId >= numGroups())
		return face; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return face;
	QDomElement group = node.toElement();

	int index = 0;
	QDomNode n = group.firstChild();
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					face.setGroupID( gId);
					face.setShortcut( e.attribute(ATTR_shortcut, ""));
					face.setTip( e.attribute( ATTR_tip, ""));
					face.setNumFrames( e.attribute( ATTR_multiframe, 0).toInt() );

					QDomNode o = e.firstChild();
					if(!o.isNull()){
						QDomElement org = o.toElement();
						if(!org.isNull()){
							face.setOrgName( org.text() );
						}
					}
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}

	// otherwise, remove failed
	return face;
}

bool CustomFaceConfig::moveFaceUp( const int gId, const int fNo )
{
	if(!m_Doc) return false;
	if(fNo<=0) return false; // the first one
	if(gId <0 || gId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();

	QDomNode result;
	QDomNode n = group.firstChild();
	int index = 0;
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					QDomNode bef = e.previousSibling();
					result = group.insertBefore( e, bef);
					break;
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}

	return (!result.isNull());
}

bool CustomFaceConfig::moveFaceDown( const int gId, const int fNo )
{
	if(!m_Doc) return false;
	if(fNo<=0) return false; // the first one
	if(gId <0 || gId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();

	QDomNode result;
	QDomNode n = group.firstChild();
	int index = 0;
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					QDomNode aft = e.nextSibling();
					result = group.insertAfter( e, aft);
					break;
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}

	return (!result.isNull());
}

bool CustomFaceConfig::moveFaceTo( const int gId, const int fNo, const int gDestId )
{
	if(!m_Doc) return false;
	if(gId <0 || gId >= numGroups())
		return false; // out of range
	if(gDestId <0 || gDestId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode src = groups.item(gId);
	if(src.isNull()) return false;

	QDomNode dest = groups.item(gDestId);
	if(dest.isNull()) return false;

	QDomNode n = src.firstChild();
	QDomNode sibling;
	int index = 0;
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					n = src.removeChild( n );
					dest.appendChild(n);
					return true;
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}
	return true;
}

bool CustomFaceConfig::removeFace(const CustomFace &face)
{
	if(!m_Doc) return false;

	int gId = face.groupID();
	if(gId <0 || gId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();

	QDomNode result;
	QDomNode n = group.firstChild();
	while(!n.isNull()){
		QDomElement e = n.toElement();
		if(!e.isNull()){
			if( e.tagName() == TAG_FACE){
				if(face.id() == e.attribute(ATTR_id, "")){
					result = group.removeChild(e);
					break;
				}
			}
		}
		n = n.nextSibling();
	}

	if(!result.isNull()){
		// remove success, so update count attribute
		group.setAttribute(ATTR_count, group.childNodes().count());
		return true;
	}
	// otherwise, remove failed
	return false;
}

bool CustomFaceConfig::removeFace(const int gId, const int fNo)
{
	if(!m_Doc) return false;

	if(gId <0 || gId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();

	int index = 0;
	QDomNode result;
	QDomNode n = group.firstChild();
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					result = group.removeChild(e);
					break;
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}

	if(!result.isNull()){
		// remove success, so update count attribute
		group.setAttribute(ATTR_count, group.childNodes().count());
		return true;
	}
	// otherwise, remove failed
	return false;
}

bool CustomFaceConfig::updateFaceTip( const int gId,
							const  int fNo,
							const QString & tip)
{
	if(!m_Doc) return false;

	if(gId <0 || gId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();

	QDomNode n = group.firstChild();
	int index = 0;
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					e.setAttribute(ATTR_tip, tip);
					return true;
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}

	return false;
}

bool CustomFaceConfig::updateFaceShortcut( const int gId,
							const  int fNo,
							const QString & shortcut )
{
	if(!m_Doc) return false;

	if(gId <0 || gId >= numGroups())
		return false; // out of range

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(gId);
	if(node.isNull()) return false;
	QDomElement group = node.toElement();

	QDomNode n = group.firstChild();
	int index = 0;
	while(!n.isNull()){
		if(index == fNo){
			QDomElement e = n.toElement();
			if(!e.isNull()){
				if( e.tagName() == TAG_FACE){
					e.setAttribute(ATTR_shortcut, shortcut);
					return true;
				}
			}
			break;
		}
		index++;
		n = n.nextSibling();
	}

	return false;
}

bool CustomFaceConfig::addGroup(const QString &name)
{
	if(name.isEmpty()) return false;
	if(!m_Doc) createConfig();

	QDomElement g = m_Doc->createElement(TAG_CUSTOMFACEGROUP);
	g.setAttribute(ATTR_count, 0);
	g.setAttribute(ATTR_version, FACE_GROUP_VERSION);
	g.setAttribute(ATTR_name, name);
	m_Doc->documentElement().appendChild(g);

	return true;
}

bool CustomFaceConfig::removeGroup(const int groupIndex)
{
	if(!m_Doc) return false;
	// default groupID never be removed
	if(groupIndex == 0) return false;

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	printf("groupIndex: %d, nums: %d\n", groupIndex, groups.count());
	QDomNode group = groups.item(groupIndex);
	if(group.isNull()) return false;
	QDomNode result = root.removeChild(group);
	return (result.isNull()) ? false : true;
}

bool CustomFaceConfig::removeGroup( const QString & name )
{
	if(!m_Doc) return false;

	QDomNode root = m_Doc->documentElement();
	QDomNode n = root.firstChild();
	QDomNode sibling;
	qDebug()<<"----CustomFaceConfig::removeGroup(....)~~"<<QObject::tr("CustomFaceConfig::removeGroup: %1\n").arg(name);
	//printf("CustomFaceConfig::removeGroup: %s\n", name.local8Bit().data());
	while(!n.isNull()){
		QDomElement e = n.toElement();
		sibling = e.nextSibling();
		if(!e.isNull()){
			if( e.tagName() == TAG_CUSTOMFACEGROUP){
				qDebug()<<"----CustomFaceConfig::removeGroup(....)~~"<<QObject::tr("group name: %1\n").arg(e.attribute(ATTR_name, ""));
				//printf("group name: %s\n", e.attribute(ATTR_name, "").local8Bit().data());
				if(e.attribute(ATTR_name, "") == name){
					QDomNode old = root.removeChild( e);
					printf("finished remove\n");
					if(old.isNull()) return false;
					else return true;
				}
			}
		}
		n = sibling;
	}
	return false;
}

bool CustomFaceConfig::renameGroup( const QString & oldName, const QString & newName )
{
	if(!m_Doc) return false;

	bool renamed = false;

	QDomNode root = m_Doc->documentElement();
	QDomNode n = root.firstChild();
	while(!n.isNull()){
		QDomElement e = n.toElement();
		if(!e.isNull()){
			if( e.tagName() == TAG_CUSTOMFACEGROUP){
				if(e.attribute(ATTR_name, "") == oldName){
					e.setAttribute( ATTR_name, newName);
					renamed = true;
				}
			}
		}
		n = n.nextSibling();
	}
	return renamed;
}

bool CustomFaceConfig::moveChildrenTo(const int srcIndex, const int destIndex)
{
	if(!m_Doc) return false;

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode src = groups.item(srcIndex);
	if(src.isNull()) return false;

	QDomNode dest = groups.item(destIndex);
	if(dest.isNull()) return false;

	QDomNode n = src.firstChild();
	QDomNode sibling;
	while(!n.isNull()){
		sibling = n.nextSibling();
		n = src.removeChild( n );
		dest.appendChild(n);
		n = sibling;
	}
	return true;
}

int CustomFaceConfig::groupIndex( const QString & name )
{
	if(!m_Doc) return -1;
	int count = -1, index = -1;
	QDomNode root = m_Doc->documentElement();
	QDomNode n = root.firstChild();
	while(!n.isNull()){
		QDomElement e = n.toElement();
		if(!e.isNull()){
			if(e.tagName() == TAG_CUSTOMFACE){
				count++;
				// note that QQ config file has not tag name for the default groupID
				if( name == e.attribute(ATTR_name, QObject::tr("Default")) ){
					index = count;
					break;
				}
			}
			if( e.tagName() == TAG_CUSTOMFACEGROUP){
				count++;
				if( name == e.attribute(ATTR_name, "") ){
					index = count;
					break;
				}
			}
		}
		n = n.nextSibling();
	}
	return index;
}

 int CustomFaceConfig::numGroups() const
{
	if(!m_Doc) return 0; // 0 means empty
	return m_Doc->documentElement().childNodes().count();
}

QString CustomFaceConfig::groupName(const int groupIndex)
{
	if(!m_Doc) return "";

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(groupIndex);
	if(node.isNull()) return "";
	QDomElement group = node.toElement();
	return group.attribute(ATTR_name, QObject::tr("Default"));
}

QStringList CustomFaceConfig::groupNames()
{
	QStringList list;
	if(!m_Doc) return list;

	QDomNode root = m_Doc->documentElement();
	QDomNode n = root.firstChild();
	while(!n.isNull()){
		QDomElement e = n.toElement();
		if(!e.isNull()){
			if( e.tagName() == TAG_CUSTOMFACEGROUP ||
				e.tagName() == TAG_CUSTOMFACE){
				list += e.attribute(ATTR_name, QObject::tr("Default"));
			}
		}
		n = n.nextSibling();
	}

	return list;
}

FaceList CustomFaceConfig::groupMembers(const int groupIndex)
{
	FaceList list;
	if(!m_Doc) return list;

	QDomNode root = m_Doc->documentElement();
	QDomNodeList groups = root.childNodes();
	QDomNode node = groups.item(groupIndex);
	if(node.isNull()) return list;
	QDomElement group = node.toElement();

	QDomNode n = group.firstChild();
	while(!n.isNull()){
		QDomElement e = n.toElement();
		if(!e.isNull()){
			if( e.tagName() == TAG_FACE){
				QString id = e.attribute(ATTR_id, "");
				QString shortcut = e.attribute(ATTR_shortcut, "");
				QString tip = e.attribute(ATTR_tip, "");
				QString frames = e.attribute(ATTR_multiframe, 0);
				QString org, fixed;
				QDomNode fsubN = e.firstChild();
				if(fsubN.isNull()){
					org = id+".jpg";
					// fixed could be generated automatically in CustomFace
					//fixed = id+"fixed.bmp";
				} else {
					QDomElement fsub = fsubN.toElement();
					if(fsub.tagName() == TAG_FILE_ORG){
						org = fsub.text();
						// we got org, we might stop looking for fixed?
						QDomNode fixN = fsub.nextSibling();
						if(fixN.isNull()) fixed = org.left(org.lastIndexOf('.')) + "fixed.bmp";
						else{
							QDomElement ssub = fixN.toElement();
							if(ssub.isNull())
								fixed = org.left(org.lastIndexOf('.')) + "fixed.bmp";
							else
								fixed = ssub.text();
						}
					}else if(fsub.tagName() == TAG_FILE_FIXED){
						fixed = fsub.text();
						QDomNode orgN = fsub.nextSibling();
						if(orgN.isNull())  // in this case, just a guess
							org = fixed.left(org.lastIndexOf('.')) + ".jpg";
						else{
							QDomElement ssub = orgN.toElement();
							if(ssub.isNull()) // give a guessed value
								org = fixed.left(org.lastIndexOf('.')) + ".jpg";
							else
								org = ssub.text();
						}
					}else {
						org = id + ".jpg";
						// don't care about fixed
					}
				}
				// acutually, "fixed" is useless. only org used here
				bool ok;
				int num = frames.toInt(&ok);
				CustomFace face(org, shortcut, tip, num, groupIndex);
				list += face;
			}
		}
		n = n.nextSibling();
	}
	return list;
}

bool CustomFaceConfig::loadXML()
{
	QFile xmlFile(m_Dir+"/"XML_FACE_FILE);
	if(! xmlFile.open( QIODevice::ReadOnly ) ) {
		// if config file not existed we create an default config file
		createConfig();
		saveXML();
		return false;
	}

	QTextStream xmlStream(&xmlFile);
	xmlStream.setCodec(QTextCodec::codecForName("GB18030") );
	QString xmlstr = xmlStream.readAll();
	xmlFile.close();

	xmlstr.replace(__FILE_ORG, TAG_FILE_ORG);
	xmlstr.replace(__FILE_FIXED, TAG_FILE_FIXED);

	if(!m_Doc) {
		m_Doc = new QDomDocument();
	}

	bool result = false;
	QString errmsg;
	int line, col;
	result = m_Doc->setContent(xmlstr, &errmsg, &line, &col);
	if(!result){
		qDebug()<<"----CustomFaceConfig::removeGroup(....)~~"<<QObject::tr("XML custom face config parse error @(%1, %2): %3\n").arg(line).arg(col).arg(errmsg);
	}
	return result;
}

bool CustomFaceConfig::saveXML()
{
	if(!m_Doc) return false;

	QString xmlstr = m_Doc->toString();
	xmlstr.replace(TAG_FILE_ORG, __FILE_ORG);
	xmlstr.replace(TAG_FILE_FIXED, __FILE_FIXED);

	QFile xmlFile(m_Dir+"/"XML_FACE_FILE);
	if(! xmlFile.open( QIODevice::ReadOnly ) ) return false;

	QTextStream xmlStream(&xmlFile);
	xmlStream.setCodec(QTextCodec::codecForName("GB18030") );
	xmlStream << xmlstr;
	xmlFile.close();

	return true;
}

bool CustomFaceConfigloadFromFileSystem(const QString &dir){

    return false;
}

QString CustomFaceConfig::toString()
{
	if(!m_Doc) return "";
	return m_Doc->toString();
}





