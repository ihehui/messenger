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

#ifndef EMOTICONSELECTOR_H
#define EMOTICONSELECTOR_H

#include "emoticonselectoruibase.h"
#include "customface.h"
#include "evaresource.h"
#include "../../settings.h"

#include <qframe.h>
#include <qmap.h>
//#include <qptrlist.h>
//#include "evautil.h"

#ifndef NUM_W
#define NUM_COLUMN         15
#define NUM_ROW           8
#define NUM_GRIDS     (NUM_COLUMN * NUM_ROW)
//#define QQ_SMILEY_AMOUNT 135
#endif

#ifndef SYSTEM_EMOTICONS_COUNT
#define SYSTEM_EMOTICONS_COUNT 107
#endif


using namespace HEHUI;

class EmoticonPanel : public QFrame
{
    Q_OBJECT

public:
    EmoticonPanel(int groupIndex, const QString &groupName, QWidget* parent = 0);

    virtual ~EmoticonPanel();
    int numPages() { return m_Surfaces.count();}
    int page() { return m_CurrPage; }
    void setPage(int index);


    void loadSystemEmoticons(const QString &resourcePath, const QStringList &imageFiles);
    void loadCustomEmoticons(const QString &baseDirPath);


private:

    void addEmoticonsPanel(int iStartIndex = 0);
    
protected:
    virtual void paintEvent( QPaintEvent *e);
    virtual void mouseReleaseEvent( QMouseEvent * e);
    
signals:
    void signalEmoticonSelected(const QString &iconPath, bool isSystemEmoticon);
    

private:
    QString m_groupName;
    int m_groupIndex;
    QPixmap m_CurrSurface;
    int m_CurrPage;
    QList<QPixmap> m_Surfaces;
    QMap<int, QString> files; // index, original file name
    //int m_sysIndex[QQ_SMILEY_AMOUNT];

    QString m_baseDirPath;
    bool m_systememoticon;




};

class EmoticonSelector : public EmoticonSelectorUIBase
{
    Q_OBJECT

public:
    EmoticonSelector( QWidget* parent = 0,
                      const QString &objectName = "",
                      Qt::WindowFlags fl = Qt::Popup | Qt::FramelessWindowHint);
    
    
    ~EmoticonSelector();
    
private:
    void loadSystemEmoticons();
    void loadCustomEmoticons();
    

signals:
    void signalEmoticonSelected(const QString &iconPath, bool isSystemEmoticon);

    void addSmileyClicked();

private slots:

    void slotAddSmileyClicked();
    void slotUseShortcutClicked();
    void slotPrevClicked();
    void slotNextClicked();
    void slotPanelChanged(int index);


protected:
    virtual void paintEvent( QPaintEvent *e);
    virtual void mouseReleaseEvent( QMouseEvent * e);

private:



};

#endif // EMOTICONSELECTOR_H
