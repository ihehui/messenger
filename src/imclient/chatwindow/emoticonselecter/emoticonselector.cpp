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

#include <QApplication>
#include <QPaintEvent>
#include <QDebug>
#include <QDir>

#include <qtabwidget.h>
#include <qcheckbox.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qimage.h>
#include <qbitmap.h>
#include <qpixmap.h>
#include <QPixmapCache>


//#include "../evamain.h"
//#include "evauser.h"
//#include "evausersetting.h"
//#include "evaqtutil.h"
//#include "evaresource.h"


#include "emoticonselector.h"



#define FACE_GRID_SIZE    24  //20 QQ_FACE_THUMBNAIL_SIZE defined in customface.h
#define GRID_COLOR    (QColor(0xDD, 0xE9, 0xF9))

// we need draw some grids
#define GRID_LINE_WIDTH   5
#define FACE_PANEL_WIDTH        ((FACE_GRID_SIZE + GRID_LINE_WIDTH) * NUM_COLUMN + GRID_LINE_WIDTH)
#define FACE_PANEL_HEIGHT       ((FACE_GRID_SIZE + GRID_LINE_WIDTH) * NUM_ROW + GRID_LINE_WIDTH)

// note: index, row, col  ---> (0,1,2,3,...,N)
#define RowOf(index)   ( (int)( (index) / NUM_COLUMN) )
#define ColOf(index)    ( (index) % NUM_COLUMN )
#define IndexOf(r, c, page)    ( (page) * NUM_GRIDS + (r) * NUM_COLUMN + c)

// parameters: pixmap with mask, original image, the color to be ignored
static void CreateMaskFromImage(QPixmap *pixmap, QImage *image, int color)
{
	QImage *maskImage;
	QBitmap maskBitmap;
	int x, dx, y, pixel;
	unsigned char *sourceLine, *destLine;

	// we only deal with depth of 32 images( QQ thumbnail format)
	if(image->depth() != 32) return;
	maskImage = new QImage(image->width(), image->height(),QImage::Format_RGB32);

	//maskImage->setColor(0, Qt::white.rgb());
	//maskImage->setColor(1, Qt::black.rgb());
	maskImage->setColor(1, Qt::white);

	for(y=0; y < image->height(); y++) {
		sourceLine = image->scanLine(y);
		destLine = maskImage->scanLine(y);
		dx = 0;
		for(x=0; x <  4* image->width(); x+=4, dx++) {
			pixel = *((int *)(sourceLine + x));
			if(pixel == color) pixel = 0;
			else pixel = 1;
			*(destLine + dx ) = pixel;
		}
	}
	maskBitmap = QBitmap::fromImage(*maskImage);
	delete maskImage;
	pixmap->setMask(maskBitmap);
}


EmoticonPanel::EmoticonPanel(int groupIndex, const QString &groupName, QWidget* parent)
    : QFrame(parent, Qt::FramelessWindowHint),
      m_groupName(groupName), m_groupIndex(groupIndex), m_CurrSurface(QPixmap()), m_CurrPage(0)
{

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

     QPalette palette;
     palette.setColor(backgroundRole(), QColor( 255, 255, 255));
     setPalette(palette);
     
     setMinimumSize(QSize(FACE_PANEL_WIDTH, FACE_PANEL_HEIGHT));
     adjustSize();

    //resize( QSize(FACE_PANEL_WIDTH, FACE_PANEL_HEIGHT) );

     
    m_baseDirPath = "";
    m_systememoticon = false;
    
}

EmoticonPanel::~EmoticonPanel()
{
	m_Surfaces.clear();
	files.clear();
}


void EmoticonPanel::setPage(int index)
{
	if(index<0 || index >= (int)(m_Surfaces.count()) ) return;
	m_CurrPage = index;
	m_CurrSurface = m_Surfaces.at(index);
	repaint();
}

void EmoticonPanel::paintEvent( QPaintEvent *e)
{
        if(m_CurrSurface.isNull()){
		QFrame::paintEvent(e);
		return;
	}

	int x = e->rect().x();
	int y = e->rect().y();
	int w = e->rect().width();
	int h = e->rect().height();

	QPainter painter(this);
        painter.drawPixmap(x, y, m_CurrSurface, x, y, w, h);

	drawFrame(&painter);
}

void EmoticonPanel::mouseReleaseEvent( QMouseEvent * e)
{
	if(e->button() != Qt::LeftButton){
		e->ignore();
		return;
	}

	int row = e->y() / (FACE_GRID_SIZE + GRID_LINE_WIDTH);
	int col = e->x() / (FACE_GRID_SIZE + GRID_LINE_WIDTH);
	if(row < NUM_ROW && col < NUM_COLUMN)
	{
            int index = IndexOf(row, col, m_CurrPage);
            if(index < 0 || index >= (int)(files.size()) ) return;
            
            emit signalEmoticonSelected(m_baseDirPath +"/"+ files[index], m_systememoticon);
                       

	}
}

