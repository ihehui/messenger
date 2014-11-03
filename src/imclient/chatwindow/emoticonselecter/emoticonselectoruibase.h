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

#ifndef EMOTICONSELECTORUIBASE_H
#define EMOTICONSELECTORUIBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QToolButton;
class QCheckBox;
class QLabel;

class EmoticonSelectorUIBase : public QWidget
{
    Q_OBJECT

public:
    EmoticonSelectorUIBase( QWidget* parent = 0, const QString &objectName = "", Qt::WindowFlags fl = 0 );
    ~EmoticonSelectorUIBase();

    QTabWidget* tabWidget;
    QToolButton* addFaceToolButton;
    QCheckBox* useShortcutCheckBox;
    QLabel* pageLabel;
    QToolButton* previousPageToolButton;
    QToolButton* nextPageToolButton;

protected:
    QGridLayout* emoticonSelectorUIBaseGridLayout;
    QVBoxLayout* mainVBoxLayout;
    QHBoxLayout* bottomHBoxLayout;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // EMOTICONSELECTORUIBASE_H
