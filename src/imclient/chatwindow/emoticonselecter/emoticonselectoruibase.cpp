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

#include "emoticonselectoruibase.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
//#include <klocale.h>


EmoticonSelectorUIBase::EmoticonSelectorUIBase( QWidget* parent, const QString &objectName, Qt::WindowFlags fl )
    : QWidget( parent, fl )
{
    if ( objectName.isEmpty() )
	setObjectName( "EmoticonSelectorUIBase" );

    QPalette palette,foregroundPalette;
    palette.setColor(backgroundRole(), QColor( 255, 255, 255 ));
    //foregroundPalette.setColor(foregroundRole(), QColor( 255, 255, 255 ));

    setPalette(palette);

    //CustomFaceUIBaseLayout = new QGridLayout( this, 1, 1, 1, 0, "CustomFaceUIBaseLayout");
    emoticonSelectorUIBaseGridLayout = new QGridLayout(this);
    //emoticonSelectorUIBaseGridLayout->setSizeConstraint(QLayout::SetFixedSize);

    //mainVBoxLayout = new QVBoxLayout( 0, 0, 0, "mainVBoxLayout");
    mainVBoxLayout = new QVBoxLayout();

    //tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget = new QTabWidget( this);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
    tabWidget->setSizePolicy(sizePolicy);
    //tabWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //tabWidget->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    //tabWidget->setPalette(palette);

    mainVBoxLayout->addWidget( tabWidget );

    //bottomHBoxLayout = new QHBoxLayout( 0, 5, 6, "bottomHBoxLayout");
    bottomHBoxLayout = new QHBoxLayout();

    //addFaceToolButton = new QToolButton( this, "addFaceToolButton" );
    addFaceToolButton = new QToolButton(this);

    //addFaceToolButton->setBackgroundMode( QToolButton::PaletteBackground );
    //addFaceToolButton->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
   // addFaceToolButton->setPalette(palette);
    //addFaceToolButton->setPalette(foregroundPalette);

    addFaceToolButton->setAutoRaise( true );
    bottomHBoxLayout->addWidget( addFaceToolButton );

   // useShortcutCheckBox = new QCheckBox( this, "useShortcutCheckBox" );
    useShortcutCheckBox = new QCheckBox(this);
    useShortcutCheckBox->setText( QObject::tr( "&Use Shortcut" ) );
    //useShortcutCheckBox->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    //useShortcutCheckBox->setPalette(foregroundPalette);
    bottomHBoxLayout->addWidget( useShortcutCheckBox );
    spacer1 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    bottomHBoxLayout->addItem( spacer1 );

    //pageLabel = new QLabel( this, "pageLabel" );
    pageLabel = new QLabel(this);
    //pageLabel->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    bottomHBoxLayout->addWidget( pageLabel );

    //previousPageToolButton = new QToolButton( this, "previousPageToolButton" );
    previousPageToolButton = new QToolButton(this);
    previousPageToolButton->setAutoRaise(true);
    previousPageToolButton->setEnabled(false);
    //previousPageToolButton->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    bottomHBoxLayout->addWidget( previousPageToolButton );

    //nextPageToolButton = new QToolButton( this, "nextPageToolButton" );
    nextPageToolButton = new QToolButton( this);
    nextPageToolButton->setAutoRaise(true);
    nextPageToolButton->setEnabled(false);
    //nextPageToolButton->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    bottomHBoxLayout->addWidget( nextPageToolButton );
    mainVBoxLayout->addLayout( bottomHBoxLayout );

    emoticonSelectorUIBaseGridLayout->addLayout( mainVBoxLayout,  0, 0, 1, 1 );
    languageChange();

    //TODO: Really useful?
    //setLayout(CustomFaceUIBaseLayout);
    //TODO: What is the best size?
//    resize( QSize(410, 280).expandedTo(minimumSizeHint()) );
    //clearWState( WState_Polished );
}

EmoticonSelectorUIBase::~EmoticonSelectorUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

void EmoticonSelectorUIBase::languageChange()
{
	setWindowTitle( QObject::tr( "EmoticonSelectorUIBase" ) );
	addFaceToolButton->setText( QObject::tr( "Add smiley" ) );
	addFaceToolButton->setToolTip(QObject::tr( "add Custom Smiley" ));
	useShortcutCheckBox->setText( QObject::tr( "&Use Shortcut" ) );
	useShortcutCheckBox->setShortcut( QKeySequence( QObject::tr( "Alt+U" ) ) );
	pageLabel->setText( QObject::tr( "0/0" ) );
	previousPageToolButton->setText( QObject::tr( "<<" ) );
	previousPageToolButton->setToolTip(QObject::tr( "Previous Page" ));
	nextPageToolButton->setText( QObject::tr( ">>" ) );
	nextPageToolButton->setToolTip(QObject::tr( "Next Page" ));
}