void EmoticonPanel::loadSystemEmoticons(const QString &resourcePath, const QStringList &imageFiles){
    
    m_Surfaces.clear();
    files.clear();

    m_baseDirPath = resourcePath;
    m_systememoticon = true;
    
    int count = imageFiles.size();
    for(int i=0; i<count; i++){
        files[i] = imageFiles.at(i);
    }
  
    int iPages = count / NUM_GRIDS + ((count % NUM_GRIDS)?1:0);
    for(int p = 0; p < iPages; p++)
    {
        addEmoticonsPanel( p * NUM_GRIDS);
    }
    setPage(0);
    
    
  
}

void EmoticonPanel::loadCustomEmoticons(const QString &baseDirPath){
    
    m_Surfaces.clear();
    files.clear();

    m_baseDirPath = baseDirPath;
    m_systememoticon = false;
    
    QDir dir(baseDirPath);
    
    
    QStringList filters;
    filters << "*.png" << "*.gif" << "*.jpg"; 
    QStringList imageFiles = dir.entryList(filters, QDir::Files|QDir::System|QDir::Hidden);
    int count = imageFiles.size();
    if(count < 1){return;}
    for(int i=0; i<count; i++){
        files[i] = imageFiles.at(i);
    }
    

    

    
    int iPages = count / NUM_GRIDS + ((count % NUM_GRIDS)?1:0);
    for(int p = 0; p < iPages; p++)
    {
        addEmoticonsPanel( p * NUM_GRIDS);
    }
    setPage(0);
    
    
  
}

void EmoticonPanel::addEmoticonsPanel(int iStartIndex){
    
    // create a back-buffer panel
    //QPixmap *pic = new QPixmap(FACE_PANEL_WIDTH, FACE_PANEL_HEIGHT);
    QPixmap pic = QPixmap(FACE_PANEL_WIDTH, FACE_PANEL_HEIGHT);
    pic.fill();

    // draw grids for last panel buffer
    QPainter painter(&pic);
    painter.setPen(QPen(GRID_COLOR, 1));

    // draw rows
    for(int r=1; r< NUM_ROW; r++){
            int y = r * (FACE_GRID_SIZE + GRID_LINE_WIDTH) - (GRID_LINE_WIDTH>>1) + GRID_LINE_WIDTH;
            painter.drawLine(GRID_LINE_WIDTH, y, FACE_PANEL_WIDTH - GRID_LINE_WIDTH, y);
    }

    // draw cols
    for(int c=1; c<NUM_COLUMN; c++){
            int x = c * (FACE_GRID_SIZE + GRID_LINE_WIDTH) - (GRID_LINE_WIDTH>>1) + GRID_LINE_WIDTH;
            painter.drawLine(x, GRID_LINE_WIDTH, x, FACE_PANEL_HEIGHT - GRID_LINE_WIDTH);
    }

    int i = iStartIndex , iRow, iCol;
    //int page = m_Surfaces.count();
    for(; i < (iStartIndex + NUM_GRIDS) && i < files.size(); i++)
    {
            iRow = (i % NUM_GRIDS) / NUM_COLUMN;
            iCol = (i % NUM_GRIDS) % NUM_COLUMN;
            //m_sysIndex[page * NUM_GRIDS + iRow * NUM_COLUMN + iCol] = i;

            //TODO:
            QString iconPath = m_baseDirPath + "/" + files[i];
            QPixmap smiley;
            if(!QPixmapCache::find(iconPath, &smiley)){
                smiley = QPixmap(iconPath);// load smileys
                smiley = smiley.scaled(FACE_GRID_SIZE, FACE_GRID_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QPixmapCache::insert(iconPath, smiley);
            }

            int y = iRow * (FACE_GRID_SIZE + GRID_LINE_WIDTH) + GRID_LINE_WIDTH;
            int x = iCol * (FACE_GRID_SIZE + GRID_LINE_WIDTH) + GRID_LINE_WIDTH;
            painter.drawPixmap(x, y, smiley);
    }

    m_Surfaces.append(pic);

}




///*  =========================================== */

EmoticonSelector::EmoticonSelector( QWidget* parent,  const QString &objectName, Qt::WindowFlags fl )
    : EmoticonSelectorUIBase( parent, objectName, fl )
{
    

    loadSystemEmoticons();
    loadCustomEmoticons();
    
    tabWidget->setCurrentIndex(0);
    slotPanelChanged(0);
    
    connect(addFaceToolButton, SIGNAL(clicked()), this, SLOT(slotAddSmileyClicked()) );
    connect(useShortcutCheckBox, SIGNAL(clicked()), this, SLOT(slotUseShortcutClicked()) );
    connect(previousPageToolButton, SIGNAL(clicked()), this, SLOT(slotPrevClicked() ) );
    connect(nextPageToolButton, SIGNAL(clicked()), this, SLOT(slotNextClicked() ) );
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotPanelChanged(int)) );

}

