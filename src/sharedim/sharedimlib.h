/*
 ****************************************************************************
 * sharedimlib.h
 *
 * Created on: 2009-5-1
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
 * Last Modified on: 2010-05-15
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#ifndef SHAREDIMLIB_H
#define SHAREDIMLIB_H


#include <QtCore/QtGlobal>

 #if defined(SHAREDIMLIB_LIBRARY_EXPORT)
 #  define SHAREDIMLIB_API Q_DECL_EXPORT
 #else
 #  define SHAREDIMLIB_API Q_DECL_IMPORT
 #endif


/*

#if defined(Q_WS_WIN)
#  if !defined(QT_QTSERVICE_EXPORT) && !defined(QT_QTSERVICE_IMPORT)
#    define QT_QTSERVICE_EXPORT
#  elif defined(QT_QTSERVICE_IMPORT)
#    if defined(QT_QTSERVICE_EXPORT)
#      undef QT_QTSERVICE_EXPORT
#    endif
#    define QT_QTSERVICE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTSERVICE_EXPORT)
#    undef QT_QTSERVICE_EXPORT
#    define QT_QTSERVICE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTSERVICE_EXPORT
#endif



*/


#endif // MYSHAREDLIB_GLOBAL_H
