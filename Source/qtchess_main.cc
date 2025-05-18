/*
** Copyright (c) 2003 - present, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from QtChess without specific prior written permission.
**
** QTCHESS IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** QTCHESS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QApplication>
#if defined(Q_OS_ANDROID) || defined(Q_OS_WINDOWS)
#include <QDir>
#endif
#include <QSettings>
#include <QtGlobal>

#ifdef Q_OS_ANDROID
#if (QT_VERSION >= QT_VERSION_CHECK(6, 1, 0))
#include <QJniObject>
#endif
#endif

#ifdef Q_OS_MACOS
#if QT_VERSION >= 0x050000
#include "CocoaInitializer.h"
#endif
#endif
#include "qtchess.h"
#include "qtchess_communications.h"
#include "qtchess_gui.h"

QPointer<qtchess> chess;
QPointer<qtchess_communications> comm;
QPointer<qtchess_gui> gui;

int main(int argc, char *argv[])
{
#if defined(Q_OS_MACOS) || defined(Q_OS_WINDOWS)
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
#endif

  QCoreApplication::setApplicationName("QtChess");
  QCoreApplication::setOrganizationDomain("cobalt.org");
  QCoreApplication::setOrganizationName("QtChess");
  QSettings::setDefaultFormat(QSettings::IniFormat);
#if defined(Q_OS_ANDROID) || defined(Q_OS_WINDOWS)
  QSettings::setPath
    (QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());
#endif

  QApplication application(argc, argv);

#ifdef Q_OS_MACOS
#if QT_VERSION >= 0x050000
  /*
  ** Eliminate warnings.
  */

  CocoaInitializer ci;
#endif
#endif

  int rc = 0;

  {
    qtchess qtchess;
    qtchess_communications qtchess_communications;
    qtchess_gui qtchess_gui;

    chess = &qtchess;
    chess->initialize();
    comm = &qtchess_communications;
    comm->initialize();
    gui = &qtchess_gui;
    gui->initialize();
    rc = static_cast<int> (application.exec());
  }

#ifdef Q_OS_ANDROID
#if (QT_VERSION >= QT_VERSION_CHECK(6, 1, 0))
  auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());

  activity.callMethod<void> ("finishAndRemoveTask");
#endif
#endif
  return rc;
}