EmoticonSelector::~EmoticonSelector()
{
    // no need to delete child widgets, Qt does it all for us
}

void EmoticonSelector::loadSystemEmoticons(){

    QString systemEmoticonsPath = ":/emoticons";
    QStringList imageFiles;
    for (int i = 0; i < SYSTEM_EMOTICONS_COUNT; i++) {
        imageFiles.append(QString::number(i)+".gif");
    }  
    
    EmoticonPanel *systemEmoticonsPanel = new EmoticonPanel(0, QString("System"), tabWidget);
    systemEmoticonsPanel->loadSystemEmoticons(systemEmoticonsPath, imageFiles);
    
    tabWidget->addTab(systemEmoticonsPanel, tr("System"));
    connect(systemEmoticonsPanel, SIGNAL(signalEmoticonSelected(const QString &, bool)),
            this, SIGNAL(signalEmoticonSelected(const QString &, bool)) );

    
}

void EmoticonSelector::loadCustomEmoticons(){
    


    QString baseDirPath = Settings::instance()->getCustomEmoticonsDir();
    QDir dir(baseDirPath);
    
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
//        QString themeSettingsFile = subDir+"/theme";
//        if(!dir.exists(themeSettingsFile)){
//            continue;
//        }
        
        EmoticonPanel *customEmoticonsPanel = new EmoticonPanel(0, subDir, tabWidget);
        customEmoticonsPanel->loadCustomEmoticons(baseDirPath + "/" +subDir);
        
        tabWidget->addTab(customEmoticonsPanel, subDir);
        //tabWidget->setCurrentWidget(customEmoticonsPanel);
        
//        int total = customEmoticonsPanel->numPages();
//        int curr = customEmoticonsPanel->page();
//        pageLabel->setText(QString("%1/%2").arg(curr+1).arg(total));
        
        connect(customEmoticonsPanel, SIGNAL(signalEmoticonSelected(const QString &, bool)),
                this, SIGNAL(signalEmoticonSelected(const QString &, bool)) );

        
        
    }

//    tabWidget->setCurrentIndex(0);
//    slotPanelChanged(0);

    
}

void EmoticonSelector::slotAddSmileyClicked()
{
	emit addSmileyClicked();
	close();
}

void EmoticonSelector::slotUseShortcutClicked()
{
	printf("Shortcut clicked\n");
}

void EmoticonSelector::slotPrevClicked()
{
	EmoticonPanel *panel = static_cast<EmoticonPanel*>(tabWidget->currentWidget());
	if(panel){
		int total = panel->numPages();
		int curr = panel->page();
		//if(curr < 1) return;
		panel->setPage( curr - 1);
		pageLabel->setText(QString("%1/%2").arg(curr).arg(total));

                    if(curr == 1){
                        previousPageToolButton->setEnabled(false);
                        nextPageToolButton->setEnabled(true);
                    }                   
               
	}
}

void EmoticonSelector::slotNextClicked()
{
	EmoticonPanel *panel = static_cast<EmoticonPanel*>(tabWidget->currentWidget());
        
	if(panel){
		int total = panel->numPages();
		int curr = panel->page();
		//if(curr > total -1 ) return; 
		panel->setPage( curr + 1);
		pageLabel->setText(QString("%1/%2").arg(curr+2).arg(total));

                    if(curr == total - 2){
                        previousPageToolButton->setEnabled(true);
                        nextPageToolButton->setEnabled(false);
                    }
                                    
	}
                    
        
}

void EmoticonSelector::slotPanelChanged(int index)
{
    qDebug()<<"---------slotPanelChanged(...)";
    
	//if(m_UseSysFaceOnly) return;
	EmoticonPanel *panel = static_cast<EmoticonPanel*>(tabWidget->widget(index));
	if(panel){
		int total = panel->numPages();
		int curr = panel->page();
                int displayNo = curr + 1;
		pageLabel->setText(QString("%1/%2").arg(displayNo).arg(total));
                if(total == 1){
                    previousPageToolButton->setEnabled(false);
                    nextPageToolButton->setEnabled(false);
                }else{
                    if(displayNo == 1){
                        previousPageToolButton->setEnabled(false);
                        nextPageToolButton->setEnabled(true);
                    }
                    if(displayNo == total){
                        previousPageToolButton->setEnabled(true);
                        nextPageToolButton->setEnabled(false);
                    }
                    
                }
                
	}
}



void EmoticonSelector::paintEvent( QPaintEvent *e)
{
	QPainter painter(this);
	painter.setPen(QPen(QColor(4, 74, 155), 1));
	painter.drawRect(rect());

	QWidget::paintEvent(e);
}

void EmoticonSelector::mouseReleaseEvent( QMouseEvent * e)
{
	if(e->button() != Qt::LeftButton){
		hide();
		e->accept();
	}
	e->ignore();
}








