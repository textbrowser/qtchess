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
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QtDebug>

#ifdef Q_OS_MAC
#if QT_VERSION >= 0x050000
#include "CocoaInitializer.h"
#endif
#endif
#include "qtchess.h"
#include "qtchess_comm.h"
#include "qtchess_gui.h"

qtchess *chess = 0;
qtchess_comm *comm = 0;
qtchess_gui *gui = 0;

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN32
#if QT_VERSION >= 0x050300
  QCoreApplication::setAttribute(Qt::AA_UseOpenGLES, true);
#endif
#endif
  QApplication application(argc, argv);

#ifdef Q_OS_MAC
#if QT_VERSION >= 0x050000
  /*
  ** Eliminate warnings.
  */

  CocoaInitializer ci;
#endif
#endif

  if(!QGLFormat::hasOpenGL())
    {
      qDebug() << "OpenGL is not supported on this system.";
      return EXIT_FAILURE;
    }

  if((chess = new(std::nothrow) qtchess()) == 0)
    {
      qDebug() << "Memory allocation failure.";
      return EXIT_FAILURE;
    }

  chess->init();

  if((gui = new(std::nothrow) qtchess_gui()) == 0)
    chess->quit("Memory allocation failure.", EXIT_FAILURE);

  if((comm = new(std::nothrow) qtchess_comm()) == 0)
    chess->quit("Memory allocation failure.", EXIT_FAILURE);

  /*
  ** Must be called early.
  */

  gui->init();

  /*
  ** Initialize communications.
  */

  comm->init();

  /*
  ** Display the board according to its configuration.
  */

  gui->display();
  return gui->exec();
}
